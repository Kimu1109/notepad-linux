#ifndef REPLACEDIALOG_H
#define REPLACEDIALOG_H

#include <QDialog>
#include <QPlainTextEdit>

namespace Ui {
class ReplaceDialog;
}

class ReplaceDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ReplaceDialog(QWidget *parent = nullptr, QPlainTextEdit* editor = nullptr);
    ~ReplaceDialog();

private:
    Ui::ReplaceDialog *ui;
    QPlainTextEdit* editor;

    void Replace();
    void ReplaceAll();
    void FindNext();
};

#endif // REPLACEDIALOG_H
