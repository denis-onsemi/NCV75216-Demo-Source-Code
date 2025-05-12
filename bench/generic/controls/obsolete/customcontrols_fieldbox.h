#ifndef CUSTOMCONTROLS_FIELDBOX_H
#define CUSTOMCONTROLS_FIELDBOX_H

#include "universalcontrol.h"
#include "customcontrols_checkbox.h"
#include "customcontrols_lineedit.h"
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <register.h>

namespace CustomControls
{

/****************************************************************************
 * @class :
 * BitBox
 ****************************************************************************/
class FieldBox : public QWidget
{
public:
    FieldBox(QWidget *parent);
    ~FieldBox();
    // TODO alter this function
    void addBits(const QString &fieldname);

    void addBits(const QString &regname, qint32 n_from, qint32 n_to);


private:
    QHBoxLayout *mp_layout;
    QList<CheckBox *> m_cb_list;
    LineEdit *mp_le;
};

}
#endif // CUSTOMCONTROLS_FIELDBOX_H
