#ifndef CUSTOMPROGRESSBAR_H
#define CUSTOMPROGRESSBAR_H


#include "universalcontrol.h"

namespace CustomControls
{



/****************************************************************************
   * @class :
   * ProgressBar
   ****************************************************************************/
class ProgressBar : public QProgressBar, public UniversalControl
{
    Q_OBJECT
public:
    ProgressBar(QWidget *p);
    ~ProgressBar();

    bool editEnabled()const;
    void setEditEnabled(bool );
    void setEditValue(double v);
    double editValue();
    const QString name(DragName dragname=NODENAME);

    bool bind(const char *, QObject *);
    void unbind();
    void setDescription(const QString &description);
    const QString  description();
    void setCaption(const QString & ){}
    const QString  caption(){return QString();}
    virtual void setEditValueRange(double min,double max);

protected:

    void mousePressEvent(QMouseEvent *pevent);
    /* paint event used for painting rectangle on CTRL press*/
    void paintEvent(QPaintEvent *pev);
    void leaveEvent(QEvent *);
    void mouseMoveEvent(QMouseEvent *);

};
}
#endif // CUSTOMPROGRESSBAR_H
