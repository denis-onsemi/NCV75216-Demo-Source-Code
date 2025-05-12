/**
 * @file:MySwitch.h   -
 * @description:
 * @project: BENCH OnSemiconductor
 * @date: 2014\11\27 13-52-19
 *
 */


#ifndef MYSWITCH_H
#define MYSWITCH_H

#include <QLabel>
#include <QPainter>
#include <QRect>
#include <QEvent>
#include <QMouseEvent>

namespace CustomControls
{

/****************************************************************************
 * @class :
 * MySwitch
 * this class is universal switch or readonly (exampe LEDs)
 ****************************************************************************/

class MySwitch : public QLabel
{

    Q_OBJECT
public:

    typedef enum
    {
        SWITCH_ON_OFF,
        SWITCH_1_2,
        SWITCH_1_3,
        SWITCH_1_4,
        SIGNAL_ON_OFF,
        SIGNAL_DIRECTION,
        SIGNAL_0_1,
        SIGNAL_0_A,
        LED_RED,
        LED_GREEN,
        LED_RED_GREEN
    }Model;



    void setSwitchModel(Model model);
    Model switchModel() const ;

    explicit MySwitch(QWidget *parent );
    ~MySwitch();
    /* add state name and image resourse*/
    void addState(const QString &name, const QString &image_res, qint32 state = -1);
    /* clears states*/
    void clearState();
    /* state number */
    qint32 state(void )const ;
    /* checks if current state is equal to <state_name> */
    bool checkState(const QString state_name);
    /* when click on this <rect> state will changed to <state> */
    void setStateRect(qint32 state, const QRect &rect);
    /* rotate control image*/
    void setAngle(const qreal &angle);
    /* angle of switch image*/
    qreal angle() const;
    /* sets read only control */
    void setReadOnly(bool read_only);
    /* returns  true when readonly*/
    bool readOnly() const;
    /* control name */
    const QString &name() const;
    /* sets states list in format name=:/resource*/
    void setStatesList (const QStringList &text);
    /**/
    bool setCurrentStateIndex(qint32 new_index);

    bool mirroredX() const;
    void setMirroredX(bool on);
    bool mirroredY() const;
    void setMirroredY(bool on);

protected:

    QString mName;
    bool mDoNotUpdate;
    bool mReadOnly;
    bool m_mirrored_by_x;
    bool m_mirrored_by_y;
    qreal mrAngle;
    qint32 mStateIndex;
    qint32 mStateChangeStep;
    qint32 mStateMax;
    qint32 mStateMin;
    Model m_model;

    typedef struct
    {
        qint32 state;
        QString name;
        QString img_res;//image in resource
        QRect rect;
    } State;

    QList<State> mStatesList;
    void switchClicked(const QPoint &pos);
    void drawImageFromResource(const QRect &rect, const QString &image_path);
    void paintEvent( QPaintEvent *pe );
    void mousePressEvent(QMouseEvent *pevent);

    /*QOBJECT SIGNALS*/
signals:
    void signal_StateChanged(qint32);
    void signal_clicked();

public slots:
    void setCurrentState(qint32 state) ;
};


/****************************************************************************
 * @class :
 * MySwitchSet
 * this class is for grouping switches
 ****************************************************************************/
class MySwitchSet: public QObject
{
    Q_OBJECT

public:
    typedef enum
    {
        None = 0,
        SingleOn,
        Alltogether
    } SwitchMethod;

    MySwitchSet(SwitchMethod method);
    ~MySwitchSet();
    void addSwitch(MySwitch *pswitch);
    MySwitch *getSwitch(qint32 sw_number);
    void setSwitchState(qint32 sw_number, qint32 sw_state);
    void setSwitchStateByName(const QString sw_name, qint32 sw_state);

private:
    SwitchMethod mMethod;
    QList<MySwitch *> mSwitches;
private slots:
    void slot_SwitchClick(void);
};

}//namespace::CustomControls

#endif // MySwitch_H
