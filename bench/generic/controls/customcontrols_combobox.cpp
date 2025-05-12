#include "customcontrols_combobox.h"
#include "translator.h"
#include <QInputDialog>

namespace CustomControls
{

const char ComboBox::m_metricPrefixSymbol[PREFIX_COUNT] = {'G','M','k',' ','m','u','n','p'};
const double ComboBox::m_metricPrefixMultiplier[PREFIX_COUNT] = {1e+12,1e+6,1e+3,1,1e-3,1e-6,1e-9,1e-12};

ComboBox::ComboBox(QWidget *p): QComboBox(p), UniversalControl()
{
    setMouseTracking(true);
    setControlType(COMBOBOX);
    m_HasCachedName = false;
    m_read_only = false;
     m_engFormat = true;
     m_dictionary_set = false;
    connect(this,SIGNAL(editTextChanged(QString)),this,SLOT(slot_TextChanged()));
    connect(this,SIGNAL(activated(int)),this,SLOT(slot_activated()));
}

ComboBox::~ComboBox()
{
}

bool ComboBox::editEnabled() const
{
    return m_read_only;
}

void ComboBox::setEditEnabled(bool on)
{
    //setEnabled(on);
    m_read_only =!on;
}

bool ComboBox::engFormat() const
{
    return m_engFormat;
}

void ComboBox::setEngFormat(bool on)
{
    m_engFormat=  on;
}

void ComboBox::setEditValue(double v)
{
    QString text;
    if(useDictionary())
    {
        if(! setCurrentItem(v) )
        {
            if(engFormat())
            {
                text = toEngFormat(v);
            }
            else
            {
                text = QString::number(v);
            }
            setEditText(text);
        }
        else{
            if(isEditable())
            {
                if(engFormat())
                {
                    text = toEngFormat(v);
                }
                else
                {
                    text = QString::number(v);
                }
                setEditText(text);
            }
        }
    }
    else
    {
        setCurrentIndex((int)v);
    }
    unmark();
}

double ComboBox::editValue()
{
    double result =0.0f;

    if(useDictionary())
    {
        int i=currentItem();
        if(i<0)
        {
            if(isEditable())
            {
                if( engFormat() )
                {
                    result = toNormalFormat(currentText());
                }
                else
                {
                    result = currentText().toDouble();
                }
            }
            else result = itemData(currentIndex()).toDouble();
        }
        else result = itemData(i).toDouble();
    }
    else{
        result = (double)currentIndex();
    }
    unmark();
    return result;
}

double ComboBox::toNormalFormat(const QString &text)
{
    double value=0;
    int prefix = detectPrefix(text);
    m_prefix_i = prefix;
    QString tmp =text;
    double mul = m_metricPrefixMultiplier[m_prefix_i];
    char symbol = m_metricPrefixSymbol[m_prefix_i];
    tmp.replace(symbol, "");
    value = tmp.toDouble()*mul;
    return value;
}

QString ComboBox::toEngFormat(double v)
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
    return QString::number(v/m_metricPrefixMultiplier[prefix])
            +  m_metricPrefixSymbol[prefix] ;
}

int ComboBox::detectPrefix(const QString &text)
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

const QString ComboBox::name(DragName dragname)
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

void  ComboBox::setArray(const QByteArray &array)
{
    this->setEditText(array);
}

QByteArray ComboBox::array()
{
    return currentText().toLatin1();
}

bool ComboBox::bind(const char *slot, QObject *pobj)
{
    m_binded = true;
    return connect(this, SIGNAL(signal_activated()), pobj, slot);
}

void ComboBox::unbind()
{
    disconnect(this, SIGNAL(signal_activated()));
}

void ComboBox::setDescription(const QString &description)
{
    this->setToolTip(description);
}

const QString ComboBox::description()
{
    return toolTip();
}

bool ComboBox::setCurrentItem(const QString &itemtext)
{
    for(int i=0;i<count();i++){
        if(itemtext == this->itemText(i))
        {
            this->setCurrentIndex(i);
            return true;
        }
    }
    return false;
}

bool ComboBox::setCurrentItem(const QByteArray &itemdata)
{
    for(int i=0;i<count();i++){
        if(itemdata == this->itemData(i))
        {
            this->setCurrentIndex(i);
            return true;
        }
    }
    return false;
}

bool ComboBox::setCurrentItem(const double &v)
{
    for(int i=0;i<count();i++)
    {
        if(v == this->itemData(i).toDouble())
        {
            this->setCurrentIndex(i);
            return true;
        }
    }
    return false;
}

void ComboBox::setDictionary(const QString &dict_script)
{
    this->clear();
    int i=0;
    foreach(const QString &item, dict_script.split(';'))
    {
        QStringList l2 = item.split("::");
        if (l2.size() == 2 )
        {
            QString caption = l2.at(0).mid(1,l2.at(0).count());
            QVariant value;
            char type = l2.at(1)[0].toAscii();
            switch(type)
            {
            case 's':
                value= l2.at(1).mid(1,l2.at(1).count());
                break;
            case 'i':
                value= l2.at(1).mid(1,l2.at(1).count()).toInt();
                break;
            case 'f':
                value= l2.at(1).mid(1,l2.at(1).count()).toDouble();
                break;
            }
            addItem(caption,value);
        }
        else{
            addItem(item,i++);
        }
    }
    m_dictionary_set = true;
}


bool ComboBox::useDictionary() const
{
    return m_dictionary_set;
}


void ComboBox::leaveEvent(QEvent *)
{
    repaint();
}

void ComboBox::mouseMoveEvent(QMouseEvent *)
{
    repaint();
}

void ComboBox::mousePressEvent(QMouseEvent *pevent)
{
    if(m_read_only) return;
    QComboBox::mousePressEvent(pevent);
    if (m_binded && pevent->button() == Qt::LeftButton
            && Qt::ControlModifier == QApplication::keyboardModifiers() )
    {
        startDrag(static_cast<UniversalControl*>(this), DRAG_SETGET);
        stopDrag();
    }
}

void ComboBox::keyPressEvent(QKeyEvent *pev)
{
    if( pev->key()== Qt::Key_Return)
    {
        emit signal_activated();
    }
    else
        QComboBox::keyPressEvent(pev);
}


void ComboBox::paintEvent(QPaintEvent *pev)
{
    QComboBox::paintEvent(pev);
    if (m_binded && Qt::ControlModifier == QApplication::keyboardModifiers())
    {
        PAINT_MARKER;
    }
}

void ComboBox::markChanged()
{
    m_changed=true;
    setStyleSheet("color:blue;");
}

bool ComboBox::isChanged() const
{
    return m_changed;
}

void ComboBox::unmark()
{
    if(isChanged()) {setStyleSheet("");m_changed=false;}
}


void ComboBox::slot_TextChanged()
{
    markChanged();
}

void ComboBox::slot_activated()
{
    emit signal_activated();
}

qint32 ComboBox::currentItem()
{
    int index=-1;

    if(count()>0)
    {
            for(int i=0;i<count();i++)
            {
                if(currentText() == itemText(i))
                {
                    index=i;
                    break;
                }
            }
    }
    return index;
}

}
