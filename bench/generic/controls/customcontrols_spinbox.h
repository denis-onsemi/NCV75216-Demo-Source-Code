#ifndef CUSTOMSPINBOX_H
#define CUSTOMSPINBOX_H

#include "universalcontrol.h"

namespace CustomControls
{



/****************************************************************************
 * @class :
 * SpinBox
 ****************************************************************************/
class SpinBox: public QSpinBox, public UniversalControl
{
    Q_OBJECT
public:
    SpinBox(QWidget *p);
    ~SpinBox();
    bool editEnabled() const;
    void setEditEnabled(bool on);
    void setEditValue(double v);
    double editValue();
    const QString name(DragName dragname=NODENAME);

    bool bind(const char *slot, QObject *pobj);
    void unbind();
    void setDescription(const QString &description);
    const QString  description();
    void setCaption(const QString & ){}
    const QString  caption(){return QString();}

protected:
    void leaveEvent(QEvent *);
    void mouseMoveEvent(QMouseEvent *);
    void mousePressEvent(QMouseEvent *pevent);
    /* paint event used for painting rectangle on CTRL press*/
    void paintEvent(QPaintEvent *pev);
private:

};
}

#endif
