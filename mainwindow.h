#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLabel>
#include <QComboBox>
#include <zoomableplaintextedit.h>
#include <finddialog.h>
#include <replacedialog.h>
#include <aboutnotepad.h>
#include <saveformatdialog.h>
#include <QMessageBox>
#include <QPrintPreviewDialog>
#include <QPrinter>
#include <QPrintDialog>
#include <QTextDocument>
#include <QPageSetupDialog>
#include <QSettings>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

protected:
    void closeEvent(QCloseEvent *event) override;

private:
    enum SearchEngine {
        Google,
        Bing,
        YahooUS,
        YahooJP,
        Baidu,
        Yandex,
        DuckDuckGo,
        Ecosia,
        Seznam,
        Naver
    };

    const QString DEFAULT_TITLE = "Untitled";

    Ui::MainWindow *ui;
    ZoomablePlainTextEdit* editor;
    FindDialog* findDialog = nullptr;
    ReplaceDialog* replaceDialog = nullptr;
    AboutNotepad* aboutNotepad = nullptr;

    QLabel* StatusLnCol;
    QLabel* StatusZoom;
    QLabel* StatusNewLineCode;
    QLabel* StatusStringFormat;

    QLabel* CreateSeparator();

    QString path;
    QString title;
    SaveFormatDialog::Codecs codec;
    QString codec_name;
    SaveFormatDialog::LineFeedCode lineCode;
    QSettings settings;

    QPrinter* printer;

    bool isEmptyEditorText = true;
    bool isUntitled = true;

    struct backupData{
        QString body;
        QString title;
        QString path;
        SaveFormatDialog::Codecs codec;
        SaveFormatDialog::LineFeedCode lineCode;
    };

    void saveBackup(const QString &text, const QString &title, const QString &path, SaveFormatDialog::Codecs codec, SaveFormatDialog::LineFeedCode lineCode);
    backupData loadBackup();

    void ChangeFileInfo(QString title, QString path, SaveFormatDialog::Codecs codec, SaveFormatDialog::LineFeedCode lineCode);
    void ChangeTitle(QString title);
    void ChangeEncode(SaveFormatDialog::Codecs codec);
    void ChangeLineFeedCode(SaveFormatDialog::LineFeedCode lineCode);
    void UpdateTitle();
    QMessageBox::StandardButton Save();
    QMessageBox::StandardButton CheckUnsave();

    void EditorLnCol();
    void EditorTextChanged();
    void EditorZoom(int pointFontSize);

    void MenuFileNew();
    void MenuFileNewWindow();
    void MenuFileOpen();
    void MenuFileSave();
    void MenuFileSaveAs();
    void MenuFilePageSetup();
    void MenuFilePrint();
    void MenuFileExit();

    void MenuEditOpening();
    void MenuEditDelete();
    void MenuEditSearchWith();
    void MenuEditFind();
    void MenuEditFindNext();
    void MenuEditFindPrevious();
    void MenuEditReplace();
    void MenuEditGoTo();
    void MenuEditTimeOrDate();

    void MenuFormatOpening();
    void MenuFormatWordWrap();
    void MenuFormatFont();

    void MenuViewOpening();
    void MenuViewZoomZoomIn();
    void MenuViewZoomZoomOut();
    void MenuViewZoomRestoreDefaultZoom();
    void MenuViewStatusBar();

    void MenuHelpOpening();
    void MenuHelpViewHelp();
    void MenuHelpSendFeedback();
    void MenuHelpEnableBackup();
    void MenuHelpAboutNotepad();
    void MenuHelpSearchEngineOpening();
    void MenuHelpSearchEngineGoogle();
    void MenuHelpSearchEngineBing();
    void MenuHelpSearchEngineYahooUS();
    void MenuHelpSearchEngineYahooJP();
    void MenuHelpSearchEngineBaidu();
    void MenuHelpSearchEngineYandex();
    void MenuHelpSearchEngineDuckDuckGo();
    void MenuHelpSearchEngineEcosia();
    void MenuHelpSearchEngineNaver();
    void MenuHelpSearchEngineSeznam();
    void MenuHelpDateTimeFormatLong();
    void MenuHelpDateTimeFormatShort();
    void MenuHelpDateTimeFormatOpening();
};
#endif // MAINWINDOW_H
