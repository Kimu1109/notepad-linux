/********************************************************************************
** Form generated from reading UI file 'saveformatdialog.ui'
**
** Created by: Qt User Interface Compiler version 6.4.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_SAVEFORMATDIALOG_H
#define UI_SAVEFORMATDIALOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QDialog>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>

QT_BEGIN_NAMESPACE

class Ui_SaveFormatDialog
{
public:
    QComboBox *EncodingCombo;
    QComboBox *LineCodeCombo;
    QLabel *label;
    QLabel *label_2;
    QLabel *label_3;
    QLineEdit *FilePathLineEdit;
    QPushButton *SelectButton;
    QPushButton *SaveButton;
    QPushButton *CancelButton;

    void setupUi(QDialog *SaveFormatDialog)
    {
        if (SaveFormatDialog->objectName().isEmpty())
            SaveFormatDialog->setObjectName("SaveFormatDialog");
        SaveFormatDialog->resize(531, 176);
        EncodingCombo = new QComboBox(SaveFormatDialog);
        EncodingCombo->setObjectName("EncodingCombo");
        EncodingCombo->setGeometry(QRect(120, 60, 151, 31));
        LineCodeCombo = new QComboBox(SaveFormatDialog);
        LineCodeCombo->setObjectName("LineCodeCombo");
        LineCodeCombo->setGeometry(QRect(120, 100, 151, 31));
        label = new QLabel(SaveFormatDialog);
        label->setObjectName("label");
        label->setGeometry(QRect(50, 60, 71, 31));
        QFont font;
        font.setPointSize(12);
        label->setFont(font);
        label_2 = new QLabel(SaveFormatDialog);
        label_2->setObjectName("label_2");
        label_2->setGeometry(QRect(10, 100, 111, 31));
        label_2->setFont(font);
        label_3 = new QLabel(SaveFormatDialog);
        label_3->setObjectName("label_3");
        label_3->setGeometry(QRect(50, 20, 71, 31));
        label_3->setFont(font);
        FilePathLineEdit = new QLineEdit(SaveFormatDialog);
        FilePathLineEdit->setObjectName("FilePathLineEdit");
        FilePathLineEdit->setGeometry(QRect(120, 20, 341, 31));
        SelectButton = new QPushButton(SaveFormatDialog);
        SelectButton->setObjectName("SelectButton");
        SelectButton->setGeometry(QRect(460, 20, 61, 31));
        SaveButton = new QPushButton(SaveFormatDialog);
        SaveButton->setObjectName("SaveButton");
        SaveButton->setGeometry(QRect(430, 140, 91, 31));
        CancelButton = new QPushButton(SaveFormatDialog);
        CancelButton->setObjectName("CancelButton");
        CancelButton->setGeometry(QRect(330, 140, 91, 31));

        retranslateUi(SaveFormatDialog);

        QMetaObject::connectSlotsByName(SaveFormatDialog);
    } // setupUi

    void retranslateUi(QDialog *SaveFormatDialog)
    {
        SaveFormatDialog->setWindowTitle(QCoreApplication::translate("SaveFormatDialog", "Save As", nullptr));
        label->setText(QCoreApplication::translate("SaveFormatDialog", "Encoding", nullptr));
        label_2->setText(QCoreApplication::translate("SaveFormatDialog", "Line feed code", nullptr));
        label_3->setText(QCoreApplication::translate("SaveFormatDialog", "File Path", nullptr));
        SelectButton->setText(QCoreApplication::translate("SaveFormatDialog", "Select", nullptr));
        SaveButton->setText(QCoreApplication::translate("SaveFormatDialog", "Save", nullptr));
        CancelButton->setText(QCoreApplication::translate("SaveFormatDialog", "Cancel", nullptr));
    } // retranslateUi

};

namespace Ui {
    class SaveFormatDialog: public Ui_SaveFormatDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SAVEFORMATDIALOG_H
