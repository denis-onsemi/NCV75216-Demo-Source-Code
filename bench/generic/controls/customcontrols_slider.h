#ifndef CUSTOMCONTROLS_SLIDER_H
#define CUSTOMCONTROLS_SLIDER_H

#include "universalcontrol.h"

namespace CustomControls
{



/****************************************************************************
 * @class :
 * Slider
 ****************************************************************************/
class Slider : public QSlider, public UniversalControl
{
    Q_OBJECT
public:
    Slider(QWidget *p);
    ~Slider();
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

#endif // CUSTOMCONTROLS_SLIDER_H
