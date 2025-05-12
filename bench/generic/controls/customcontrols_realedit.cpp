#include "customcontrols_realedit.h"
#include <QInputDialog>

namespace CustomControls
{

const char RealEdit::m_metricPrefixSymbol[PREFIX_COUNT] = {'G','M','k',' ','m','u','n','p'};
const double RealEdit::m_metricPrefixMultiplier[PREFIX_COUNT] = {1e+12,1e+6,1e+3,1,1e-3,1e-6,1e-9,1e-12};


//MetricPrefix detectGroup(double value)
//{
//    int i=0;
//    while(value>m_metricPrefixMultiplier[PICO])
//    {
//        if((qint32)(value*1000) >0) break;
//        i
//    }
//}

// double toEngFormat(const QString &text)
// {

// }

RealEdit::RealEdit(QWidget *p): QLineEdit(p), UniversalControl()
{    
    m_prefix_i = NONE;
    setFocusPolicy(Qt::WheelFocus);
    m_allowUpdate = false;
    mp_menu = new QMenu();
    mp_action_group = new QActionGroup(this);
    connect(this,SIGNAL(textChanged(QString)),this,SLOT(slot_TextChanged()));
    connect(this,SIGNAL(returnPressed()),this,SLOT(slot_triggered()));
    connect(this,SIGNAL(editingFinished()),this,SLOT(slot_triggered()));
    setPrecision(2);
    m_HasCachedName = false;
    setControlType(REALEDIT);
    m_engFormat = true;
}


RealEdit::~RealEdit()
{
}


void RealEdit::setPrecision(qint32 val)
{
    m_precision = val;
    m_precision_mask = 1.0f;
    while(val--)
    {
        m_precision_mask /= 10.0f;
    }
    setEditValue(editValue());
}


qint32 RealEdit::precision() const
{
    return m_precision;
}

bool RealEdit::engFormat() const
{
    return m_engFormat;
}

void RealEdit::setEngFormat(bool on)
{
    m_engFormat=  on;
}


bool RealEdit::editEnabled() const
{
    return !isReadOnly();
}


void RealEdit::setEditEnabled(bool on)
{
    setReadOnly(!on);
}


void RealEdit::setEditValue(double v)
{
    QString text ;
    if(engFormat())
    {        
        text = toEngFormat(v,m_precision);        
    }
    else
    {
        text = QString::number(v,'f',m_precision);
    }
    setText(text);
    unmark();
}

double RealEdit::editValue()
{
    double result =0.0f;    
    result = toNormalFormat(text());
    return result;
}

int RealEdit::detectPrefix(const QString &text)
{
    int prefix = NONE;
    int i;
    for(i=0;i<PREFIX_COUNT;i++)
    {
        if(text.contains(m_metricPrefixSymbol[i]))
            prefix = i;
    }
    return prefix;
}


double RealEdit::toNormalFormat(const QString &text)
{
    double value=0;
       int prefix = detectPrefix(text);
       m_prefix_i = prefix;
       double mul = m_metricPrefixMultiplier[m_prefix_i];
       char symb = m_metricPrefixSymbol[m_prefix_i];


       if(text.contains(symb))
       {
           if(text.contains('.') && text.endsWith(symb))
           {
               value = QString(text).replace(symb,"").toDouble()*mul;
           }
           else
           {
               QRegExp rx;
               rx.setPattern("([0-9]*\\.)?([0-9]+)?[GMkmunp]{1}([0-9]+)?");
               if(rx.indexIn(text) !=-1 && rx.capturedTexts().count()==4)
               {
                   double l = rx.capturedTexts()[2].toDouble();
                   QString r = rx.capturedTexts()[3];
                   while(r.count()<3) r.append('0');
                   value = l*mul + r.toDouble();
               }
           }
       }
       else {
           value = text.toDouble();
       }


       return value;

}

QString RealEdit::toEngFormat(double v,qint32 precision)
{
    int prefix = NONE;
    bool sign = false;
    if(v<0)
    {
        sign =true;
        v= -v ;
    }
    for(int i=0;i<PREFIX_COUNT;i++)
    {
        if(v>=m_metricPrefixMultiplier[i]) {prefix =i;break;}
    }
    v = sign?-v:v;
    m_prefix_i = prefix;
    QString text;
    text = QString::number(v/m_metricPrefixMultiplier[prefix],'f',precision);
    if(prefix != NONE)
        text+=m_metricPrefixSymbol[prefix];
    return text;

}




const QString RealEdit::name(DragName dragname)
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


bool RealEdit::bind(const char *slot, QObject *pobj)
{
    m_binded = true;
    return connect(this, SIGNAL(signal_activated()), pobj, slot);
}

void RealEdit::unbind()
{
    disconnect(this,SIGNAL(signal_activated()));
}


void RealEdit::setDescription(const QString &description)
{
    this->setToolTip(description);
}


const QString RealEdit::description()
{
    return toolTip();
}


void RealEdit::focusInEvent(QFocusEvent *pevent)
{
    m_allowUpdate = true;
    QLineEdit::focusInEvent(pevent);
}


void RealEdit::focusOutEvent(QFocusEvent *pevent)
{
    m_allowUpdate = false;
    QLineEdit::focusOutEvent(pevent);
}


void RealEdit::contextMenuEvent(QContextMenuEvent *event)
{
    mp_menu->exec(event->globalPos());
}


void RealEdit::mousePressEvent(QMouseEvent *pevent)
{
    QLineEdit::mousePressEvent(pevent);
    if (m_binded && pevent->button() == Qt::LeftButton
            && Qt::ControlModifier == QApplication::keyboardModifiers() )
    {
        startDrag(static_cast<UniversalControl*>(this), DRAG_SETGET);
        stopDrag();
    }
}


void RealEdit::paintEvent(QPaintEvent *pev)
{
    QLineEdit::paintEvent(pev);
    if (m_binded && Qt::ControlModifier == QApplication::keyboardModifiers())
    {
        PAINT_MARKER;
    }
}


void RealEdit::wheelEvent(QWheelEvent *pev)
{
    static qint32 delta = 0;
    if(!isReadOnly() && m_allowUpdate)
    {
        delta += pev->delta();
        if (delta >= 120 || delta <= -120)
        {
            double mul =m_metricPrefixMultiplier[m_prefix_i];
            if (Qt::ShiftModifier == QApplication::keyboardModifiers())
            {
                setEditValue((double)this->editValue()
                             + (1*(delta / 120))*mul);
            }
            else
            {
                double dif = m_precision_mask*(delta / 120);
                double value = (double)this->editValue();
                if(value== mul && (dif<0))
                {
                   mul =m_metricPrefixMultiplier[m_prefix_i+1];
                }
                setEditValue((double)value +dif*mul);
            }
            delta %= 120;
             markChanged();
        }
    }
}


void RealEdit::keyPressEvent(QKeyEvent *pev)
{
    QLineEdit::keyPressEvent(pev);
    if (pev->key() == Qt::Key_Up)
    {
        setEditValue(editValue() +  m_precision_mask);
    }
    else if (pev->key() == Qt::Key_Down)
    {
        setEditValue(editValue() -  m_precision_mask);
    }
    else if(pev->key() == Qt::Key_Escape)
    {
        if(isChanged()) setEditValue(m_prev_value);
    }
}


void RealEdit::markChanged()
{
    m_changed =true;
    setStyleSheet("color:blue;");
}

void RealEdit::markError()
{
}

void RealEdit::unmark()
{
    if(m_changed) {setStyleSheet(""); m_changed =false;}
}

bool RealEdit::isChanged() const
{
    return m_changed;
}

void RealEdit::construct()
{
    int i=0;
    while(i<PREFIX_COUNT)
    {
        if(m_metricPrefixSymbol[i] != ' ' && text().contains(m_metricPrefixSymbol[i]))
        {
            setEngFormat(1);
            break;
        }
        i++;
    }
    if(!engFormat())
    setPrecision(text().count() - text().indexOf('.')-1);
}

void RealEdit::slot_CustomPrecision()
{
    setPrecision(QInputDialog::getInt(0,"","Precision",m_precision));    
}

void RealEdit::slot_TextChanged()
{    
    //saving value
    if(!isChanged()) m_prev_value = editValue();
    markChanged();
}

void RealEdit::slot_triggered()
{    
    if(isChanged())
    {
        emit signal_activated();
        if(!engFormat())
            setPrecision(text().count() - text().indexOf('.')-1);
        unmark();
    }
}
}
