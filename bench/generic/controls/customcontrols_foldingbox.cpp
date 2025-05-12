#include "customcontrols_foldingbox.h"
#include <QDebug>
#include <QInputDialog>
#include "customcontrols_checkbox.h"
#include <applet.h>
#include "mainapp.h"

#define ENROLLED_HEIGHT (20)
namespace CustomControls
{

FoldingBox::FoldingBox(QWidget *parent) :
    QGroupBox(parent)
{

    m_former_size = -1;
    setMouseTracking(true);
    this->m_drag_drop_run = false;
}

void FoldingBox::setExpanded(bool on)
{
    if(this->isCheckable()){
    if( m_former_size == -1)
    {
        m_former_size = this->height();
    }
    setChecked(on);
    if(on)
    {
        setFixedHeight(m_former_size);
    }
    else
    {
        setFixedHeight(ENROLLED_HEIGHT);
    }
    emit signal_resized();
    }
}

int FoldingBox::formerHeight() const
{
    return m_former_size;
}


void FoldingBox::mouseReleaseEvent(QMouseEvent *pevent)
{    
    QGroupBox::mouseReleaseEvent(pevent);
    setExpanded(isChecked());
}



void FoldingBox::mousePressEvent(QMouseEvent *pevent)
{
    QGroupBox::mousePressEvent(pevent);
    if (this->m_drag_drop_run && pevent->button() == Qt::LeftButton
            && Qt::ControlModifier == QApplication::keyboardModifiers() )
    {
        emit signal_dragdrop();
    }

}

int FoldingBox::enrolledHeight()
{
    return ENROLLED_HEIGHT;
}

void FoldingBox::enableDragDrop(bool yes)
{
    m_drag_drop_run = yes;
}

void FoldingBox::mouseDoubleClickEvent(QMouseEvent *)
{
/*    QInputDialog d;
    d.setTextValue(this->title());
    if(d.exec() == QDialog::Accepted)
        this->setTitle(d.textValue());*/
}

void FoldingBox::leaveEvent(QEvent *)
{
    repaint();
}

void FoldingBox::mouseMoveEvent(QMouseEvent *)
{
    repaint();
}

void FoldingBox::paintEvent(QPaintEvent *pev)
{
    QGroupBox::paintEvent(pev);
    if (this->m_drag_drop_run && Qt::ControlModifier == QApplication::keyboardModifiers())
    {
        PAINT_MARKER;
    }
}

}
