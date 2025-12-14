#ifndef FINDDIALOG_H
#define FINDDIALOG_H

#include <QDialog>
#include <QPlainTextEdit>

namespace Ui {
class FindDialog;
}

class FindDialog : public QDialog
{
    Q_OBJECT

public:
    explicit FindDialog(QWidget *parent = nullptr, QPlainTextEdit* editor = nullptr);

    void FindNext();
    void FindPrevious();

    ~FindDialog();

private:
    Ui::FindDialog *ui;
    QPlainTextEdit* editor;

    void Find(bool isPrevious);
};

#endif // FINDDIALOG_H
