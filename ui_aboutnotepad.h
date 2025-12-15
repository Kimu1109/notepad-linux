/********************************************************************************
** Form generated from reading UI file 'aboutnotepad.ui'
**
** Created by: Qt User Interface Compiler version 6.4.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_ABOUTNOTEPAD_H
#define UI_ABOUTNOTEPAD_H

#include <QtCore/QVariant>
#include <QtGui/QIcon>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QLabel>

QT_BEGIN_NAMESPACE

class Ui_AboutNotepad
{
public:
    QLabel *ImgLabel;
    QLabel *BodyLabel;

    void setupUi(QDialog *AboutNotepad)
    {
        if (AboutNotepad->objectName().isEmpty())
            AboutNotepad->setObjectName("AboutNotepad");
        AboutNotepad->resize(474, 159);
        QIcon icon;
        icon.addFile(QString::fromUtf8(":/new/prefix/notepad-linux-icon.png"), QSize(), QIcon::Normal, QIcon::Off);
        AboutNotepad->setWindowIcon(icon);
        ImgLabel = new QLabel(AboutNotepad);
        ImgLabel->setObjectName("ImgLabel");
        ImgLabel->setGeometry(QRect(10, 10, 131, 141));
        ImgLabel->setPixmap(QPixmap(QString::fromUtf8(":/new/prefix/k-studio.ddo.jp.png")));
        BodyLabel = new QLabel(AboutNotepad);
        BodyLabel->setObjectName("BodyLabel");
        BodyLabel->setGeometry(QRect(140, 10, 321, 131));
        BodyLabel->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignTop);

        retranslateUi(AboutNotepad);

        QMetaObject::connectSlotsByName(AboutNotepad);
    } // setupUi

    void retranslateUi(QDialog *AboutNotepad)
    {
        AboutNotepad->setWindowTitle(QCoreApplication::translate("AboutNotepad", "About notepad", nullptr));
        ImgLabel->setText(QString());
        BodyLabel->setText(QCoreApplication::translate("AboutNotepad", "Notepad by k-studio.ddo.jp<br/>\n"
"Created by Kimu1109<br/>\n"
"<br/>\n"
"<a href=\"https://github.com/Kimu1109\">GitHub</a><br/>\n"
"<a href=\"https://k-studio.ddo.jp\">HomePage</a><br/>\n"
"<br/>\n"
"(c) Copyright 2025 K-Studio.ddo.jp InterMediant", nullptr));
    } // retranslateUi

};

namespace Ui {
    class AboutNotepad: public Ui_AboutNotepad {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_ABOUTNOTEPAD_H
