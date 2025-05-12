#ifndef CUSTOMSWITCH_H
#define CUSTOMSWITCH_H

#include "universalcontrol.h"
#include "myswitch.h"

namespace CustomControls
{


/****************************************************************************
  * @class :
  * Switch
  ****************************************************************************/
class Switch : public MySwitch, public UniversalControl
{

    Q_OBJECT
    Q_ENUMS(Model)
    Q_PROPERTY(Model model READ switchModel WRITE setSwitchModel)
    Q_PROPERTY(int currentState READ state WRITE setCurrentState)
    Q_PROPERTY(double angle READ angle WRITE setAngle)
    Q_PROPERTY(bool mirroredX READ mirroredX WRITE setMirroredX)
    Q_PROPERTY(bool mirroredY READ mirroredY WRITE setMirroredY)

public:

    Switch(QWidget *p);
    ~Switch();

    /* instantiated from universalcontrol*/
    bool editEnabled()const;
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
    void construct();
    void mousePressEvent(QMouseEvent *pevent);
    void leaveEvent(QEvent *);
    void mouseMoveEvent(QMouseEvent *);
    void paintEvent(QPaintEvent *pev);

private:


};
}

#endif
