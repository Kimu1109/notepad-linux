#include "welcomedialog.h"
#include "ui_welcomedialog.h"
#include "mainwindow.h"
#include "magic_enum.hpp"
#include <QPushButton>
#include <QComboBox>

WelcomeDialog::WelcomeDialog(QApplication *app, QTranslator* translater, QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::WelcomeDialog)
{
    this->app = app;
    this->translater = translater;

    ui->setupUi(this);

    ui->LanguageCombo->addItem("English", MainWindow::LanguageList::English);
    ui->LanguageCombo->addItem("日本語", MainWindow::LanguageList::Japanese);

    auto engines = magic_enum::enum_values<MainWindow::SearchEngine>();
    for(const auto& engine: engines){
        auto str = magic_enum::enum_name(engine);
        ui->SearchEngineCombo->addItem(
            QString::fromUtf8(str.data(), static_cast<int>(str.size())),
            engine
        );
    }

    connect(ui->OkButton, &QPushButton::clicked, this, &WelcomeDialog::OkButtonPressed);
    connect(ui->LanguageCombo, &QComboBox::currentIndexChanged, this, &WelcomeDialog::LanguageComboChanged);
}
void WelcomeDialog::OkButtonPressed(){
    QSettings setting;
    setting.setValue("application/first-execute", false);
    setting.setValue("application/language", qvariant_cast<MainWindow::LanguageList>(ui->LanguageCombo->itemData(ui->LanguageCombo->currentIndex())));
    setting.setValue("editor/search-engine", qvariant_cast<MainWindow::SearchEngine>(ui->SearchEngineCombo->itemData(ui->SearchEngineCombo->currentIndex())));
    setting.setValue("editor/back-up", ui->EnableBackUpCheck->isChecked());
    setting.setValue("dialog/save-dialog-is-native", ui->EnableNativeSaveDialog->isChecked());

    this->accept();
}
void WelcomeDialog::LanguageComboChanged(int index){
    this->app->removeTranslator(this->translater);

    auto lang = qvariant_cast<MainWindow::LanguageList>(ui->LanguageCombo->itemData(index));
    switch(lang){
    case MainWindow::LanguageList::English:
        break;
    case MainWindow::LanguageList::Japanese:
        this->translater->load(":/i18n/app_ja.qm");
        this->app->installTranslator(this->translater);
        break;
    }
}
void WelcomeDialog::changeEvent(QEvent *event)
{
    if (event->type() == QEvent::LanguageChange) {
        ui->retranslateUi(this);
    }
    QDialog::changeEvent(event);
}

WelcomeDialog::~WelcomeDialog()
{
    delete ui;
}
