#include "finddialog.h"
#include "ui_finddialog.h"
#include <QPushButton>
#include <QPlainTextEdit>
#include <QMessageBox>

FindDialog::FindDialog(QWidget *parent, QPlainTextEdit* editor)
    : QDialog(parent)
    , ui(new Ui::FindDialog)
{
    ui->setupUi(this);
    this->editor = editor;

    //find
    connect(ui->FindNextButton, &QPushButton::clicked, [this](){
        this->Find(ui->upRadio->isChecked() && !ui->downRadio->isChecked());
    });

    //close window
    connect(ui->CancelButton, &QPushButton::clicked, [this](){
        this->close();
    });
}

void FindDialog::FindNext(){
    this->Find(false);
}
void FindDialog::FindPrevious(){
    this->Find(true);
}
void FindDialog::Find(bool isPrevious){
    QTextDocument::FindFlags options{};
    QString findWhat = ui->FindWhat->text();

    if(isPrevious){
        options |= QTextDocument::FindBackward;
    }
    if(ui->checkMatchCase->isChecked()){
        options |= QTextDocument::FindCaseSensitively;
    }

    if(findWhat.length() != 0){
        bool found = this->editor->find(findWhat, options);
        if(found){
            this->editor->ensureCursorVisible();
            return;

        }else if(ui->checkWrapAround->isChecked()){
            QTextCursor originalCursor = this->editor->textCursor();
            QTextCursor cursor = this->editor->textCursor();

            //turning
            if (!isPrevious) {
                cursor.movePosition(QTextCursor::Start);
            } else {
                cursor.movePosition(QTextCursor::End);
            }
            this->editor->setTextCursor(cursor);

            //re find
            found = this->editor->find(findWhat, options);
            if(found){
                this->editor->ensureCursorVisible();
            } else {
                this->editor->setTextCursor(originalCursor);
                QMessageBox::information(this, "notepad", "Cannot find '" + findWhat + "'", QMessageBox::Ok);
            }
        }
    }
}

FindDialog::~FindDialog()
{
    delete ui;
}
