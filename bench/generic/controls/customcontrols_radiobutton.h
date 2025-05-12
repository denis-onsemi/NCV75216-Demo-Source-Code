#ifndef CUSTOMRADIOBUTTON_H
#define CUSTOMRADIOBUTTON_H

#include "universalcontrol.h"

namespace CustomControls
{

/****************************************************************************
   * @class :
   * RadioButton
   ****************************************************************************/
class RadioButton: public QRadioButton, public UniversalControl
{
    Q_OBJECT
public:
    RadioButton(QWidget *p);

    ~RadioButton();
    bool editEnabled()const;
    void setEditEnabled(bool on);
    void setEditValue(double v);
    double editValue();
    const QString name(DragName dragname=NODENAME);
    bool bind(const char *slot, QObject *pobj);
    void unbind();
    void setDescription(const QString &description);
    const QString  description();
    void setCaption(const QString & caption);
    const QString  caption();

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
