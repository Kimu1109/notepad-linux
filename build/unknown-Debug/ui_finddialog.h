/********************************************************************************
** Form generated from reading UI file 'finddialog.ui'
**
** Created by: Qt User Interface Compiler version 6.4.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_FINDDIALOG_H
#define UI_FINDDIALOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QDialog>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QRadioButton>

QT_BEGIN_NAMESPACE

class Ui_FindDialog
{
public:
    QLabel *label;
    QLineEdit *FindWhat;
    QPushButton *FindNextButton;
    QPushButton *CancelButton;
    QCheckBox *checkMatchCase;
    QCheckBox *checkWrapAround;
    QGroupBox *groupBox;
    QRadioButton *upRadio;
    QRadioButton *downRadio;

    void setupUi(QDialog *FindDialog)
    {
        if (FindDialog->objectName().isEmpty())
            FindDialog->setObjectName("FindDialog");
        FindDialog->resize(456, 130);
        QSizePolicy sizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(FindDialog->sizePolicy().hasHeightForWidth());
        FindDialog->setSizePolicy(sizePolicy);
        label = new QLabel(FindDialog);
        label->setObjectName("label");
        label->setGeometry(QRect(10, 10, 71, 18));
        FindWhat = new QLineEdit(FindDialog);
        FindWhat->setObjectName("FindWhat");
        FindWhat->setGeometry(QRect(90, 10, 251, 26));
        FindNextButton = new QPushButton(FindDialog);
        FindNextButton->setObjectName("FindNextButton");
        FindNextButton->setGeometry(QRect(360, 10, 88, 26));
        CancelButton = new QPushButton(FindDialog);
        CancelButton->setObjectName("CancelButton");
        CancelButton->setGeometry(QRect(360, 50, 88, 26));
        checkMatchCase = new QCheckBox(FindDialog);
        checkMatchCase->setObjectName("checkMatchCase");
        checkMatchCase->setGeometry(QRect(10, 60, 101, 24));
        checkWrapAround = new QCheckBox(FindDialog);
        checkWrapAround->setObjectName("checkWrapAround");
        checkWrapAround->setGeometry(QRect(10, 90, 111, 24));
        groupBox = new QGroupBox(FindDialog);
        groupBox->setObjectName("groupBox");
        groupBox->setGeometry(QRect(200, 50, 141, 71));
        upRadio = new QRadioButton(groupBox);
        upRadio->setObjectName("upRadio");
        upRadio->setGeometry(QRect(10, 40, 51, 24));
        downRadio = new QRadioButton(groupBox);
        downRadio->setObjectName("downRadio");
        downRadio->setGeometry(QRect(70, 40, 61, 24));
        downRadio->setChecked(true);

        retranslateUi(FindDialog);

        QMetaObject::connectSlotsByName(FindDialog);
    } // setupUi

    void retranslateUi(QDialog *FindDialog)
    {
        FindDialog->setWindowTitle(QCoreApplication::translate("FindDialog", "Find", nullptr));
        label->setText(QCoreApplication::translate("FindDialog", "Find what:", nullptr));
        FindNextButton->setText(QCoreApplication::translate("FindDialog", "Find Next", nullptr));
        CancelButton->setText(QCoreApplication::translate("FindDialog", "Cancel", nullptr));
        checkMatchCase->setText(QCoreApplication::translate("FindDialog", "Match case", nullptr));
        checkWrapAround->setText(QCoreApplication::translate("FindDialog", "Wrap around", nullptr));
        groupBox->setTitle(QCoreApplication::translate("FindDialog", "Direction", nullptr));
        upRadio->setText(QCoreApplication::translate("FindDialog", "Up", nullptr));
        downRadio->setText(QCoreApplication::translate("FindDialog", "Down", nullptr));
    } // retranslateUi

};

namespace Ui {
    class FindDialog: public Ui_FindDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_FINDDIALOG_H
