#include "customcontrols_buttonbox.h"


namespace CustomControls
{

ButtonBox::ButtonBox(QWidget *p): QGroupBox(p),UniversalControl()
{
    setControlType(BUTTONBOX);
    m_HasCachedName = false;
    m_editable = true;
    setMouseTracking(true);
    m_constructed = false;
}

ButtonBox::~ButtonBox()
{
}

void ButtonBox::construct(ButtonBox::Orientation orientation,Type type, const QStringList &list)
{     
    if(list.size() && !m_constructed)
    {
    m_constructed =true;
    if(orientation == VERTICAL)
    {
        mp_layout = new QVBoxLayout();
    }
    else
    {
        mp_layout = new QHBoxLayout();
    }
    for(qint32 i=0;i<list.size();i++)
    {
        QAbstractButton *pbutton = NULL;
        if(type == BUTTON)
        {
            pbutton = new QPushButton(list.at(i));
        }
        else if(type == RADIO)
        {
            pbutton = new QRadioButton(list.at(i));
        }
        if(pbutton)
        {
            pbutton->setAutoExclusive(true);
            pbutton->setCheckable(true);
            m_buttons.append(pbutton);
            mp_layout->addWidget(pbutton);
            connect(pbutton,SIGNAL(clicked()),this,SLOT(slot_on_click()));
        }
    }        
    setLayout(mp_layout);
    }
}

bool ButtonBox::editEnabled() const
{
    return m_editable;
}

void ButtonBox::setEditEnabled(bool on)
{
    m_editable = on;
    for(int i=0;i<m_buttons.size();i++){
        m_buttons.at(i)->setEnabled(on);
    }
}

void ButtonBox::setEditValue(double v)
{    
    for(int i=0;i<m_buttons.count(); i++)
    {
        if((qint32)v==i)
        {
            m_buttons.at(i)->setChecked(true);
            break;
        }
        else m_buttons.at(i)->setChecked(false);
    }
}

double ButtonBox::editValue()
{
    quint32 count = m_buttons.count();
    while(count--)
    {
       if(m_buttons.at(count)->isChecked())
       {
           return count;
       }
    }
    return 0;
}

const QString ButtonBox::name(DragName dragname)
{
    if ( !m_HasCachedName )
      {
        QString fullpath;
        if(accessibleName().isEmpty())
          fullpath = generateRegisterName(objectName());
        else fullpath = accessibleName();
        extractParameter(fullpath,0,&m_CachedName,&m_CachedParameter);
        m_bindName = fullpath;
        m_HasCachedName =  true;
      }
    switch(dragname)
    {
      case PARAMETER:  return m_CachedParameter;
      case NODENAME: return m_CachedName;
      case FULLPATH: return m_bindName;
      default : break;
    }
    return m_CachedName;
}

bool ButtonBox::bind(const char *slot, QObject *pobj)
{
    m_binded = true;    
    connect(this,SIGNAL(signal_clicked()), pobj, slot);
    return true;
}

void ButtonBox::unbind()
{    
    if(m_binded)
        disconnect(this,SIGNAL(signal_clicked()));
}

void ButtonBox::setDescription(const QString &description)
{
    this->setToolTip(description);
}

const QString ButtonBox::description()
{
    return toolTip();
}

void ButtonBox::setCaption(const QString &caption)
{
    setTitle(caption);
}

const QString ButtonBox::caption()
{
    return title();
}

void ButtonBox::makeSetup(const QString &init_script)
{
    QStringList l1 = QString(init_script).trimmed().split('@');
    foreach(const QString &item, l1)
    {
        QList<QString> l2 = item.trimmed().split('=');
        if(l2.size() == 2)
        {
            if(l2.at(0) == "readonly")
            {


            }
        }
    }
}

void ButtonBox::leaveEvent(QEvent *)
{
    repaint();
}

void ButtonBox::mouseMoveEvent(QMouseEvent *)
{
    repaint();
}

void ButtonBox::mousePressEvent(QMouseEvent *pevent)
{
    if(editEnabled())
    {
        QGroupBox::mousePressEvent(pevent);
    }
    if (pevent->button() == Qt::LeftButton
            && Qt::ControlModifier == QApplication::keyboardModifiers() )
    {
        startDrag(static_cast<UniversalControl*>(this), DRAG_SETGET);
        stopDrag();
    }
}

void ButtonBox::paintEvent(QPaintEvent *pev)
{
    QGroupBox::paintEvent(pev);
    if (Qt::ControlModifier == QApplication::keyboardModifiers())
    {
        PAINT_MARKER;
    }
}

void ButtonBox::slot_on_click()
{
    emit signal_clicked();
}














}
