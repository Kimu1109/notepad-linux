#ifndef SAVEDIALOGNOTNATIVE_H
#define SAVEDIALOGNOTNATIVE_H

#include <QWidget>
#include <QFileDialog>
#include <QComboBox>
#include <QLabel>
#include "saveformatdialog.h"
#include <QPlainTextEdit>
#include <QGridLayout>

struct IOSaveDialog{
    QString path;
    QString title;
    SaveFormatDialog::Codecs codec;
    SaveFormatDialog::LineFeedCode lineFeedCode;
    bool successed;
};

IOSaveDialog saveDialogNotNative(QWidget *parent, QPlainTextEdit* editor, IOSaveDialog input)
{
    QFileDialog dialog(parent, "Save As", input.path.isEmpty() ? QDir::homePath() : QDir(input.path).filePath(input.title));
    dialog.setAcceptMode(QFileDialog::AcceptSave);
    dialog.setFileMode(QFileDialog::AnyFile);

    dialog.setOption(QFileDialog::DontUseNativeDialog, true);

    // ---------- エンコード選択 UI ----------
    QComboBox *codecCombo = new QComboBox(&dialog);
    for(const auto& [num, str] : SaveFormatDialog::getCodecs()){
        codecCombo->addItem(str, num);
    }
    QComboBox *lineFeedCodeCombo = new QComboBox(&dialog);
    for(const auto& [num, str] : SaveFormatDialog::getLineFeedCodes()){
        lineFeedCodeCombo->addItem(str, num);
    }
    codecCombo->setCurrentIndex((int)input.codec);
    lineFeedCodeCombo->setCurrentIndex((int)input.lineFeedCode);

    QLabel *labelCodec = new QLabel("Codec:", &dialog);
    QLabel *labelLineFeedCode = new QLabel("Line Feed Code:", &dialog);

    // ダイアログ内部レイアウトを取得
    QGridLayout *layout = qobject_cast<QGridLayout*>(dialog.layout());
    if (layout) {
        // filename の下あたりに追加する
        layout->addWidget(labelCodec, layout->rowCount(), 0);
        layout->addWidget(codecCombo, layout->rowCount() - 1, 1);

        layout->addWidget(labelLineFeedCode, layout->rowCount(), 0);
        layout->addWidget(lineFeedCodeCombo, layout->rowCount() - 1, 1);
    }

    bool result = false;

    IOSaveDialog info{};

    // ---------- 表示 ----------
    if (dialog.exec() == QDialog::Accepted) {
        QString filePath = dialog.selectedFiles().first();
        SaveFormatDialog::Codecs encoding = qvariant_cast<SaveFormatDialog::Codecs>(codecCombo->itemData(codecCombo->currentIndex()));
        SaveFormatDialog::LineFeedCode lineCode = qvariant_cast<SaveFormatDialog::LineFeedCode>(lineFeedCodeCombo->itemData(lineFeedCodeCombo->currentIndex()));

        QFileInfo fileInfo(filePath);
        QDir dir(fileInfo.path());

        if(filePath != "" && dir.exists()){
            if(SaveFormatDialog::WriteTo(filePath, encoding, lineCode, editor)){
                info.path = fileInfo.absolutePath();
                info.title = fileInfo.fileName();
                info.codec = encoding;
                info.lineFeedCode = lineCode;
                info.successed = true;
            }
        }
    }

    delete codecCombo;
    delete lineFeedCodeCombo;
    delete labelCodec;
    delete labelLineFeedCode;

    return info;
}

#endif // SAVEDIALOGNOTNATIVE_H
