#ifndef CUSTOMBUTTONBOX_H
#define CUSTOMBUTTONBOX_H

#include "universalcontrol.h"
#include <QGroupBox>
#include <QVBoxLayout>
namespace CustomControls
{

/****************************************************************************
   * @class :
   * RadioButton
   ****************************************************************************/
class ButtonBox:public QGroupBox, public UniversalControl
{
    Q_OBJECT
public:
    typedef enum  {HORIZONTAL, VERTICAL} Orientation;
    typedef enum { BUTTON, RADIO} Type;
    ButtonBox(QWidget *p);


    ~ButtonBox();
    void construct(Orientation orientation, Type type, const QStringList &list);
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

    void makeSetup(const QString &init_script);

signals:
    void signal_clicked();

protected:
    void leaveEvent(QEvent *);
    void mouseMoveEvent(QMouseEvent *);
    void mousePressEvent(QMouseEvent *pevent);
    /* paint event used for painting rectangle on CTRL press*/
    void paintEvent(QPaintEvent *pev);

private:

    bool m_editable;
    bool m_constructed;
    QList<QAbstractButton *> m_buttons;
    QLayout *mp_layout;
private slots:
    void slot_on_click();
};


}


#endif
