#include "customcontrols_slider.h"

namespace CustomControls
{

  Slider::Slider(QWidget *p): QSlider(p), UniversalControl()
  {
    setMouseTracking(true);
    m_HasCachedName = false;
    setControlType(SLIDER);
    m_value = 0;
  }

  Slider::~Slider()
  {
  }

  bool Slider::editEnabled() const
  {
    return true;
  }

  void Slider::setEditEnabled(bool)
  {

  }

  void Slider::setEditValue(double v)
  {
    /* this protection from cycling .
         * Because signal is valueChanged */
    qint32 iv = (qint32) v;
    if(m_value != iv)
      {
        setValue( iv );
      }
    m_value = iv;
  }

  double Slider::editValue()
  {
    return (double)value();
  }

  const QString Slider::name(DragName dragname)
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

  bool Slider::bind(const char *slot, QObject *pobj)
  {
    m_binded = true;
    return connect(this, SIGNAL(sliderMoved(int)), pobj, slot);
  }

  void Slider::unbind()
  {
      disconnect(this,SIGNAL(sliderMoved(int)));
  }

  void Slider::setDescription(const QString &description)
  {
    this->setToolTip(description);
  }

  const QString Slider::description()
  {
    return toolTip();
  }

  void Slider::mousePressEvent(QMouseEvent *pevent)
  {
    QSlider::mousePressEvent(pevent);
    if (pevent->button() == Qt::LeftButton
        && Qt::ControlModifier == QApplication::keyboardModifiers() )
      {
        startDrag(static_cast<UniversalControl*>(this), DRAG_SETGET);
        stopDrag();
      }
  }

  void Slider::leaveEvent(QEvent *ev)
  {
    QSlider::leaveEvent(ev);
    repaint();
  }

  void Slider::mouseMoveEvent(QMouseEvent *ev)
  {
    QSlider::mouseMoveEvent(ev);
    repaint();
  }

  void Slider::paintEvent(QPaintEvent *pev)
  {
    QSlider::paintEvent(pev);
    if (Qt::ControlModifier == QApplication::keyboardModifiers())
      {
        PAINT_MARKER;
      }
  }

}
