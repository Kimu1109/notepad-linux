#include "saveformatdialog.h"
#include "ui_saveformatdialog.h"
#include <QPushButton>
#include <QFileDialog>
#include <QFileInfo>
#include <QDir>
#include <QPlainTextEdit>
#include <QStringConverter>
#include "character_controller.h"
#include <QMessageBox>
#include <unicode/ucsdet.h>
#include <unicode/ucnv.h>
#include <QFile>
#include <QPlainTextEdit>

SaveFormatDialog::SaveFormatDialog(
    QWidget *parent,
    QPlainTextEdit* editor,
    Codecs encoding,
    LineFeedCode lineCode,
    QString path,
    QString title
)
    : QDialog(parent)
    , ui(new Ui::SaveFormatDialog)
{
    ui->setupUi(this);

    for(const auto& [num, str] : this->getCodecs()){
        ui->EncodingCombo->addItem(str, num);
    }
    for(const auto& [num, str] : this->getLineFeedCodes()){
        ui->LineCodeCombo->addItem(str, num);
    }

    this->encoding = encoding;
    this->lineFeedCode = lineCode;

    ui->EncodingCombo->setCurrentIndex((int)encoding);
    if(lineCode == LineFeedCode::Unknown){
        ui->LineCodeCombo->setCurrentIndex(LineFeedCode::LF);
    }else{
        ui->LineCodeCombo->setCurrentIndex((int)lineCode);
    }

    this->path = path;
    this->title = title;
    if(!this->path.isEmpty() && !this->title.isEmpty()){
        ui->FilePathLineEdit->setText(QDir(this->path).filePath(this->title));
    }

    this->editor = editor;

    connect(ui->SelectButton, &QPushButton::clicked, this, [this](){
        QString filePath = QFileDialog::getSaveFileName(this, tr("Save As"), this->path.isEmpty() ? QDir::homePath() : QDir(this->path).filePath(this->title), tr("TextDocuments (*.txt);;All Files (*)"));
        if(filePath != ""){
            ui->FilePathLineEdit->setText(filePath);
        }
    });
    connect(ui->CancelButton, &QPushButton::clicked, this, [this](){
        this->reject();
    });
    connect(ui->SaveButton, &QPushButton::clicked, this, [this](){
        QString filePath = ui->FilePathLineEdit->text();
        Codecs encoding = qvariant_cast<Codecs>(ui->EncodingCombo->itemData(ui->EncodingCombo->currentIndex()));
        LineFeedCode lineCode = qvariant_cast<LineFeedCode>(ui->LineCodeCombo->itemData(ui->LineCodeCombo->currentIndex()));

        QFileInfo fileInfo(filePath);
        QDir dir(fileInfo.path());

        if(filePath == "" || !dir.exists()){
            filePath = QFileDialog::getSaveFileName(this, tr("Save As"), this->path.isEmpty() ? QDir::homePath() : QDir(this->path).filePath(this->title), tr("TextDocuments (*.txt);;All Files (*)"));
            if(filePath == ""){
                return;
            }
            ui->FilePathLineEdit->setText(filePath);
            fileInfo = QFileInfo(filePath);
        }

        if(this->WriteTo(filePath, encoding, lineCode, this->editor)){
            this->path = fileInfo.absolutePath();
            this->title = fileInfo.fileName();
            this->encoding = encoding;
            this->lineFeedCode = lineCode;
            this->accept();
            return;
        }
        this->reject();
    });

    ui->SaveButton->setFocus();
}
bool SaveFormatDialog::WriteTo(QString filePath, Codecs encoding, LineFeedCode lineCode, QPlainTextEdit* editor){
    QFileInfo fileInfo(filePath);
    QDir dir(fileInfo.path());

    QFile file(filePath);
    if (file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QString plainText = editor->toPlainText();

        switch(lineCode){
        case LineFeedCode::Unknown:
        case LineFeedCode::LF:
            plainText.replace("\r\n", "\n"); //win -> unix
            plainText.replace("\r", "\n"); //old mac os -> unix
            break;
        case LineFeedCode::CRLF:
            plainText.replace("\r\n", "\n"); //win -> unix
            plainText.replace("\r", "\n"); //old mac os -> unix
            plainText.replace("\n", "\r\n");  //unix -> windows!
            break;
        case LineFeedCode::CR:
            plainText.replace("\r\n", "\n"); //win -> unix
            plainText.replace("\r", "\n"); //old mac os -> unix
            plainText.replace("\n", "\r"); //unix -> old mac os!
            break;
        default:
            break;
        }

        if (encoding == Codecs::UTF8_BOM) {
            static const unsigned char bom[] = {0xEF, 0xBB, 0xBF};
            file.write((const char*)bom, 3);
        } else if (encoding == Codecs::UTF16LE) {
            static const unsigned char bom[] = {0xFF, 0xFE};
            file.write((const char*)bom, 2);
        } else if (encoding == Codecs::UTF16BE) {
            static const unsigned char bom[] = {0xFE, 0xFF};
            file.write((const char*)bom, 2);
        }

        // ICU (Shift-JIS)
        if (const char* icuName = codecToICUName(encoding)) {
            QByteArray bytes = toICUEncoded(plainText, icuName);
            file.write(bytes);
        }else{
            // UTF 系 (Qt 標準)
            QTextStream out(&file);

            if (encoding == Codecs::UTF8 || encoding == Codecs::UTF8_BOM)
                out.setEncoding(QStringConverter::Utf8);
            else if (encoding == Codecs::UTF16LE)
                out.setEncoding(QStringConverter::Utf16LE);
            else if (encoding == Codecs::UTF16BE)
                out.setEncoding(QStringConverter::Utf16BE);

            out << plainText;
        }

        file.close();
        return true;
    } else {
        QMessageBox(QMessageBox::Icon::Critical, tr("Saving Error"), tr("Failed to open the file.")).exec();
        return false;
    }
}
SaveFormatDialog::LineFeedCode SaveFormatDialog::detectLineEnding(const QByteArray& data)
{
    bool hasLF = false;
    bool hasCRLF = false;
    bool hasCR = false;

    for (int i = 0; i < data.size(); ++i) {
        if (data[i] == '\n') {
            if (i > 0 && data[i - 1] == '\r')
                hasCRLF = true;
            else
                hasLF = true;
        } else if (data[i] == '\r') {
            if (i + 1 >= data.size() || data[i + 1] != '\n')
                hasCR = true;
        }
    }

    int count = hasLF + hasCRLF + hasCR;
    if (count > 1) return LineFeedCode::Mixed;
    if (hasCRLF) return LineFeedCode::CRLF;
    if (hasLF)   return LineFeedCode::LF;
    if (hasCR)   return LineFeedCode::CR;
    return LineFeedCode::Unknown;
}
QByteArray SaveFormatDialog::normalizeLineEndingToLF(const QByteArray& data)
{
    QByteArray out;
    out.reserve(data.size());

    for (int i = 0; i < data.size(); ++i) {
        if (data[i] == '\r') {
            // CRLF or CR → LF
            if (i + 1 < data.size() && data[i + 1] == '\n') {
                ++i; // skip LF
            }
            out.append('\n');
        } else {
            out.append(data[i]);
        }
    }
    return out;
}
SaveFormatDialog::TextDecodeResult SaveFormatDialog::DecodeBytes(const QByteArray& rawData){
    TextDecodeResult result;
    result.lineEnding = detectLineEnding(rawData);

    QByteArray data = rawData;

    // ---------- Mixed のみ LF に統一 ----------
    if (result.lineEnding == LineFeedCode::Mixed) {
        data = normalizeLineEndingToLF(rawData);
        result.lineEnding = LineFeedCode::LF;
    }

    // ---------- BOM ----------
    if (data.startsWith("\xEF\xBB\xBF")) {
        result.text = QStringDecoder(QStringDecoder::Utf8)
                          .decode(data.mid(3));
        result.encoding = Codecs::UTF8_BOM;
        result.encodingName = "UTF-8 BOM";
        return result;
    }

    if (data.startsWith("\xFF\xFE")) {
        result.text = QStringDecoder(QStringDecoder::Utf16LE)
                          .decode(data.mid(2));
        result.encoding = Codecs::UTF16LE;
        result.encodingName = "UTF-16 LE";
        return result;
    }

    if (data.startsWith("\xFE\xFF")) {
        result.text = QStringDecoder(QStringDecoder::Utf16BE)
                          .decode(data.mid(2));
        result.encoding = Codecs::UTF16BE;
        result.encodingName = "UTF-16 BE";
        return result;
    }

    // ---------- UTF-8 (BOMなし) ----------
    if (!data.contains(char(0x1B))) { //is not contains esc!
        QStringDecoder dec(QStringDecoder::Utf8);
        QString s = dec.decode(data);
        if (!dec.hasError()) {
            result.text = s;
            result.encoding = Codecs::UTF8;
            result.encodingName = "UTF-8";
            return result;
        }
    }

    // ---------- ICU ----------
    UErrorCode status = U_ZERO_ERROR;
    UCharsetDetector* detector = ucsdet_open(&status);
    ucsdet_setText(detector, data.constData(), data.size(), &status);

    const UCharsetMatch* match = ucsdet_detect(detector, &status);
    if (U_SUCCESS(status) && match) {
        const char* charset = ucsdet_getName(match, &status);
        result.encodingName = charset;

        if (QString(charset).contains("Shift_JIS", Qt::CaseInsensitive))
            result.encoding = Codecs::ShiftJIS;
        else if (QString(charset).contains("EUC-JP", Qt::CaseInsensitive))
            result.encoding = Codecs::EUCJP;
        else if (QString(charset).contains("ISO-2022-JP", Qt::CaseInsensitive))
            result.encoding = Codecs::ISO2022JP;
        else
            result.encoding = Codecs::Other;

        UConverter* conv = ucnv_open(charset, &status);

        int32_t len = ucnv_toUChars(
            conv, nullptr, 0,
            data.constData(), data.size(), &status
            );
        status = U_ZERO_ERROR;

        QVector<UChar> buf(len + 1);
        ucnv_toUChars(
            conv, buf.data(), buf.size(),
            data.constData(), data.size(), &status
            );

        ucnv_close(conv);
        ucsdet_close(detector);

        result.text = QString::fromUtf16(
            reinterpret_cast<const char16_t*>(buf.data()),
            len
            );
        return result;
    }

    ucsdet_close(detector);

    // ---------- fallback ----------
    result.text = QStringDecoder(QStringDecoder::Latin1).decode(data);
    result.encoding = Codecs::Other;
    result.encodingName = "Latin-1 fallback";
    return result;
}
const char* SaveFormatDialog::codecToICUName(Codecs c)
{
    switch (c) {
    case Codecs::ShiftJIS:   return "Shift_JIS";
    case Codecs::EUCJP:      return "EUC-JP";
    case Codecs::ISO2022JP:  return "ISO-2022-JP";
    default:                 return nullptr;
    }
}
QList<std::tuple<SaveFormatDialog::Codecs, QString>> SaveFormatDialog::getCodecs(){
    return {
        std::forward_as_tuple(Codecs::UTF8, "UTF-8"),
        std::forward_as_tuple(Codecs::UTF8_BOM, "UTF-8 BOM"),
        std::forward_as_tuple(Codecs::UTF16LE, "UTF-16 LE"),
        std::forward_as_tuple(Codecs::UTF16BE, "UTF-16 BE"),
        std::forward_as_tuple(Codecs::ShiftJIS, "Shift-JIS"),
        std::forward_as_tuple(Codecs::EUCJP, "EUC JP"),
        std::forward_as_tuple(Codecs::ISO2022JP, "ISO 2022 JP")
    };
}
QList<std::tuple<SaveFormatDialog::LineFeedCode, QString>> SaveFormatDialog::getLineFeedCodes(){
    return {
        std::forward_as_tuple(LineFeedCode::LF, "Lf (Linux/macOS)"),
        std::forward_as_tuple(LineFeedCode::CRLF, "CrLf (Windows)"),
        std::forward_as_tuple(LineFeedCode::CR, "Cr (Old MacOS)")
    };
}

SaveFormatDialog::~SaveFormatDialog()
{
    delete ui;
}
