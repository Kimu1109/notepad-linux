#ifndef SAVEFORMATDIALOG_H
#define SAVEFORMATDIALOG_H

#include <QDialog>
#include <QPlainTextEdit>

namespace Ui {
class SaveFormatDialog;
}

class SaveFormatDialog : public QDialog
{
    Q_OBJECT

public:
    enum Codecs {
        UTF8,
        UTF8_BOM,
        UTF16LE,
        UTF16BE,
        ShiftJIS,
        EUCJP,
        ISO2022JP,
        Other
    };
    enum LineFeedCode {
        LF,
        CRLF,
        CR,
        Unknown,
        Mixed
    };
    struct TextDecodeResult {
        QString text;
        Codecs encoding;
        QString encodingName;
        LineFeedCode lineEnding;
    };

    explicit SaveFormatDialog(
        QWidget *parent = nullptr,
        QPlainTextEdit* editor = nullptr,
        Codecs encoding = Codecs::UTF8,
        LineFeedCode lineCode = LineFeedCode::LF,
        QString path = "",
        QString title = ""
    );
    ~SaveFormatDialog();

    QString path = "";
    QString title = "";
    Codecs encoding = Codecs::UTF8;
    LineFeedCode lineFeedCode = LineFeedCode::LF;

    static bool WriteTo(QString filePath, Codecs encoding, LineFeedCode lineFeedCode, QPlainTextEdit* editor);
    static LineFeedCode detectLineEnding(const QByteArray& data);
    static QByteArray normalizeLineEndingToLF(const QByteArray& data);
    static TextDecodeResult DecodeBytes(const QByteArray& data);
    static const char* codecToICUName(Codecs c);

private:
    Ui::SaveFormatDialog *ui;
    QPlainTextEdit* editor;
};

#endif // SAVEFORMATDIALOG_H
