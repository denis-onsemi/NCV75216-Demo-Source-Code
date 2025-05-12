#include "customcontrols_dial.h"

namespace CustomControls
{

  Dial::Dial(QWidget *p): QDial(p), UniversalControl()
  {
    setMouseTracking(true);
    setNotchesVisible(true);
    m_HasCachedName = false;
    setControlType(DIAL);
    m_value = 0;
  }

  Dial::~Dial()
  {
  }

  bool Dial::editEnabled() const
  {
    return true;
  }

  void Dial::setEditEnabled(bool)
  {

  }

  void Dial::setEditValue(double v)
  {
    /* this protection from cycling .
         * Because signal is valueChanged */
    int iv = (qint32)v;
    if(m_value != iv)
      {
        setValue(iv);
      }
    m_value = iv;
  }

  double Dial::editValue()
  {
    return (double)value();
  }

  const QString Dial::name(DragName dragname)
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

  bool Dial::bind(const char *slot, QObject *pobj)
  {
    m_binded = true;
    return connect(this, SIGNAL(valueChanged(int)), pobj, slot);
  }

  void Dial::unbind()
  {
      disconnect(this, SIGNAL(valueChanged(int)));
  }

  void Dial::setDescription(const QString &description)
  {
    this->setToolTip(description);
  }

  const QString Dial::description()
  {
      return toolTip();
  }

  void Dial::setEditValueRange(double min,double max)
  {
      int value = (int)min;
      if(value<0) value =0;
      setMinimum(value);
      value = (int)max;
      if(value<1 || value>60000) value = 60000;
      setMaximum(value);
  }


  void Dial::mousePressEvent(QMouseEvent *pevent)
  {
    QDial::mousePressEvent(pevent);
    if (pevent->button() == Qt::LeftButton
        && Qt::ControlModifier == QApplication::keyboardModifiers() )
      {
        startDrag(static_cast<UniversalControl*>(this), DRAG_SETGET);
        stopDrag();
      }
  }

  void Dial::leaveEvent(QEvent *ev)
  {
    QDial::leaveEvent(ev);
    repaint();
  }

  void Dial::mouseMoveEvent(QMouseEvent *ev)
  {
    QDial::mouseMoveEvent(ev);
    repaint();
  }

  void Dial::paintEvent(QPaintEvent *pev)
  {
    QDial::paintEvent(pev);
    if (Qt::ControlModifier == QApplication::keyboardModifiers())
      {
        PAINT_MARKER;
      }
  }


}
