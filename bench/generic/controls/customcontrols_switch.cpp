#include "customcontrols_switch.h"
#include "parameterparser.h"

namespace CustomControls
{



Switch::Switch(QWidget *p): MySwitch(p), UniversalControl()
{
    this->setMouseTracking(true);
    m_HasCachedName = false;
    setControlType(SWITCH);
}

Switch::~Switch()
{
}

bool Switch::editEnabled() const
{
    return readOnly();
}

void Switch::setEditEnabled(bool on)
{
    setReadOnly(!on);
}

void Switch::setEditValue(double v)
{
    setCurrentState((qint32)v);
}

double Switch::editValue()
{
    return (double)state();
}

const QString Switch::name(DragName dragname)
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

bool Switch::bind(const char *slot, QObject *pobj)
{
    m_binded = true;
    return connect(this, SIGNAL(signal_clicked()), pobj, slot);
}

void Switch::unbind()
{
    disconnect(this,SIGNAL(signal_clicked()));
}

void Switch::setDescription(const QString &description)
{
    this->setToolTip(description);
}

const QString Switch::description()
{
    return toolTip();
}

void Switch::construct()
{
    //    SWITCH_ON_OFF,
    //    SWITCH_1_2,
    //    SWITCH_1_3,
    //    SWITCH_1_4,
    //    SWITCH_1_5,
    //    SIGNAL_ON_OFF,
    //    SIGNAL_DIRECTION,
    //    SIGNAL_0_1,
    //    SIGNAL_0_A,
    //    LED_RED,
    //    LED_GREEN,
    //    LED_RED_GREEN
    double angle=mrAngle;
    bool mirroredX=m_mirrored_by_x;
    bool mirroredY=m_mirrored_by_y;
    QString type;

    if(m_already_constructed) return;

    if(!text().isEmpty())
    {
        if(text().startsWith("["))
        {
                angle = text().section(",",1).toDouble();
                mirroredX = text().section(",",2).toInt();
                type = text().section(",",0,0);
        }
        else
        {
            ParameterParser p(text(),",");
            type = p["type"].toString();
            angle = p["angle"].toDouble();
            mirroredX = p["mirrorX"].toBool();
            mirroredY = p["mirrorY"].toBool();
        }


    }

    if(type=="[/--]" )
    {
        setSwitchModel(SWITCH_ON_OFF);
    }
    else if(type=="[/-2]")
    {
        setSwitchModel(SWITCH_1_2);
    }
    else if(type=="[/-3]" )
    {
        setSwitchModel(SWITCH_1_3);
    }
    else if(type=="[/-4]")
    {
        setSwitchModel(SWITCH_1_4);
    }
    else if(type=="[x]")
    {
        setSwitchModel(SIGNAL_ON_OFF);
    }
    else if(type=="[<>]")
    {
        setSwitchModel(SIGNAL_DIRECTION);
    }
    else if(type=="[RED]")
    {
        setSwitchModel(LED_RED);
    }
    else if(type=="[GREEN]")
    {
        setSwitchModel(LED_GREEN);
    }
    else if(type=="[RED-GREEN]")
    {
        setSwitchModel(LED_GREEN);
    }
    else if(type=="[*]")
    {
        setSwitchModel(SIGNAL_0_1);
    }
    else if(type=="[R]")
    {
        setSwitchModel(SIGNAL_0_A);
    }
    setAngle(angle);
    setMirroredX(mirroredX);
    setMirroredY(mirroredY);
}





void Switch::mousePressEvent(QMouseEvent *pevent)
{
    MySwitch::mousePressEvent(pevent);
    if (pevent->button() == Qt::LeftButton
            && Qt::ControlModifier == QApplication::keyboardModifiers() )
    {
        startDrag(static_cast<UniversalControl*>(this), DRAG_SETGET);
        stopDrag();
    }
}

void Switch::leaveEvent(QEvent *)
{
    repaint();
}

void Switch::mouseMoveEvent(QMouseEvent *)
{
    repaint();
}

void Switch::paintEvent(QPaintEvent *pev)
{
    MySwitch::paintEvent(pev);
    if (Qt::ControlModifier == QApplication::keyboardModifiers())
    {
        PAINT_MARKER;
    }
}


}
