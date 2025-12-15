/********************************************************************************
** Form generated from reading UI file 'replacedialog.ui'
**
** Created by: Qt User Interface Compiler version 6.4.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_REPLACEDIALOG_H
#define UI_REPLACEDIALOG_H

#include <QtCore/QVariant>
#include <QtGui/QIcon>
#include <QtWidgets/QApplication>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QDialog>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>

QT_BEGIN_NAMESPACE

class Ui_ReplaceDialog
{
public:
    QLabel *label;
    QPushButton *FindNextButton;
    QCheckBox *checkWrapAround;
    QLineEdit *FindWhat;
    QCheckBox *checkMatchCase;
    QPushButton *CancelButton;
    QPushButton *ReplaceButton;
    QPushButton *ReplaceAllButton;
    QLineEdit *ReplaceWith;
    QLabel *label_2;

    void setupUi(QDialog *ReplaceDialog)
    {
        if (ReplaceDialog->objectName().isEmpty())
            ReplaceDialog->setObjectName("ReplaceDialog");
        ReplaceDialog->resize(481, 172);
        QIcon icon;
        icon.addFile(QString::fromUtf8(":/new/prefix/notepad-linux-icon.png"), QSize(), QIcon::Normal, QIcon::Off);
        ReplaceDialog->setWindowIcon(icon);
        label = new QLabel(ReplaceDialog);
        label->setObjectName("label");
        label->setGeometry(QRect(10, 10, 101, 18));
        FindNextButton = new QPushButton(ReplaceDialog);
        FindNextButton->setObjectName("FindNextButton");
        FindNextButton->setGeometry(QRect(380, 10, 88, 26));
        checkWrapAround = new QCheckBox(ReplaceDialog);
        checkWrapAround->setObjectName("checkWrapAround");
        checkWrapAround->setGeometry(QRect(10, 140, 111, 24));
        checkWrapAround->setChecked(true);
        FindWhat = new QLineEdit(ReplaceDialog);
        FindWhat->setObjectName("FindWhat");
        FindWhat->setGeometry(QRect(110, 10, 251, 26));
        checkMatchCase = new QCheckBox(ReplaceDialog);
        checkMatchCase->setObjectName("checkMatchCase");
        checkMatchCase->setGeometry(QRect(10, 110, 101, 24));
        CancelButton = new QPushButton(ReplaceDialog);
        CancelButton->setObjectName("CancelButton");
        CancelButton->setGeometry(QRect(380, 100, 88, 26));
        ReplaceButton = new QPushButton(ReplaceDialog);
        ReplaceButton->setObjectName("ReplaceButton");
        ReplaceButton->setGeometry(QRect(380, 40, 88, 26));
        ReplaceAllButton = new QPushButton(ReplaceDialog);
        ReplaceAllButton->setObjectName("ReplaceAllButton");
        ReplaceAllButton->setGeometry(QRect(380, 70, 88, 26));
        ReplaceWith = new QLineEdit(ReplaceDialog);
        ReplaceWith->setObjectName("ReplaceWith");
        ReplaceWith->setGeometry(QRect(110, 40, 251, 26));
        label_2 = new QLabel(ReplaceDialog);
        label_2->setObjectName("label_2");
        label_2->setGeometry(QRect(10, 40, 101, 18));

        retranslateUi(ReplaceDialog);

        QMetaObject::connectSlotsByName(ReplaceDialog);
    } // setupUi

    void retranslateUi(QDialog *ReplaceDialog)
    {
        ReplaceDialog->setWindowTitle(QCoreApplication::translate("ReplaceDialog", "Replace", nullptr));
        label->setText(QCoreApplication::translate("ReplaceDialog", "Find what:", nullptr));
        FindNextButton->setText(QCoreApplication::translate("ReplaceDialog", "Find Next", nullptr));
        checkWrapAround->setText(QCoreApplication::translate("ReplaceDialog", "Wrap around", nullptr));
        checkMatchCase->setText(QCoreApplication::translate("ReplaceDialog", "Match case", nullptr));
        CancelButton->setText(QCoreApplication::translate("ReplaceDialog", "Cancel", nullptr));
        ReplaceButton->setText(QCoreApplication::translate("ReplaceDialog", "Replace", nullptr));
        ReplaceAllButton->setText(QCoreApplication::translate("ReplaceDialog", "Replace All", nullptr));
        label_2->setText(QCoreApplication::translate("ReplaceDialog", "Replace with:", nullptr));
    } // retranslateUi

};

namespace Ui {
    class ReplaceDialog: public Ui_ReplaceDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_REPLACEDIALOG_H
