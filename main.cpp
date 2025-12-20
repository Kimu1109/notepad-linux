#include "mainwindow.h"
#include "welcomedialog.h"

#include <QApplication>
#include <QTranslator>
#include <QLocale>
#include <QLibraryInfo>

int main(int argc, char *argv[])
{
    QCoreApplication::setOrganizationName("k-studio.ddo.jp");
    QCoreApplication::setApplicationName("notepad-linux");

    QTranslator qtTranslator;
    QTranslator appTranslator;

    QApplication a(argc, argv);
    QSettings setting;

    if(setting.value("application/first-execute", true).toBool()){
        WelcomeDialog welcome(&a, &appTranslator);
        if(welcome.exec() != QDialog::Accepted){
            return 0;
        }
    }

    if (qtTranslator.load(
            "qtbase_" + MainWindow::GetLocaleStr(),
            "/usr/share/qt6/translations"
            )
        )
    {
        a.installTranslator(&qtTranslator);
    }

    if (setting.value("application/language", MainWindow::LanguageList::English).toInt() == MainWindow::LanguageList::Japanese && appTranslator.load(":/i18n/app_ja.qm")) {
        a.installTranslator(&appTranslator);
    }

    MainWindow w;
    w.show();
    return a.exec();
}
