#include "replacedialog.h"
#include "ui_replacedialog.h"
#include <QPlainTextEdit>
#include <QMessageBox>
#include <QPushButton>

ReplaceDialog::ReplaceDialog(QWidget *parent, QPlainTextEdit* editor)
    : QDialog(parent)
    , ui(new Ui::ReplaceDialog)
{
    ui->setupUi(this);
    this->editor = editor;

    connect(ui->FindNextButton, &QPushButton::clicked, this, &ReplaceDialog::FindNext);
    connect(ui->ReplaceButton, &QPushButton::clicked, this, &ReplaceDialog::Replace);
    connect(ui->ReplaceAllButton, &QPushButton::clicked, this, &ReplaceDialog::ReplaceAll);
    connect(ui->CancelButton, &QPushButton::clicked, [this](){
        this->close();
    });
}
void ReplaceDialog::Replace(){
    QTextCursor cursor = this->editor->textCursor();
    QString replaceWith = ui->ReplaceWith->text();
    QString selected = cursor.selectedText();

    if(ui->FindWhat->text().toLower() == selected.toLower()){
        cursor.insertText(replaceWith);
        editor->setTextCursor(cursor);

        this->FindNext();
    }
    if(selected.length() == 0){
        this->FindNext();
    }
}
void ReplaceDialog::ReplaceAll() {
    QString findWhat = ui->FindWhat->text();
    QString replaceWith = ui->ReplaceWith->text();

    if (findWhat.isEmpty())
        return;

    QPlainTextEdit* editor = this->editor;
    QTextDocument* doc = editor->document();

    QTextCursor originalCursor = editor->textCursor();

    QTextCursor cursor(doc);
    cursor.beginEditBlock();

    QString originalText = doc->toPlainText();
    QString replacedText = originalText;

    if(ui->checkMatchCase->isChecked()){
        replacedText.replace(findWhat, replaceWith, Qt::CaseSensitive);
    }else{
        replacedText.replace(findWhat, replaceWith, Qt::CaseInsensitive);
    }

    if (replacedText == originalText) {
        cursor.endEditBlock();
        return;
    }

    cursor.select(QTextCursor::Document);
    cursor.insertText(replacedText);

    cursor.endEditBlock();
    editor->setTextCursor(originalCursor);
}

void ReplaceDialog::FindNext(){
    QTextDocument::FindFlags options{};
    QString findWhat = ui->FindWhat->text();

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

            cursor.movePosition(QTextCursor::Start);
            this->editor->setTextCursor(cursor);

            //re find
            found = this->editor->find(findWhat, options);
            if(found){
                this->editor->ensureCursorVisible();
            } else {
                this->editor->setTextCursor(originalCursor);
                QMessageBox::information(this, tr("notepad"), tr("Cannot find '%1'").arg(findWhat), QMessageBox::Ok);
            }
        }
    }
}

ReplaceDialog::~ReplaceDialog()
{
    delete ui;
}
