#include "customcontrols_bitbox.h"
#include <bitfieldparser.h>

namespace CustomControls
{

BitBox::BitBox(QWidget *parent): QFrame(parent)
{
    mp_layout = new QGridLayout(this);
    setLayoutType(HORIZONTAL);
}


void BitBox::setLayoutType(BitBox::Layout layout)
{
    m_layout = layout;
    setLayout(mp_layout);
}

void BitBox::addRegister(const Register &reg)
{  
    for(int i=reg.size()-1;i>=0;i--)
    {
        addItemWidget(QString("cb%1_%2").arg(reg.name()).arg(i));
    }
}

void BitBox::addBits(const QString &fieldname)
{
    BitFieldParser parser(fieldname.toLatin1());
    if(parser.isOk())
    {

        int from = parser.lsb();
        int to = parser.msb();
        if(from>to ) qSwap(from,to);
        for(int i=to;i>=from;i--)
        {
            addItemWidget(QString("cb%1_%2").arg(parser.name()).arg(i));
        }
    }
}


void BitBox::addBit(const QString &name)
{
    addItemWidget(name);
}

void BitBox::removeAll()
{
    while (m_cb_list.size())
    {
        CheckBox *pcb = m_cb_list.at(0);
        mp_layout->removeWidget(pcb);
        delete (pcb);
        m_cb_list.removeFirst();
    }

}

void BitBox::addItemWidget(const QString &name)
{
    CheckBox *pcb = new CheckBox(this);
    pcb->setObjectName(name);
    m_cb_list.append(pcb);
    qint32 row=0,col=0;
    QGridLayout *pgrid = static_cast<QGridLayout*>(mp_layout);
    switch(m_layout)
    {
    case HORIZONTAL:
        row = 0;
        col = m_cb_list.count();
        break;
    case VERTICAL:
        row = m_cb_list.count();;
        col = 0;
        break;
    case BOX8:
        row = (m_cb_list.count()-1)/8;
        col = (m_cb_list.count()-1)%8;
        break;
    case BOX16:
        row = (m_cb_list.count()-1)/16;
        col = (m_cb_list.count()-1)%16;
        break;
    }
    pgrid->addWidget(pcb, row, col);
}
}
