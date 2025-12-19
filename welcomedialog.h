#ifndef WELCOMEDIALOG_H
#define WELCOMEDIALOG_H

#include <QDialog>
#include <QTranslator>

namespace Ui {
class WelcomeDialog;
}

class WelcomeDialog : public QDialog
{
    Q_OBJECT

public:
    explicit WelcomeDialog(QApplication *app = nullptr, QTranslator* translator = nullptr, QWidget *parent = nullptr);
    ~WelcomeDialog();

protected:
    void changeEvent(QEvent *event) override;

private:
    Ui::WelcomeDialog *ui;
    QApplication *app;
    QTranslator *translater;

    void OkButtonPressed();
    void LanguageComboChanged(int index);
};

#endif // WELCOMEDIALOG_H
