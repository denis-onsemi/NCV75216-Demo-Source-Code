#include "customcontrols_pushbutton.h"
#include "applet.h"
namespace CustomControls
{

PushButton::PushButton(QWidget *p): QPushButton(p), UniversalControl()
{
    m_HasCachedName = false;
    setControlType(PUSHBUTTON);
    setMouseTracking(true);

//    /* find applet name*/
//    bool found = false;
//    QWidget *pwidget =   this;
//    for(int i=0;i<10;i++){
//        if(!pwidget) break;
//        qDebug()<<i<<pwidget->objectName();
//        if (pwidget->inherits("Applet"))
//        {
//            found = true;
//            m_appletName = QString(pwidget->metaObject()->className());
//            break;
//        }
//        else pwidget = pwidget->parentWidget();
//    }
//    if(!found )
//        qWarning()<<QString("Control:%1 parent applet not found").arg(this->text());
}

PushButton::~PushButton()
{

}

bool PushButton::editEnabled() const
{
    return isCheckable();
}

void PushButton::setEditEnabled(bool on)
{
    setCheckable(on);
}

void PushButton::setEditValue(double)
{

}

double PushButton::editValue()
{
    return 0.0f;
}

const QString PushButton::name(DragName dragname)
{
    if (!m_HasCachedName)
    {
        if(accessibleName().isEmpty())
        {
            QStringList l = objectName().mid(2,objectName().size()).split('_');
            if(l.size() >0 )
            {
                m_appletName = l.at(0);
            }
            if(l.size() >1)
            {
                m_CachedName = l.at(1);
            }
            if(l.size() >2)
            {
                m_CachedParameter = l.at(2);
            }
        }
        else{
            extractParameter(accessibleName(), &m_appletName,&m_CachedName,&m_CachedParameter);
        }
         m_bindName = m_CachedName;
        m_HasCachedName =  true;
    }
    switch(dragname)
    {
    case PARAMETER:  return m_CachedParameter;
    case NODENAME: return m_CachedName;
    case FULLPATH: return m_bindName;
    default : break;
    }
    return m_bindName;
}

bool PushButton::bind(const char *slot, QObject *pobj)
{
    m_binded = true;
    return connect(this, SIGNAL(clicked()), pobj, slot);
}

void PushButton::unbind()
{
    disconnect(this, SIGNAL(clicked()));
}

void PushButton::setDescription(const QString &description)
{
    this->setToolTip(description);
}

const QString PushButton::description()
{
    return toolTip();
}

void PushButton::leaveEvent(QEvent *)
{
    repaint();
}

void PushButton::mouseMoveEvent(QMouseEvent *)
{
    repaint();
}

void PushButton::mousePressEvent(QMouseEvent *pevent)
{
    QPushButton::mousePressEvent(pevent);
    if (pevent->button() == Qt::LeftButton
            && Qt::ControlModifier == QApplication::keyboardModifiers() )
    {
        startDrag(static_cast<UniversalControl*>(this), DRAG_RUN);
        stopDrag();        
    }
}

void PushButton::paintEvent(QPaintEvent *pev)
{
    QPushButton::paintEvent(pev);
    if (Qt::ControlModifier == QApplication::keyboardModifiers())
    {
        PAINT_MARKER;
    }
}

}
