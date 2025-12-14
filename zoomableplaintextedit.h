#ifndef ZOOMABLEPLAINTEXTEDIT_H
#define ZOOMABLEPLAINTEXTEDIT_H

#include <QPlainTextEdit>
#include <QEvent>
#include <QSettings>

class ZoomablePlainTextEdit : public QPlainTextEdit
{
    Q_OBJECT
public:
    using QPlainTextEdit::QPlainTextEdit;
    explicit ZoomablePlainTextEdit(QWidget *parent = nullptr);
    void ZoomIn();
    void ZoomOut();
    void UnZoom();
signals:
    void OnZoom(int pointFontSize);
private:
    void Zoom(bool inOrOut);
    QSettings settings;

protected:
    void wheelEvent(QWheelEvent *event) override;
    void showEvent(QShowEvent *event) override;
};

#endif // ZOOMABLEPLAINTEXTEDIT_H
