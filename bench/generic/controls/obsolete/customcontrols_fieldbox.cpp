#include "customcontrols_fieldbox.h"
#include <QLabel>

namespace CustomControls
{

  FieldBox::FieldBox(QWidget *parent): QWidget(parent)
  {
    mp_layout = new QHBoxLayout(this);
    mp_le =  new LineEdit(this);
    this->setLayout(mp_layout);
  }

  FieldBox::~FieldBox()
  {

  }

  void FieldBox::addBits(const QString &fieldname)
  {
    Register reg;
    reg.addBit(fieldname.toLatin1());
    QString regname = UniversalControl::extractParameter(fieldname);
    addBits(regname, 0, reg.size() - 1);
  }

  void FieldBox::addBits(const QString &regname, qint32 n_from, qint32 n_to)
  {
    if (n_from > n_to)
      {
        qint32 tmp = n_from;
        n_from = n_to;
        n_to = tmp;
      }
    mp_layout->addWidget(new QLabel(regname));
    mp_layout->addSpacing(10);    
    mp_le->setObjectName(QString("le%1_%2_%3").arg(regname).arg(n_from).arg(n_to));
    mp_layout->addWidget(mp_le);
    qint32 count = n_to - n_from + 1;
    qint32 i = count;
    while ( count-- )
      {
        CheckBox *pcb = new CheckBox(this);
        pcb->setObjectName(QString("cb%1_%2").arg(regname).arg(n_to));
        if (i % 8 == 0 )
          {
            mp_layout->addSpacing(10);
          }
        mp_layout->addWidget(pcb);
        m_cb_list.append(pcb);
        i--;
        n_to--;
      }
  }

}
