#include "customcontrols_lineedit.h"

namespace CustomControls
{

LineEdit::LineEdit(QWidget *p): QLineEdit(p), UniversalControl()
{
    m_allowUpdate = false;
    setFocusPolicy(Qt::WheelFocus);
    mp_menu = new QMenu();
    mp_action_group = new QActionGroup(this);
    mp_actionHex =  mp_menu->addAction("HEX");
    mp_actionBin =  mp_menu->addAction("BIN");
    mp_actionDec =  mp_menu->addAction("DEC");
    mp_actionArray =  mp_menu->addAction("ARRAY");
    mp_actionDec->setCheckable(true);
    mp_actionDec->setChecked(true);
    mp_actionHex->setCheckable(true);
    mp_actionBin->setCheckable(true);
    mp_actionArray->setCheckable(true);
    mp_action_group->addAction(mp_actionBin);
    mp_action_group->addAction(mp_actionHex);
    mp_action_group->addAction(mp_actionDec);
    mp_action_group->addAction(mp_actionArray);
    m_input_base = 10;
    m_array=false;
    connect(mp_actionHex, SIGNAL(toggled(bool)), this, SLOT(slot_ActionMode(bool)));
    connect(mp_actionBin, SIGNAL(toggled(bool)), this, SLOT(slot_ActionMode(bool)));
    connect(mp_actionDec, SIGNAL(toggled(bool)), this, SLOT(slot_ActionMode(bool)));
    connect(mp_actionArray, SIGNAL(toggled(bool)), this, SLOT(slot_ActionMode(bool)));
    connect(this,SIGNAL(textChanged(QString)),this,SLOT(slot_TextChanged()));
    connect(this,SIGNAL(returnPressed()),this,SLOT(slot_triggered()));
    connect(this,SIGNAL(editingFinished()),this,SLOT(slot_triggered()));
    m_HasCachedName = false;
    setControlType(LINEEDIT);
}

LineEdit::~LineEdit()
{
}

qint32 LineEdit::inputBase() const
{
    return m_input_base;
}

bool LineEdit::editEnabled() const
{
    return !isReadOnly();
}

void LineEdit::setEditEnabled(bool on)
{
    setReadOnly(!on);
}

void LineEdit::setEditValue(double v)
{
    if(!m_array)
    {
        setText(QString::number((quint32)v, inputBase()).toUpper());
    }
    unmark();
}

double LineEdit::editValue()
{
    double result ;
    bool ok;
    if(text().startsWith("0x")) {
        m_input_base=16;
        result = (double)text().mid(2).toInt(&ok, inputBase());
        setText(text().mid(2));
    }
    else if(text().startsWith("0b")) {
        m_input_base=2;
        result = (double)text().mid(1).toInt(&ok, inputBase());
        setText(text().mid(1));
    }
    else if(text().startsWith("0d")) {
        m_input_base=2;
        result = (double)text().mid(1).toInt(&ok, inputBase());
        setText(text().mid(1));
    }
    else if(text().startsWith("#")) {
        m_array=true;
        ok=true;
        setText(text().mid(1));
    }
    else {
        result = (double)text().toInt(&ok, inputBase());
    }

    if (!ok)
    {
        markError();
    }    return result;
}

void LineEdit::setInputBase(int input_base)
{
    m_input_base = input_base;
    setEditValue(editValue());
}

const QString LineEdit::name(DragName dragname)
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

bool LineEdit::bind(const char *slot, QObject *pobj)
{
    m_binded = true;
    return connect(this, SIGNAL(signal_activated()), pobj, slot);
}

void LineEdit::unbind()
{
    disconnect(this, SIGNAL(signal_activated()));
}

void LineEdit::setDescription(const QString &description)
{
    this->setToolTip(description);
}

const QString LineEdit::description()
{
    return toolTip();
}

QByteArray LineEdit::array()
{
    return text().toLatin1();
}

void LineEdit::setArray(const QByteArray &data)
{
    setText(data);
}

void LineEdit::focusInEvent(QFocusEvent *pevent)
{
    QLineEdit::focusInEvent(pevent);
    m_allowUpdate = true;
    if (!(QApplication::mouseButtons() & Qt::LeftButton)){
        this->clearFocus();
    }
}

void LineEdit::focusOutEvent(QFocusEvent *pevent)
{
    QLineEdit::focusOutEvent(pevent);
    m_allowUpdate = false;
}

void LineEdit::contextMenuEvent(QContextMenuEvent *event)
{
    if(m_array)
    {
        mp_actionArray->setChecked(1);
    }
    else {
        if(inputBase()==2){
            mp_actionBin->setChecked(1);
        }
        else if(inputBase()==10)
        {
            mp_actionDec->setChecked(1);
        }
        else if(inputBase()==16)
        {
            mp_actionHex->setChecked(1);
        }
    }
    mp_menu->exec(event->globalPos());

}

void LineEdit::mousePressEvent(QMouseEvent *pevent)
{
    QLineEdit::mousePressEvent(pevent);
    if (pevent->button() == Qt::LeftButton
            && Qt::ControlModifier == QApplication::keyboardModifiers() )
    {
        startDrag(static_cast<UniversalControl*>(this), DRAG_SETGET);
        stopDrag();
    }
}

void LineEdit::paintEvent(QPaintEvent *pev)
{
    QLineEdit::paintEvent(pev);
    if (m_binded &&Qt::ControlModifier == QApplication::keyboardModifiers())
    {
        PAINT_MARKER;
    }    
    //    if(text().isEmpty()){
    //        QPainter p(this);
    //        p.setPen(QPen(Qt::lightGray));
    //        QRect r= this->rect();
    //        r.setX(r.x()+5);
    //        r.setY(r.y()+2);
    //        if(this->m_input_base ==16)
    //            p.drawText(r,"Hexademical");
    //        else if(this->m_input_base ==10)
    //            p.drawText(r,"Decimal");
    //        else if(this->m_input_base ==10)
    //            p.drawText(r,"Binary");
    //    }
}

void LineEdit::wheelEvent(QWheelEvent *pev)
{
    static qint32 delta = 0;
    if(!isReadOnly() && m_allowUpdate)
    {
        delta += pev->delta();

        if (delta >= 120 || delta <= -120)
        {
            setEditValue(this->editValue() + delta / 120);
            delta %= 120;
            markChanged();
        }
    }
}

void LineEdit::keyPressEvent(QKeyEvent *pev)
{
    QLineEdit::keyPressEvent(pev);
    if (pev->key() == Qt::Key_Up)
    {
        setEditValue(this->editValue() + 1);
    }
    else if (pev->key() == Qt::Key_Down)
    {
        setEditValue(this->editValue() - 1);
    }
}

void LineEdit::markChanged()
{
    m_changed =true;
    setStyleSheet("color:blue;");
}

void LineEdit::markError()
{
    setStyleSheet("color:red");
}

void LineEdit::unmark()
{
    if(m_changed) {setStyleSheet("");m_changed=false;}
}

bool LineEdit::isChanged() const
{
    return m_changed;
}

void LineEdit::construct()
{
    editValue();
}

void LineEdit::slot_TextChanged()
{
    markChanged();
}

void LineEdit::slot_ActionMode(bool)
{
    bool ok;
    quint32 value = text().toUInt(&ok,m_input_base);
    if(!ok ) value = 0;
    if (mp_actionBin->isChecked())
    {
        m_input_base = 2;
    }
    else if (mp_actionDec->isChecked())
    {
        m_input_base = 10;
    }
    else if (mp_actionHex->isChecked())
    {
        m_input_base = 16;
    }
    else if (mp_actionHex->isChecked())
    {
        m_array =true;
    }
    this->setEditValue((double)value);
}

void LineEdit::slot_triggered()
{
    if(isChanged()) {
        emit signal_activated();
        unmark();
    }
}

}

