#ifndef CUSTOMBITBOX_H
#define CUSTOMBITBOX_H

#include "universalcontrol.h"
#include "customcontrols_checkbox.h"
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QLabel>
#include "register.h"

namespace CustomControls
{

/****************************************************************************
 * @class :
 * BitBox
 ****************************************************************************/
class BitBox : public QFrame
{
    Q_OBJECT
public:
    BitBox(QWidget *parent);
    typedef enum
    {
        HORIZONTAL ,
        VERTICAL,
        BOX4,
        BOX8,
        BOX16 ,
        BOX
    } Layout;

    void setBindName(const QString & name)
        {if(name.isEmpty()) addBits(name); m_bindName = name;}
    const QString & bindName() const
        {return m_bindName;}

    /* sets layout type for bitbox HORIZONTAL or VERTICAL */
    void setLayoutType(Layout layout );
    /* adds register with register name */
    void addRegister( const Register &reg);
    /* adds bits in text format reg[10] will add 10 checkboxes*/
    void addBits(const QString &fieldname);
    /* add bits as register name and number bits to add*/
    //void addBits(const QString &regname, qint32 count, qint32 start = 0);
    /* adds bit with name (checkbox) */
    void addBit(const QString &name);
    /* removes  All bits(checkboxes) from bitbox */
    void removeAll();

private:
    QList<CheckBox *> m_cb_list;
    Layout m_layout;
    QGridLayout *mp_layout;
    QString m_bindName;

    /* adds item widget with name */
    void addItemWidget(const QString &name);
};

}
#endif // CUSTOMBITBOX_H
