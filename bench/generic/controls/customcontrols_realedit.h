#ifndef CUSTOMCONTROLS_REALEDIT_H
#define CUSTOMCONTROLS_REALEDIT_H

#include "universalcontrol.h"
#include <QLineEdit>


namespace CustomControls
{
/****************************************************************************
 * @class :
 * RealEdit
 ****************************************************************************/
class RealEdit : public QLineEdit, public UniversalControl
{
    Q_OBJECT
public:
    enum{
        GIGA = (0),
        MEGA =(1),
        KILO =(2),
        NONE =(3),
        MILI =(4),
        MICRO =(5),
        NANO =(6),
        PICO= (7),
        PREFIX_COUNT =(8)
    };

    static const char m_metricPrefixSymbol[PREFIX_COUNT];
    static const double m_metricPrefixMultiplier[PREFIX_COUNT];

    explicit RealEdit(QWidget *p);

    ~RealEdit();

    void setPrecision(qint32 val);

    qint32 precision() const;

    bool engFormat() const;
    void setEngFormat(bool on);

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

signals:
    void signal_activated();

protected:
    int m_prefix_i ;
    qint32 m_precision;
    double m_precision_mask;
    bool m_changed;
    double m_prev_value;
    bool m_engFormat;
    bool m_allowUpdate;
    QActionGroup *mp_action_group;
    QMenu *mp_menu;
    void focusInEvent(QFocusEvent *pevent);
    void focusOutEvent(QFocusEvent *pevent);
    void contextMenuEvent(QContextMenuEvent *event);
    void mousePressEvent(QMouseEvent *pevent);
    /* paint event used for painting rectangle on CTRL press*/
    void paintEvent(QPaintEvent *pev);
    void wheelEvent(QWheelEvent *pev);
    void keyPressEvent(QKeyEvent *pev);


    static int detectPrefix(const QString &text);
    double toNormalFormat(const QString &text);
    QString toEngFormat(double v, qint32 precision);
    void markChanged();
    void markError();
    void unmark();
    bool isChanged()const;
    void construct();


private slots:


    void slot_CustomPrecision();
    void slot_TextChanged();
    void slot_triggered();

private:

};

}


#endif // CUSTOMCONTROLS_REALEDIT_H
