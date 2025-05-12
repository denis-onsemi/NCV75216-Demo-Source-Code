#include "customcontrols_spinbox.h"

namespace CustomControls
{

  SpinBox::SpinBox(QWidget *p): QSpinBox(p), UniversalControl()
  {
    setControlType(SPINBOX);
    setMouseTracking(true);
    setMaximum(65535);
    m_HasCachedName = false;
  }

  SpinBox::~SpinBox()
  {
  }

  bool SpinBox::editEnabled() const
  {
    return !isReadOnly();
  }

  void SpinBox::setEditEnabled(bool on)
  {
    setReadOnly(!on);
  }

  void SpinBox::setEditValue(double v)
  {
    setValue((int)v);
  }

  double SpinBox::editValue()
  {
    return (double)value();
  }

  const QString SpinBox::name(DragName dragname)
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

  bool SpinBox::bind(const char *slot, QObject *pobj)
  {
    m_binded = true;
    return connect(this, SIGNAL(editingFinished()), pobj, slot);
  }

  void SpinBox::unbind()
  {
      disconnect(this,SIGNAL(editingFinished()));
  }

  void SpinBox::setDescription(const QString &description)
  {
    this->setToolTip(description);
  }

  const QString SpinBox::description()
  {
    return toolTip();
  }

  void SpinBox::leaveEvent(QEvent *)
  {
    repaint();
  }

  void SpinBox::mouseMoveEvent(QMouseEvent *)
  {
    repaint();
  }

  void SpinBox::mousePressEvent(QMouseEvent *pevent)
  {
    QSpinBox::mousePressEvent(pevent);
    if (pevent->button() == Qt::LeftButton
        && Qt::ControlModifier == QApplication::keyboardModifiers() )
      {
        startDrag(static_cast<UniversalControl*>(this), DRAG_SETGET);
        stopDrag();
      }
  }

  void SpinBox::paintEvent(QPaintEvent *pev)
  {
    QSpinBox::paintEvent(pev);
    if (Qt::ControlModifier == QApplication::keyboardModifiers())
      {
        PAINT_MARKER;
      }
  }

}
