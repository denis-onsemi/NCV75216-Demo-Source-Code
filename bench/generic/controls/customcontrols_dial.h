#ifndef CUSTOMDIAL_H
#define CUSTOMDIAL_H

#include "universalcontrol.h"

namespace CustomControls
{



/****************************************************************************
 * @class :
 * Dial
 ****************************************************************************/
class Dial : public QDial, public UniversalControl
{
    Q_OBJECT
public:
    Dial(QWidget *p);
    ~Dial();
    bool editEnabled()const;
    void setEditEnabled(bool );
    void setEditValue(double v);
    double editValue();
    const QString name(DragName dragname=NODENAME);

    bool bind(const char *slot, QObject *pobj);
    void unbind();
    void setDescription(const QString &description);
    const QString  description();
    void setCaption(const QString & ){}
    const QString  caption(){return QString();}
    virtual void setEditValueRange(double min,double max);

protected:

    void mousePressEvent(QMouseEvent *pevent);
    void leaveEvent(QEvent *ev);
    void mouseMoveEvent(QMouseEvent *ev);
    /* paint event used for painting rectangle on CTRL press*/
    void paintEvent(QPaintEvent *pev);
private:
    qint32 m_value;


};
}
#endif // CUSTOMDIAL_H
