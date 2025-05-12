#include "customcontrols_RegisterBox.h"
#include <QWidget>
#include <QGroupBox>





namespace CustomControls
{


RegisterBox::RegisterBox(QWidget *parent): QScrollArea(parent)
{

    m_group_name = "bank";
    m_opt=0;

}

RegisterBox::~RegisterBox()
{
    clear();
}




void RegisterBox::addRegister(Register &reg)
{    
    static const char *COLORS[3] = {"rgba(202, 202, 202, 162)"
                                    , "rgba(255, 202, 156, 162)"
                                    ,"rgba(142, 203, 255, 162)"
                                   };


    clear();
    widget()->setFixedWidth(width());
    if(!widget()->layout()) {
        widget()->setLayout(new QVBoxLayout(this));
        widget()->layout()->setContentsMargins(0,0,0,0);
    }



    int col=0;
    int row =0;
    foreach(const QString &item , reg.toString("@purename;").split(";",QString::SkipEmptyParts))
    {
        int type = Control::Bit;
        int v=0;
        int bank = reg.sub(item)->bitAt(0)->extra( m_group_name ).toInt();
        QGroupBox *pwdg;
        if(m_banks.contains(bank))
        {
            pwdg = m_banks[bank];
        }
        else
        {
            pwdg = new  QGroupBox(widget());
            widget()->layout()->addWidget(pwdg);
            m_banks[bank] = pwdg;
            pwdg->setLayout(new QGridLayout());
            pwdg->layout()->setContentsMargins(0,0,0,0);


            // header
            if(m_opt & ShowHeader){
                pwdg->setTitle(QString("%1 (%2)").arg(m_group_name.toUpper()).arg(bank));
                pwdg->layout()->setContentsMargins(0,15,0,5);
            }

            // opt
            QString color = reg.sub(item)->bitAt(0)->extra( "color" );

            if(color.isEmpty()){
                if(m_opt & BankZebra )
                {
                    int ci =0;
                    ci =  bank % 2 +1;
                    color = COLORS[ci];
                }
                else
                {
                    color = COLORS[0];
                }
            }

            if(m_opt & EnableChecked){
                pwdg->setCheckable(true);
            }

            pwdg->setStyleSheet(QString("QGroupBox{"
                                        "font-weight: bold; "
                                        "color: grey; "
                                        "background-color: qlineargradient(spread:reflect, x1:0.432, y1:0, x2:0.432, y2:0.466318, stop:0.1875 %1, stop:1 rgba(255, 255, 255, 255));"
                                        "border: 2px solid %2; "
                                        "border-radius:5px;}").arg(color).arg(color));

            col =0;
            row=0;
        }


        int offset= 0;

        QString caption = item;
        if(m_opt & AbsoluteOffset)
        {
            offset = reg.findBitByName(item);
            caption = QString("%1[%2]").arg(item).arg(offset);
        }

        if(reg.sub(item)->size() >1)
        {
            type = Control::Field;
            caption = QString("%1[%2:%3]").arg(item).arg(0 + offset).arg(reg.sub(item)->size()-1+offset);
            v= reg.sub(item)->toUInt();
        }

        Control *pctrl = new Control(this, type);
        pctrl->setCaption(caption);
        pctrl->setValue(v);
        pctrl->setAccessibleName(QString("%1[%2]").arg(reg.name()).arg(item));

        if(col >= m_cols) {            row++; col=0;            }
        (static_cast<QGridLayout*>(pwdg->layout()))->addWidget(pctrl, row, col,1,1);
        col++;
    }
}


void RegisterBox::clear()
{    
    foreach(const int &num,m_banks.keys()) delete m_banks[num];
    m_banks.clear();
}

RegisterBox::Control::Control(QWidget *p, int type)
    :QWidget(p)

{

    this->setLayout(new QHBoxLayout());
    layout()->setContentsMargins(0,0,0,0);
    setSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed);
    m_type = type;

    switch(type)
    {
    case Bit:
        mp_caption =  new QLabel(this);
        mp_caption->setFixedWidth(100);
        layout()->addWidget(mp_caption);
        mp_cb =  new CheckBox(this);
        mp_cb->setFixedWidth(25);
        layout()->addWidget(mp_cb);
        break;

    case Field:
        mp_caption =  new QLabel(this);
        mp_caption->setFixedWidth(100);
        layout()->addWidget(mp_caption);
        mp_le = new LineEdit(this);
        mp_le->setFixedWidth(80);
        layout()->addWidget(mp_le);                
        break;
    default:
        break;
    }


}



void  RegisterBox::Control::setAccessibleName(const QString &name)
{
    switch(m_type)
    {
    case Bit:
        mp_cb->setAccessibleName(name);
        break;
    case Field:
        mp_le->setAccessibleName(name);
        break;
    }
}

void RegisterBox::Control::setCaption(const QString &caption)
{

        mp_caption->setText(caption);
}

void RegisterBox::Control::setValue(int v)
{
    switch(m_type)
    {
    case Bit:
        mp_cb->setChecked(v);
        break;
    case Field:
        mp_le->setEditValue(v);
        break;
    }
}

void CustomControls::RegisterBox::setOptions(unsigned int opt)
{
    m_opt = opt;
}

QList<int> CustomControls::RegisterBox::checkedBanks()
{
    QList<int> l;
    foreach(const int &num, m_banks.keys()){
        if(m_banks[num]->isChecked()){
            l.append(num);
        }
    }
    return l;
}

void CustomControls::RegisterBox::setChecked(int n, bool on)
{
    if(m_banks.contains(n) && m_banks[n])
            m_banks[n]->setChecked(on);
}

}
