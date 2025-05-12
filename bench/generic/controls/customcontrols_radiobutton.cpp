#include "customcontrols_radiobutton.h"

namespace CustomControls
{

  RadioButton::RadioButton(QWidget *p): QRadioButton(p), UniversalControl()
  {
    setControlType(RADIOBUTTON);
    m_HasCachedName = false;
  }

  RadioButton::~RadioButton()
  {
  }

  bool RadioButton::editEnabled() const
  {
    return isCheckable();
  }

  void RadioButton::setEditEnabled(bool on)
  {
    setCheckable(on);
  }

  void RadioButton::setEditValue(double v)
  {
    setChecked((bool)v);
  }

  double RadioButton::editValue()
  {
    return (double)isChecked();
  }

  const QString RadioButton::name(DragName dragname)
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

  bool RadioButton::bind(const char *slot, QObject *pobj)
  {
    m_binded = true;
    return connect(this, SIGNAL(clicked()), pobj, slot);
  }

  void RadioButton::unbind()
  {
      disconnect(this,SIGNAL(clicked()));
  }

  void RadioButton::setDescription(const QString &description)
  {
    this->setToolTip(description);
  }

  const QString RadioButton::description()
  {
      return toolTip();
  }

  void RadioButton::setCaption(const QString &caption)
  {
      this->setText(caption);
  }

  const QString RadioButton::caption()
  {
      return this->text();
  }

  void RadioButton::leaveEvent(QEvent *)
  {
    repaint();
  }

  void RadioButton::mouseMoveEvent(QMouseEvent *)
  {
    repaint();
  }

  void RadioButton::mousePressEvent(QMouseEvent *pevent)
  {
    QRadioButton::mousePressEvent(pevent);
    if (pevent->button() == Qt::LeftButton
        && Qt::ControlModifier == QApplication::keyboardModifiers() )
      {
        startDrag(static_cast<UniversalControl*>(this), DRAG_SETGET);
        stopDrag();
      }
  }

  void RadioButton::paintEvent(QPaintEvent *pev)
  {
    QRadioButton::paintEvent(pev);
    if (Qt::ControlModifier == QApplication::keyboardModifiers())
      {
        PAINT_MARKER;
      }
  }














}
