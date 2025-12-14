#ifndef ABOUTNOTEPAD_H
#define ABOUTNOTEPAD_H

#include <QDialog>

namespace Ui {
class AboutNotepad;
}

class AboutNotepad : public QDialog
{
    Q_OBJECT

public:
    explicit AboutNotepad(QWidget *parent = nullptr);
    ~AboutNotepad();

private:
    Ui::AboutNotepad *ui;
};

#endif // ABOUTNOTEPAD_H
