#include "mainwindow.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QCoreApplication::setOrganizationName("k-studio.ddo.jp");
    QCoreApplication::setApplicationName("notepad-linux");

    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return a.exec();
}
