#include "aboutnotepad.h"
#include "ui_aboutnotepad.h"

AboutNotepad::AboutNotepad(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::AboutNotepad)
{
    ui->setupUi(this);
    ui->BodyLabel->setTextInteractionFlags(Qt::LinksAccessibleByMouse);
    ui->BodyLabel->setOpenExternalLinks(true);
    ui->BodyLabel->setTextFormat(Qt::RichText);
}

AboutNotepad::~AboutNotepad()
{
    delete ui;
}
