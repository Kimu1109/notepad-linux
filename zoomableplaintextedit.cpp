#include "zoomableplaintextedit.h"

#include <QFont>

ZoomablePlainTextEdit::ZoomablePlainTextEdit(QWidget *parent)
    : QPlainTextEdit(parent)
{
    QFont f = QFontDatabase::systemFont(QFontDatabase::FixedFont);
    if(this->settings.contains("editor/font-family")){
        f = QFont(
            this->settings.value("editor/font-family").toString(),
            -1,
            this->settings.value("editor/font-weight").toInt()
        );
    }else{
        this->settings.setValue("editor/font-family", f.family());
        this->settings.setValue("editor/font-weight", -1);
    }

    f.setPointSize(this->settings.value("editor/point-font-size", 14).toInt());
    setFont(f);
}
void ZoomablePlainTextEdit::showEvent(QShowEvent *event)
{
    QPlainTextEdit::showEvent(event);

    if(this->settings.value("editor/wrap-mode", QPlainTextEdit::NoWrap).toInt() == (int)QPlainTextEdit::NoWrap){
        setLineWrapMode(QPlainTextEdit::NoWrap);
        setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
    }else{
        setLineWrapMode(QPlainTextEdit::WidgetWidth);
        setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    }
}
void ZoomablePlainTextEdit::UnZoom(){
    QFont f = this->font();
    f.setPointSize(14);
    this->setFont(f);
    this->settings.setValue("editor/point-font-size", 14);

    emit this->OnZoom(14);
}
void ZoomablePlainTextEdit::ZoomIn(){
    this->Zoom(true);
}
void ZoomablePlainTextEdit::ZoomOut(){
    this->Zoom(false);
}
void ZoomablePlainTextEdit::Zoom(bool inOrOut){
    QFont f = this->font();
    if(inOrOut){
        f.setPointSize(f.pointSize() + 1);
    }else{
        f.setPointSize(std::max(1, f.pointSize() - 1));
    }
    this->settings.setValue("editor/point-font-size", f.pointSize());
    emit this->OnZoom(f.pointSize());
    this->setFont(f);
}
void ZoomablePlainTextEdit::wheelEvent(QWheelEvent *event)
{
    if (event->modifiers() & Qt::ControlModifier) {
        this->Zoom(event->angleDelta().y() > 0);
        event->accept();
        return;
    }

    QPlainTextEdit::wheelEvent(event);
}
