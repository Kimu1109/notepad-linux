#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QLabel>
#include <QAction>
#include <QPlainTextEdit>
#include <QFontDialog>
#include <QInputDialog>
#include <QDateTime>
#include <QLocale>
#include <QDate>
#include <QDesktopServices>
#include <QMenu>
#include <QClipboard>
#include <QMimeData>
#include <QMessageBox>
#include <QFileDialog>
#include <QComboBox>
#include <QSettings>
#include <QStandardPaths>
#include <QTimer>
#include <QProcess>
#include "zoomableplaintextedit.h"
#include "finddialog.h"
#include "aboutnotepad.h"
#include "saveformatdialog.h"
#include "savedialognotnative.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    //status bar
    statusBar()->addPermanentWidget(this->CreateSeparator());
    this->StatusLnCol = new QLabel(tr("Ln 1, Col 1"));
    statusBar()->addPermanentWidget(this->StatusLnCol);

    statusBar()->addPermanentWidget(this->CreateSeparator());
    this->StatusZoom = new QLabel("100%");
    statusBar()->addPermanentWidget(this->StatusZoom);

    statusBar()->addPermanentWidget(this->CreateSeparator());
    this->StatusNewLineCode = new QLabel("Windows(CRLF)");
    statusBar()->addPermanentWidget(this->StatusNewLineCode);

    statusBar()->addPermanentWidget(this->CreateSeparator());
    this->StatusStringFormat = new QLabel("UTF-8");
    statusBar()->addPermanentWidget(this->StatusStringFormat);

    //backup
    QTimer::singleShot(0, this, [this]() {
        if(this->settings.value("editor/back-up", true).toBool()){
            backupData backuped = this->loadBackup();
            if(!backuped.body.isEmpty()){
                QMessageBox msgBox;
                msgBox.setText(tr("Do you want to restore your unsaved %1?").arg(backuped.title));
                msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
                msgBox.setDefaultButton(QMessageBox::Yes);
                if(msgBox.exec() == QMessageBox::Yes){
                    this->editor->setPlainText(backuped.body);
                    this->ChangeFileInfo(backuped.title, backuped.path, backuped.codec, backuped.lineCode);
                }
            }
        }
    });

    //init
    this->path = "";
    this->codec_name = "Something went wrong!";
    this->ChangeTitle(tr("Untitled"));
    this->ChangeEncode(SaveFormatDialog::UTF8);
    this->ChangeLineFeedCode(SaveFormatDialog::LineFeedCode::LF);
    this->printer = new QPrinter(QPrinter::HighResolution);

    //body
    this->editor = new ZoomablePlainTextEdit(this);
    this->editor->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
    this->editor->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
    setCentralWidget(this->editor);
    connect(this->editor, &ZoomablePlainTextEdit::cursorPositionChanged, this, &MainWindow::EditorLnCol);
    connect(this->editor, &ZoomablePlainTextEdit::textChanged, this, &MainWindow::EditorTextChanged);
    connect(this->editor, &ZoomablePlainTextEdit::OnZoom, this, &MainWindow::EditorZoom);
    this->EditorZoom(this->settings.value("editor/point-font-size").toInt());

    //menu -> file
    connect(ui->actionNew, &QAction::triggered, this, &MainWindow::MenuFileNew);
    connect(ui->actionNew_Window, &QAction::triggered, this, &MainWindow::MenuFileNewWindow);
    connect(ui->actionOpen, &QAction::triggered, this, &MainWindow::MenuFileOpen);
    connect(ui->actionSave, &QAction::triggered, this, &MainWindow::MenuFileSave);
    connect(ui->actionSave_As, &QAction::triggered, this, &MainWindow::MenuFileSaveAs);
    connect(ui->actionPage_Setup, &QAction::triggered, this, &MainWindow::MenuFilePageSetup);
    connect(ui->actionPrint, &QAction::triggered, this, &MainWindow::MenuFilePrint);
    connect(ui->actionExit, &QAction::triggered, this, &MainWindow::MenuFileExit);

    //menu -> edit
    connect(ui->menuEdit, &QMenu::aboutToShow, this, &MainWindow::MenuEditOpening);
    connect(ui->actionUndo, &QAction::triggered, this->editor, &ZoomablePlainTextEdit::undo);
    connect(ui->actionCut, &QAction::triggered, this->editor, &ZoomablePlainTextEdit::cut);
    connect(ui->actionCopy, &QAction::triggered, this->editor, &ZoomablePlainTextEdit::copy);
    connect(ui->actionPaste, &QAction::triggered, this->editor, &ZoomablePlainTextEdit::paste);
    connect(ui->actionDelete, &QAction::triggered, this, &MainWindow::MenuEditDelete);
    connect(ui->actionSearch_in_Internet, &QAction::triggered, this, &MainWindow::MenuEditSearchWith);
    connect(ui->actionFind, &QAction::triggered, this, &MainWindow::MenuEditFind);
    connect(ui->actionFind_Next, &QAction::triggered, this, &MainWindow::MenuEditFindNext);
    connect(ui->actionFind_Previous, &QAction::triggered, this, &MainWindow::MenuEditFindPrevious);
    connect(ui->actionReplace, &QAction::triggered, this, &MainWindow::MenuEditReplace);
    connect(ui->actionGo_To, &QAction::triggered, this, &MainWindow::MenuEditGoTo);
    connect(ui->actionSelect_All, &QAction::triggered, this->editor, &ZoomablePlainTextEdit::selectAll);
    connect(ui->actionTime_Date, &QAction::triggered, this, &MainWindow::MenuEditTimeOrDate);

    //menu -> format
    connect(ui->menuFormat, &QMenu::aboutToShow, this, &MainWindow::MenuFormatOpening);
    connect(ui->actionWord_Wrap, &QAction::triggered, this, &MainWindow::MenuFormatWordWrap);
    connect(ui->actionFont, &QAction::triggered, this, &MainWindow::MenuFormatFont);

    //menu -> view
    connect(ui->actionZoom_In, &QAction::triggered, this, &MainWindow::MenuViewZoomZoomIn);
    connect(ui->actionZoom_Out, &QAction::triggered, this, &MainWindow::MenuViewZoomZoomOut);
    connect(ui->actionRestore_Default_Zoom, &QAction::triggered, this, &MainWindow::MenuViewZoomRestoreDefaultZoom);
    connect(ui->actionStatus_Bar, &QAction::triggered, this, &MainWindow::MenuViewStatusBar);

    //menu -> help
    connect(ui->menuHelp, &QMenu::aboutToShow, this, &MainWindow::MenuHelpOpening);
    connect(ui->actionView_Help, &QAction::triggered, this, &MainWindow::MenuHelpViewHelp);
    connect(ui->actionSeed_Feedback, &QAction::triggered, this, &MainWindow::MenuHelpSendFeedback);
    connect(ui->actionIsEnable_Backup, &QAction::triggered, this, &MainWindow::MenuHelpEnableBackup);
    connect(ui->actionEnable_Native_Save_Dialog, &QAction::triggered, this, &MainWindow::MenuHelpEnableNativeSaveDialog);
    connect(ui->actionAbout_Notepad, &QAction::triggered, this, &MainWindow::MenuHelpAboutNotepad);
    connect(ui->menuSearch_Engine, &QMenu::aboutToShow, this, &MainWindow::MenuHelpSearchEngineOpening);
    connect(ui->actionGoogle, &QAction::triggered, this, &MainWindow::MenuHelpSearchEngineGoogle);
    connect(ui->actionBing, &QAction::triggered, this, &MainWindow::MenuHelpSearchEngineBing);
    connect(ui->actionYahoo_US, &QAction::triggered, this, &MainWindow::MenuHelpSearchEngineYahooUS);
    connect(ui->actionYahoo_Japan, &QAction::triggered, this, &MainWindow::MenuHelpSearchEngineYahooJP);
    connect(ui->actionBaidu, &QAction::triggered, this, &MainWindow::MenuHelpSearchEngineBaidu);
    connect(ui->actionYandex, &QAction::triggered, this, &MainWindow::MenuHelpSearchEngineYandex);
    connect(ui->actionDuckDuckGo, &QAction::triggered, this, &MainWindow::MenuHelpSearchEngineDuckDuckGo);
    connect(ui->actionEcosia, &QAction::triggered, this, &MainWindow::MenuHelpSearchEngineEcosia);
    connect(ui->actionNaver, &QAction::triggered, this, &MainWindow::MenuHelpSearchEngineNaver);
    connect(ui->actionSeznam, &QAction::triggered, this, &MainWindow::MenuHelpSearchEngineSeznam);
    connect(ui->actionDateTimeLong, &QAction::triggered, this, &MainWindow::MenuHelpDateTimeFormatLong);
    connect(ui->actionDateTimeShort, &QAction::triggered, this, &MainWindow::MenuHelpDateTimeFormatShort);
    connect(ui->menuDate_Time_Format, &QMenu::aboutToShow, this, &MainWindow::MenuHelpDateTimeFormatOpening);
    connect(ui->actionEnglish, &QAction::triggered, this, &MainWindow::MenuHelpLanguageEnglish);
    connect(ui->actionJapanese, &QAction::triggered, this, &MainWindow::MenuHelpLanguageJapanese);
    connect(ui->menuLanguage, &QMenu::aboutToShow, this, &MainWindow::MenuHelpLanguageOpening);
}

// SYSTEMS ===================================
void MainWindow::saveBackup(const QString &text, const QString &title, const QString &path, SaveFormatDialog::Codecs codec, SaveFormatDialog::LineFeedCode lineCode)
{
    QFile file(QStandardPaths::writableLocation(
                   QStandardPaths::AppDataLocation)
               + "/backup.txt");

    QDir().mkpath(QFileInfo(file).path());

    if (file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QTextStream out(&file);
        out.setEncoding(QStringConverter::Utf8);
        out << text;

        this->settings.setValue("editor/back-up-title", title);
        this->settings.setValue("editor/back-up-path", path);
        this->settings.setValue("editor/back-up-codec", codec);
        this->settings.setValue("editor/back-up-line-feed-code", lineCode);
    }
}
MainWindow::backupData MainWindow::loadBackup()
{
    QString backupPath =
        QStandardPaths::writableLocation(QStandardPaths::AppDataLocation)
        + "/backup.txt";

    QFile file(backupPath);

    QString text = "";
    if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QTextStream in(&file);
        in.setEncoding(QStringConverter::Utf8);
        text = in.readAll();
        file.close();
    }

    backupData result = backupData();
    result.body = text;
    result.title = this->settings.value("editor/back-up-title", "Untitled").toString();
    result.path = this->settings.value("editor/back-up-path", "").toString();
    result.codec = (SaveFormatDialog::Codecs)this->settings.value("editor/back-up-codec", (int)SaveFormatDialog::Codecs::UTF8).toInt();
    result.lineCode = (SaveFormatDialog::LineFeedCode)this->settings.value("editor/back-up-line-feed-code", (int)SaveFormatDialog::LineFeedCode::LF).toInt();

    if (!text.isEmpty()) {
        QFile::remove(backupPath);

        this->settings.remove("editor/back-up-title");
        this->settings.remove("editor/back-up-path");
        this->settings.remove("editor/back-up-codec");
        this->settings.remove("editor/back-up-line-feed-code");
    }

    return result;
}

void MainWindow::ChangeFileInfo(QString title, QString path, SaveFormatDialog::Codecs codec, SaveFormatDialog::LineFeedCode lineCode){
    this->ChangeTitle(title);
    this->ChangeEncode(codec);
    this->ChangeLineFeedCode(lineCode);
    this->path = path;
}
void MainWindow::ChangeTitle(QString title){
    this->title = title;
    this->UpdateTitle();
}
void MainWindow::UpdateTitle(){
    QString windowTitle = "";
    if(
        (this->isUntitled && !this->isEmptyEditorText) ||
        (!this->isUntitled && this->editor->document()->isModified())
    ){
        windowTitle += "*";
    }
    windowTitle += tr("%1 - notepad").arg(title);
    this->setWindowTitle(windowTitle);
}
void MainWindow::ChangeEncode(SaveFormatDialog::Codecs codec){
    this->codec = codec;
    QString c = "";
    switch(codec){
    case SaveFormatDialog::Codecs::UTF8:
        c = "UTF-8";
        break;
    case SaveFormatDialog::Codecs::UTF8_BOM:
        c = "UTF-8 BOM";
        break;
    case SaveFormatDialog::Codecs::UTF16LE:
        c = "UTF-16 LE";
        break;
    case SaveFormatDialog::Codecs::UTF16BE:
        c = "UTF-16 BE";
        break;
    case SaveFormatDialog::Codecs::ShiftJIS:
        c = "Shift-JIS";
        break;
    case SaveFormatDialog::Codecs::ISO2022JP:
        c = "ISO 2022 JP";
        break;
    case SaveFormatDialog::Codecs::EUCJP:
        c = "EUC JP";
        break;
    case SaveFormatDialog::Codecs::Other:
        c = this->codec_name;
        break;
    }

    this->StatusStringFormat->setText(c);
}
void MainWindow::ChangeLineFeedCode(SaveFormatDialog::LineFeedCode lineCode){
    this->lineCode = lineCode;
    QString l = "";
    switch(lineCode){
    case SaveFormatDialog::LineFeedCode::LF:
        l = "Lf (Linux/macOS)";
        break;
    case SaveFormatDialog::LineFeedCode::CR:
        l = tr("Cr (Old MacOS)");
        break;
    case SaveFormatDialog::LineFeedCode::CRLF:
        l = "CrLf (Windows)";
        break;
    case SaveFormatDialog::LineFeedCode::Unknown:
        l = tr("Unknown");
        break;
    case SaveFormatDialog::LineFeedCode::Mixed:
        l = "Something went wrong!";
        break;
    }

    this->StatusNewLineCode->setText(l);
}
QMessageBox::StandardButton MainWindow::Save(){
    if(this->settings.value("dialog/save-dialog-is-native", true).toBool()){
        SaveFormatDialog* dia = new SaveFormatDialog(this, this->editor, this->codec, this->lineCode, this->path, this->title);
        if(dia->exec() == QDialog::Accepted){
            this->isUntitled = false;
            this->editor->document()->setModified(false);
            this->ChangeFileInfo(dia->title, dia->path, dia->encoding, dia->lineFeedCode);
            delete dia;
            return QMessageBox::Save;
        }
        delete dia;
    }else{
        IOSaveDialog result = saveDialogNotNative(this, this->editor, IOSaveDialog{this->path, this->title, this->codec, this->lineCode});
        if(result.successed){
            this->isUntitled = false;
            this->editor->document()->setModified(false);
            this->ChangeFileInfo(result.title, result.path, result.codec, result.lineFeedCode);
            return QMessageBox::Save;
        }
    }

    return QMessageBox::Cancel;
}
QMessageBox::StandardButton MainWindow::CheckUnsave(){
    if(
        (this->isUntitled && !this->isEmptyEditorText) ||
        (!this->isUntitled && this->editor->document()->isModified())
    ){
        QMessageBox msgBox;
        msgBox.setText(tr("Do you want to save changes to %1 ?").arg(this->title));
        msgBox.setStandardButtons(QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel);
        msgBox.setDefaultButton(QMessageBox::Save);
        switch (msgBox.exec()) {
        case QMessageBox::Save:
        {
            if(this->isUntitled){
                return this->Save();
            }else{
                if(SaveFormatDialog::WriteTo(QDir(this->path).filePath(this->title), this->codec, this->lineCode, this->editor)){
                    this->editor->document()->setModified(false);
                    this->UpdateTitle();
                }
            }
        }
        case QMessageBox::Discard:
            if(this->settings.value("editor/back-up", true).toBool()){
                this->saveBackup(this->editor->toPlainText(), this->title, this->path, this->codec, this->lineCode);
            }
            return QMessageBox::Discard;
        case QMessageBox::Cancel:
            return QMessageBox::Cancel;
        }
        return QMessageBox::Cancel;
    }

    return QMessageBox::Discard;
}

QLabel* MainWindow::CreateSeparator(){
    QLabel *sep = new QLabel;
    sep->setFixedWidth(1);
    sep->setStyleSheet("background-color: #888; margin-left: 4px; margin-right: 4px;");
    return sep;
}

// EDITOR ===================================
void MainWindow::EditorLnCol(){
    QTextCursor cursor = editor->textCursor();

    int line = cursor.blockNumber() + 1;
    int column = cursor.columnNumber() + 1;

    this->StatusLnCol->setText(tr("Ln %1, Col %2").arg(line).arg(column));
}
void MainWindow::EditorTextChanged(){
    if(!this->isUntitled){
        this->UpdateTitle();
        return;
    }
    bool isEmptyEditorText = editor->toPlainText().isEmpty();
    if(this->isUntitled && isEmptyEditorText != this->isEmptyEditorText){
        this->isEmptyEditorText = isEmptyEditorText;
        this->UpdateTitle();
        return;
    }
}
void MainWindow::EditorZoom(int pointFontSize){
    int percentage = pointFontSize / 14.0f * 100;
    this->StatusZoom->setText(QString::number(percentage)+"%");
}

// MENU FILE ===============================
void MainWindow::MenuFileNew(){
    if(this->CheckUnsave() == QMessageBox::Discard){
        this->isUntitled = true;
        this->editor->clear();
        this->ChangeFileInfo(
            tr("Untitled"),
            "", //path
            SaveFormatDialog::Codecs::UTF8,
            SaveFormatDialog::LineFeedCode::LF
        );
    }
}
void MainWindow::MenuFileNewWindow(){
    QProcess::startDetached(QCoreApplication::applicationFilePath());
}
void MainWindow::MenuFileOpen(){
    if(this->CheckUnsave() == QMessageBox::Discard){
        QString fileName = QFileDialog::getOpenFileName(
            this,
            tr("Open"),
            this->path.isEmpty() ? QDir::homePath() : this->path,
            tr("Text Documents(*.txt);;All Files(*.*)")
            );
        if (!fileName.isEmpty()) {
            QFile file(fileName);
            if (file.open(QIODevice::ReadOnly)) {
                QByteArray data = file.readAll();
                QFileInfo fileInfo = QFileInfo(fileName);
                file.close();

                SaveFormatDialog::TextDecodeResult result = SaveFormatDialog::DecodeBytes(data);

                this->isUntitled = false;
                this->editor->setPlainText(result.text);
                this->editor->document()->setModified(false);
                this->codec_name = result.encodingName;
                this->ChangeFileInfo(fileInfo.fileName(), fileInfo.absolutePath(), result.encoding, result.lineEnding);
            }else{
                QMessageBox(QMessageBox::Icon::Critical, tr("Opening Error"), tr("Failed to open the file.")).exec();
            }
        }
    }
}
void MainWindow::MenuFileSave(){
    if(this->isUntitled){
        this->Save();
    }else{
        if(SaveFormatDialog::WriteTo(QDir(this->path).filePath(this->title), this->codec, this->lineCode, this->editor)){
            this->editor->document()->setModified(false);
            this->UpdateTitle();
        }
    }
}
void MainWindow::MenuFileSaveAs(){
    this->Save();
}
void MainWindow::MenuFilePageSetup(){
    QPageSetupDialog dialog(this->printer, this);
    dialog.setWindowTitle(tr("Page Setup"));

    dialog.exec();
}
void MainWindow::MenuFilePrint(){
    this->printer->setOutputFileName(QDir(QDir::homePath()).filePath(this->isUntitled ? tr("Untitled.pdf") : (QFileInfo(this->title).completeBaseName() + ".pdf")));

    QPrintPreviewDialog preview(this->printer, this);
    preview.setWindowTitle(tr("Print"));

    connect(&preview, &QPrintPreviewDialog::paintRequested,
            this, [this](QPrinter *printer){
                this->editor->document()->print(printer);
            });

    preview.exec();
}
void MainWindow::MenuFileExit(){
    this->close();
}

// MENU EDIT ===============================
void MainWindow::MenuEditOpening(){
    QTextDocument* doc = editor->document();
    QTextCursor cursor = editor->textCursor();
    bool isEmpty = editor->toPlainText().isEmpty();
    bool isEmptySelected = cursor.selectedText().isEmpty();

    QClipboard *clipboard = QGuiApplication::clipboard();
    const QMimeData *mimeData = clipboard->mimeData();

    ui->actionUndo->setEnabled(doc->isUndoAvailable());
    ui->actionRedo->setEnabled(doc->isRedoAvailable());
    ui->actionCut->setEnabled(!isEmptySelected);
    ui->actionCopy->setEnabled(!isEmptySelected);
    ui->actionPaste->setEnabled(mimeData->hasText());
    ui->actionDelete->setEnabled(!isEmptySelected || !cursor.atBlockEnd());

    ui->actionSearch_in_Internet->setEnabled(!isEmptySelected);

    ui->actionFind->setEnabled(!isEmpty);
    ui->actionFind_Next->setEnabled(!isEmpty);
    ui->actionFind_Previous->setEnabled(!isEmpty);
}
void MainWindow::MenuEditDelete(){
    QTextCursor cursor = editor->textCursor();
    if (editor->textCursor().hasSelection()) {
        cursor.removeSelectedText();
    }else{
        cursor.deleteChar();
    }
    editor->setTextCursor(cursor);
}
void MainWindow::MenuEditSearchWith(){
    QString url = "";
    QString encoded = QUrl::toPercentEncoding(editor->textCursor().selectedText());

    switch ((SearchEngine)this->settings.value("editor/search-engine", SearchEngine::Google).toInt()) {

    case SearchEngine::Bing:
        url = QString("https://www.bing.com/search?q=%1").arg(encoded);
        break;
    case SearchEngine::YahooUS:
        url = QString("https://search.yahoo.com/search?p=%1").arg(encoded);
        break;
    case SearchEngine::YahooJP:
        url = QString("https://search.yahoo.co.jp/search?p=%1").arg(encoded);
        break;
    case SearchEngine::Baidu:
        url = QString("https://www.baidu.com/s?wd=%1").arg(encoded);
        break;
    case SearchEngine::Yandex:
        url = QString("https://yandex.com/search/?text=%1").arg(encoded);
        break;
    case SearchEngine::DuckDuckGo:
        url = QString("https://duckduckgo.com/?q=%1").arg(encoded);
        break;
    case SearchEngine::Ecosia:
        url = QString("https://www.ecosia.org/search?q=%1").arg(encoded);
        break;
    case SearchEngine::Seznam:
        url = QString("https://search.seznam.cz/?q=%1").arg(encoded);
        break;
    case SearchEngine::Naver:
        url = QString("https://search.naver.com/search.naver?query=%1").arg(encoded);
        break;
    case SearchEngine::Google:
    default:
        url = QString("https://www.google.com/search?q=%1").arg(encoded);
        break;

    }

    QDesktopServices::openUrl(QUrl(url));
}
void MainWindow::MenuEditFind(){
    if(this->findDialog == nullptr){
        this->findDialog = new FindDialog(this, this->editor);
        connect(this->findDialog, &QObject::destroyed, this, [&](){
            this->findDialog = nullptr;
        });
        this->findDialog->show();
        this->findDialog->setAttribute(Qt::WA_DeleteOnClose);
    }
}
void MainWindow::MenuEditFindNext(){
    if(this->findDialog != nullptr){
        this->findDialog->FindNext();
    }
}
void MainWindow::MenuEditFindPrevious(){
    if(this->findDialog != nullptr){
        this->findDialog->FindPrevious();
    }
}
void MainWindow::MenuEditReplace(){
    if(this->replaceDialog == nullptr){
        this->replaceDialog = new ReplaceDialog(this, this->editor);
        connect(this->replaceDialog, &QObject::destroyed, this, [&](){
            this->replaceDialog = nullptr;
        });
        this->replaceDialog->show();
        this->replaceDialog->setAttribute(Qt::WA_DeleteOnClose);
    }
}
void MainWindow::MenuEditGoTo(){
    QTextCursor cursor = editor->textCursor();
    int lineNumber = QInputDialog::getInt(this, tr("Go To Line"), tr("Line number:"), cursor.blockNumber() + 1, 1, editor->blockCount());
    cursor.movePosition(QTextCursor::Start);
    cursor.movePosition(QTextCursor::Down, QTextCursor::MoveAnchor, lineNumber);
    editor->setTextCursor(cursor);
}
void MainWindow::MenuEditTimeOrDate(){
    QLocale locale = QLocale::system();
    QDateTime dateTime = QDateTime::currentDateTime();

    QString dateString = "";
    if(this->settings.value("editor/is-long-date", true).toBool()){
        dateString = locale.toString(dateTime, QLocale::LongFormat);
    }else{
        dateString = locale.toString(dateTime, QLocale::ShortFormat);
    }
    editor->insertPlainText(dateString);
}

//MENU FORMAT ========================================
void MainWindow::MenuFormatOpening(){
    ui->actionWord_Wrap->setChecked(this->editor->lineWrapMode() == QPlainTextEdit::WidgetWidth);
}
void MainWindow::MenuFormatWordWrap(){
    if(ui->actionWord_Wrap->isChecked()){
        editor->setLineWrapMode(QPlainTextEdit::WidgetWidth);
        editor->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

        this->settings.setValue("editor/wrap-mode", QPlainTextEdit::WidgetWidth);
    }else{
        editor->setLineWrapMode(QPlainTextEdit::NoWrap);
        editor->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOn);

        this->settings.setValue("editor/wrap-mode", QPlainTextEdit::NoWrap);
    }

    qDebug() << this->settings.value("editor/wrap-mode");
}
void MainWindow::MenuFormatFont(){
    bool ok;
    QFont font = QFontDialog::getFont(&ok, this->editor->font(), this);
    if (ok) {
        font.setPointSize(this->editor->font().pointSize());
        this->editor->setFont(font);
        this->settings.setValue("editor/font-family", font.family());
        this->settings.setValue("editor/font-weight", (int)font.weight());
    }
}

//MENU VIEW ==========================================
void MainWindow::MenuViewOpening(){
    ui->actionStatus_Bar->setChecked(this->settings.value("editor/status-bar", true).toBool());
}
void MainWindow::MenuViewZoomZoomIn(){
    this->editor->ZoomIn();
}
void MainWindow::MenuViewZoomZoomOut(){
    this->editor->ZoomOut();
}
void MainWindow::MenuViewZoomRestoreDefaultZoom(){
    this->editor->UnZoom();
}
void MainWindow::MenuViewStatusBar(){
    if(ui->actionStatus_Bar->isChecked()){
        ui->statusbar->show();
    }else{
        ui->statusbar->hide();
    }
}

//MENU HELP ==========================================
void MainWindow::MenuHelpOpening(){
    ui->actionIsEnable_Backup->setChecked(this->settings.value("editor/back-up", true).toBool());
    ui->actionEnable_Native_Save_Dialog->setChecked(this->settings.value("dialog/save-dialog-is-native", true).toBool());
}
void MainWindow::MenuHelpViewHelp(){
    QDesktopServices::openUrl(QUrl("https://www.bing.com/search?q=get+help+with+notepad+in+windows&filters=guid:%224466414-en-dia%22%20lang:%22en%22&form=T00032&ocid=HelpPane-BingIA"));
}
void MainWindow::MenuHelpSendFeedback(){
    // TO DO!
    QDesktopServices::openUrl(QUrl("https://github.com/Kimu1109"));
}
void MainWindow::MenuHelpEnableBackup(){
    this->settings.setValue("editor/back-up", ui->actionIsEnable_Backup->isChecked());
}
void MainWindow::MenuHelpEnableNativeSaveDialog(){
    this->settings.setValue("dialog/save-dialog-is-native", ui->actionEnable_Native_Save_Dialog->isChecked());
}
void MainWindow::MenuHelpAboutNotepad(){
    if(this->aboutNotepad == nullptr){
        this->aboutNotepad = new AboutNotepad(this);
        connect(this->aboutNotepad, &QObject::destroyed, this, [&](){
            this->aboutNotepad = nullptr;
        });
        this->aboutNotepad->show();
        this->aboutNotepad->setAttribute(Qt::WA_DeleteOnClose);
    }
}
void MainWindow::MenuHelpSearchEngineOpening(){
    auto searchEngine = (SearchEngine)this->settings.value("editor/search-engine", SearchEngine::Google).toInt();
    ui->actionGoogle->setChecked(searchEngine == SearchEngine::Google);
    ui->actionBing->setChecked(searchEngine == SearchEngine::Bing);
    ui->actionYahoo_US->setChecked(searchEngine == SearchEngine::YahooUS);
    ui->actionYahoo_Japan->setChecked(searchEngine == SearchEngine::YahooJP);
    ui->actionBaidu->setChecked(searchEngine == SearchEngine::Baidu);
    ui->actionYandex->setChecked(searchEngine == SearchEngine::Yandex);
    ui->actionDuckDuckGo->setChecked(searchEngine == SearchEngine::DuckDuckGo);
    ui->actionEcosia->setChecked(searchEngine == SearchEngine::Ecosia);
    ui->actionNaver->setChecked(searchEngine == SearchEngine::Naver);
}
void MainWindow::MenuHelpSearchEngineGoogle(){
    this->settings.setValue("editor/search-engine", SearchEngine::Google);
}
void MainWindow::MenuHelpSearchEngineBing(){
    this->settings.setValue("editor/search-engine", SearchEngine::Bing);
}
void MainWindow::MenuHelpSearchEngineYahooUS(){
    this->settings.setValue("editor/search-engine", SearchEngine::YahooUS);
}
void MainWindow::MenuHelpSearchEngineYahooJP(){
    this->settings.setValue("editor/search-engine", SearchEngine::YahooJP);
}
void MainWindow::MenuHelpSearchEngineBaidu(){
    this->settings.setValue("editor/search-engine", SearchEngine::Baidu);
}
void MainWindow::MenuHelpSearchEngineYandex(){
    this->settings.setValue("editor/search-engine", SearchEngine::Yandex);
}
void MainWindow::MenuHelpSearchEngineDuckDuckGo(){
    this->settings.setValue("editor/search-engine", SearchEngine::DuckDuckGo);
}
void MainWindow::MenuHelpSearchEngineEcosia(){
    this->settings.setValue("editor/search-engine", SearchEngine::Ecosia);
}
void MainWindow::MenuHelpSearchEngineNaver(){
    this->settings.setValue("editor/search-engine", SearchEngine::Naver);
}
void MainWindow::MenuHelpSearchEngineSeznam(){
    this->settings.setValue("editor/search-engine", SearchEngine::Seznam);
}
void MainWindow::MenuHelpDateTimeFormatOpening(){
    bool IsDateLong = this->settings.value("editor/is-long-date", true).toBool();
    ui->actionDateTimeLong->setChecked(IsDateLong);
    ui->actionDateTimeShort->setChecked(!IsDateLong);
}
void MainWindow::MenuHelpDateTimeFormatLong(){
    this->settings.setValue("editor/is-long-date", true);
}
void MainWindow::MenuHelpDateTimeFormatShort(){
    this->settings.setValue("editor/is-long-date", false);
}
void MainWindow::MenuHelpLanguageOpening(){
    auto language = (LanguageList)this->settings.value("application/language", LanguageList::English).toInt();
    ui->actionEnglish->setChecked(language == LanguageList::English);
    ui->actionJapanese->setChecked(language == LanguageList::Japanese);
}
void MainWindow::MenuHelpLanguageEnglish(){
    if(this->settings.value("application/language", LanguageList::English).toInt() != (int)LanguageList::English){
        QMessageBox(QMessageBox::Icon::Information, tr("notepad"), tr("Changing the language requires restarting the app")).exec();
    }
    this->settings.setValue("application/language", LanguageList::English);
}
void MainWindow::MenuHelpLanguageJapanese(){
    if(this->settings.value("application/language", LanguageList::English).toInt() != (int)LanguageList::Japanese){
        QMessageBox(QMessageBox::Icon::Information, tr("notepad"), tr("Changing the language requires restarting the app")).exec();
    }
    this->settings.setValue("application/language", LanguageList::Japanese);
}
QString MainWindow::GetLocaleStr(){
    switch((MainWindow::LanguageList)QSettings().value("application/language", MainWindow::LanguageList::English).toInt()){
    case MainWindow::LanguageList::English:
        return "en";
    case MainWindow::LanguageList::Japanese:
        return "ja";
    }
}
void MainWindow::closeEvent(QCloseEvent *event)
{
    if(this->CheckUnsave() == QMessageBox::Cancel){
        event->ignore();
    }else{
        event->accept();
    }
}

MainWindow::~MainWindow()
{
    delete ui;
    delete this->editor;
    delete this->StatusLnCol;
    delete this->StatusNewLineCode;
    delete this->StatusStringFormat;
    delete this->StatusZoom;
    delete this->printer;
}
