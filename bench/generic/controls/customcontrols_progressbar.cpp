#include "customcontrols_progressbar.h"

namespace CustomControls
{

  ProgressBar::ProgressBar(QWidget *p): QProgressBar(p), UniversalControl()
  {
    m_HasCachedName = false;
    setControlType(SPINBOX);
    setMouseTracking(true);
  }

  ProgressBar::~ProgressBar()
  {
  }

  bool ProgressBar::editEnabled() const
  {
    return false;
  }

  void ProgressBar::setEditEnabled(bool)
  {

  }

  void ProgressBar::setEditValue(double v)
  {
    setValue((int)v);
  }

  double ProgressBar::editValue()
  {
    return (double)value();
  }

  const QString ProgressBar::name(DragName dragname)
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

  bool ProgressBar::bind(const char *, QObject *)
  {
    m_binded = true;
    return false;
  }

  void ProgressBar::leaveEvent(QEvent *)
  {
      repaint();
  }

  void ProgressBar::mouseMoveEvent(QMouseEvent *)
  {
      repaint();
  }

  void ProgressBar::unbind()
  {
  }

  void ProgressBar::setDescription(const QString &description)
  {
    this->setToolTip(description);
  }

  const QString ProgressBar::description()
  {
      return toolTip();
  }

  void ProgressBar::setEditValueRange(double min,double max)
  {
      int value = (int)min;
      if(value<0) value =0;
      setMinimum(value);
      value = (int)max;
      if(value<1 ||  value > 60000 ) value = 60000;
      setMaximum(value);
  }


  void ProgressBar::mousePressEvent(QMouseEvent *pevent)
  {
    QProgressBar::mousePressEvent(pevent);
    if (pevent->button() == Qt::LeftButton
        && Qt::ControlModifier == QApplication::keyboardModifiers() )
      {
        startDrag(static_cast<UniversalControl*>(this), DRAG_GET);
        stopDrag();
      }
  }

  void ProgressBar::paintEvent(QPaintEvent *pev)
  {
    QProgressBar::paintEvent(pev);
    if (Qt::ControlModifier == QApplication::keyboardModifiers())
      {
        PAINT_MARKER;
      }
  }

}
