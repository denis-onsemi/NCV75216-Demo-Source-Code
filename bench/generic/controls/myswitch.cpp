/**
 * @file:MySwitch.cpp   -
 * @description:
 * @project: BENCH OnSemiconductor
 * @date: 2014\11\27 13-52-19
 *
 */


#include "myswitch.h"
#include <QDebug>
namespace CustomControls
{

#define MAX_MODELS (12)

const struct {
    QStringList images;
    QList<QRect> rects;
}switchModels[MAX_MODELS] =
{
//SWITCH_ON_OFF,
    {
        QStringList()<<":/switches/img_sw_off"<<":/switches/img_sw_on",
        QList<QRect>(),
    },
//SWITCH_1_2
    {
        QStringList()<<":/switches/img_sw1_2_on2"<<":/switches/img_sw1_2_on1",
        QList<QRect>()<<QRect(0,20,32,12)<<QRect(0,0,32,12),
    },
//SWITCH_1_3
    {
        QStringList()<<":/switches/img_sw1_3_on3"<<":/switches/img_sw1_3_on2"
                        <<":/switches/img_sw1_3_on1",
        QList<QRect>()<<QRect(0,22,32,10)<<QRect(0,11,32,11)<<QRect(0,0,32,10),
    },
//SWITCH_1_4
    {
    QStringList()<<":/switches/img_sw1_4_on4"<<":/switches/img_sw1_4_on3"
                        <<":/switches/img_sw1_4_on2"<<":/switches/img_sw1_4_on1",
    QList<QRect>()<<QRect(17, 34, 19, 16)<< QRect(0, 25, 17, 20)<< QRect(0, 5, 17, 20)<<QRect(17, 0, 19, 16),
    },
//SIGNAL_ON_OFF
    {
    QStringList()<<":/switches/img_signal_0"<<":/switches/img_signal_1",
    QList<QRect>(),
    },
//SIGNAL_DIRECTION
    {
    QStringList()<<":/switches/img_buffer_left"<<":/switches/img_buffer_right",
    QList<QRect>(),
    },
//SIGNAL_0_1
    {
    QStringList()<<":/switches/img_btn_off"<<":/switches/img_btn_on",
    QList<QRect>(),
    },
//SIGNAL_0_A
    {
    QStringList()<<":/switches/img_btn_off"<<":/switches/img_btn_on_red",
    QList<QRect>(),
    },
//LED_RED
    {
    QStringList()<<":/leds/img_led_gray"<<":/leds/img_led_red",
    QList<QRect>(),
    },
//LED_GREEN
    {
    QStringList()<<":/leds/img_led_gray"<<":/leds/img_led_green",
    QList<QRect>(),
    },
//LED_RED_GREEN
    {
    QStringList()<<":/leds/img_led_gray"<<":/leds/img_led_green"
                                             <<":/leds/img_led_red",
    QList<QRect>(),
    }
};

/****************************************************************************
* @function name:
*
* @param:
*
      QWidget *parent
* @description:
* @return: (  )
****************************************************************************/
MySwitch::MySwitch(QWidget *parent) :
    QLabel(parent)
{
    clearState();
    setReadOnly(false);
    //setAttribute(Qt::WA_OpaquePaintEvent);
    setAutoFillBackground(true);
    m_mirrored_by_y = false;
    m_mirrored_by_x = false;
    mrAngle = 0.0f;
}


/****************************************************************************
* @function name: ~MySwitch - ---
*
* @param:
*             void
* @description:
* @return: ( MySwitch )
****************************************************************************/
MySwitch :: ~MySwitch()
{

}


/****************************************************************************
* @function name: addState - ---
*
* @param:
*
      const QString &name
       const QString &image_res
* @description:
* @return: ( void )
****************************************************************************/
void MySwitch :: addState(const QString &name, const QString &image_res, qint32 index)
{
    State state_item;
    state_item.state = (index == -1) ? mStatesList.count() : index;
    state_item.img_res = image_res;
    state_item.name = name;
    state_item.rect = QRect(0, 0, 0, 0);
    mStatesList.append(state_item);
    mStateMax = mStatesList.count() - 1;
}

void MySwitch::clearState()
{
    mStateIndex = 0;
    mStateChangeStep = 1;
    mStateMax = 0;
    mStateMin = 0;
}

/****************************************************************************
* @function name: MySwitch::setAngle - ---
*
* @param:
*
      const qreal &angle
* @description:
* @return: ( void )
****************************************************************************/
void MySwitch::setAngle(const qreal &angle)
{
    mrAngle = angle;
}

qreal MySwitch::angle() const
{
  return mrAngle;
}

/****************************************************************************
* @function name: MySwitch::setReadonly - ---
*
* @param:
*
      bool read_only
* @description:
* @return: ( void )
****************************************************************************/
void MySwitch::setReadOnly(bool read_only)
{
    mReadOnly = read_only;
    if (read_only)
    {
        setCursor(QCursor());
    }
    else
    {
        setCursor(Qt::PointingHandCursor);
    }
}

/****************************************************************************
* @function name: MySwitch::readOnly()
*
* @param:
*
* @description:
* @return: ( bool )
****************************************************************************/
bool MySwitch::readOnly() const
{
    return mReadOnly;
}

/****************************************************************************
* @function name: &MySwitch::name()
*
* @param:
*             void
* @description:
* @return: ( const )
****************************************************************************/
const QString &MySwitch::name() const
{
    return mName;
}


/****************************************************************************
* @function name: MySwitch::state()
*
* @param:
*
      void
* @description:
* @return: ( qint32 )
****************************************************************************/
qint32 MySwitch::state(void ) const
{
    return mStatesList[mStateIndex].state;
}


/****************************************************************************
* @function name: MySwitch::checkState - ---
*
* @param:
*
      const QString state_name
* @description:
* @return: ( bool )
****************************************************************************/
bool MySwitch::checkState(const QString state_name)
{
    return  (mStatesList[mStateIndex].name == state_name);
}


/****************************************************************************
* @function name: MySwitch::addStateRect - ---
*
* @param:
*
      qint32 state
       const QRect &rect
* @description:
* @return: ( void )
****************************************************************************/
void MySwitch::setStateRect(qint32 state, const QRect &rect)
{
    mStatesList[state].rect = rect;
}


/****************************************************************************
* @function name: setCurrentState - ---
*
* @param:
*
      qint32 state
* @description:
* @return: ( bool )
****************************************************************************/
void MySwitch::setCurrentState(qint32 new_state)
{
    qint32 i = 0;
    for (; i < mStatesList.size(); i++)
    {
        if (mStatesList[i].state == new_state)
        {
            setCurrentStateIndex(i);
        }
    }
}

bool MySwitch::mirroredX() const
{
    return m_mirrored_by_x ;
}

void MySwitch::setMirroredX(bool on)
{
    m_mirrored_by_x = on;
}

bool MySwitch::mirroredY() const
{
    return m_mirrored_by_y ;
}

void MySwitch::setMirroredY(bool on)
{
    m_mirrored_by_y = on;
}


void MySwitch::setSwitchModel(MySwitch::Model model)
{
    int i=0;
    clearState();
    foreach(const QString &image, switchModels[model].images)
    {
        addState("", image, i++);
    }
    i=0;
    foreach(const QRect &rect, switchModels[model].rects )
    {
        setStateRect(i++, rect);
    }
    this->repaint();
}

MySwitch::Model MySwitch::switchModel() const
{
    return m_model;
}

/****************************************************************************
* @function name: setCurrentStateIndex - ---
*
* @param:
*
      qint32 new_index
* @description:
* @return: ( bool )
****************************************************************************/
bool MySwitch :: setCurrentStateIndex(qint32 new_index)
{
    if(mStateIndex != new_index && new_index < mStatesList.size())
    {
        mStateIndex = new_index;
        repaint();
        emit signal_StateChanged(mStatesList[new_index].state);
        return true;
    }

    return false;
}
/****************************************************************************
* @function name: MySwitch::drawImageFromResource - ---
*
* @param:
*
      const QRect &rect
       const QString &image_path
* @description:
* @return: ( void )
****************************************************************************/
void MySwitch::drawImageFromResource(const QRect &rect, const QString &image_path)
{
    QPainter p( this );
    QPixmap img = QPixmap(image_path)
            .transformed(QMatrix().rotate(mrAngle))
            .transformed(QMatrix().scale( (m_mirrored_by_x?-1:1), (m_mirrored_by_y?-1:1) ));
    if (!img.isNull())
    {
        p.drawPixmap(rect, img);
    }
    p.end();
}


/****************************************************************************
* @function name: paintEvent - ---
*
* @param:
*
      QPaintEvent* pe
* @description:
* @return: ( void )
****************************************************************************/
void MySwitch :: paintEvent( QPaintEvent * )
{
    if (mStateIndex >=0 && mStateIndex <  mStatesList.count())
    {
        drawImageFromResource(rect(), mStatesList.at(mStateIndex).img_res);
    }
}


/****************************************************************************
* @function name: MySwitch::switchClicked - ---
*
* @param:
*
      const QPoint & pos
* @description:
* @return: ( void )
****************************************************************************/

void MySwitch::switchClicked(const QPoint &pos)
{    
    qint32 found_rect = -1;

        // first method when it detects the area of item        
        //matrix.rotate(-mrAngle);
        QRect orig_size = QPixmap( mStatesList.at(mStateIndex).img_res).rect();
        qreal qx =(qreal)orig_size.width()/rect().width();
        qreal qy = (qreal)orig_size.height()/rect().height();

        QPoint tr_pos = pos;
        tr_pos.setX(tr_pos.x()*qx);
        tr_pos.setY(tr_pos.y()*qy);

//        tr_pos.setX(tr_pos.x()*cos(90.0)+tr_pos.y()*sin(90.0));
//        tr_pos.setY(tr_pos.x()*sin(90.0)+tr_pos.y()*cos(90.0));
        if(m_mirrored_by_x) tr_pos.setX(orig_size.width()-tr_pos.x());
        if(m_mirrored_by_y) tr_pos.setY(orig_size.height()-tr_pos.y());

        for (int i = 0; i < mStatesList.count(); i++)
        {                                               
            QRect state_rect = mStatesList.at(i).rect;
            if(state_rect == QRect())
            {
                found_rect=-1;
                break;
            }
            else if ( state_rect.contains(tr_pos))
            {
                found_rect = i;
                break;
            }
        }

    if(found_rect<0)
    {
        found_rect = (mStateIndex + 1) % mStatesList.size();
    }
    if(found_rect>=0 && found_rect <mStatesList.count())
    {
        setCurrentStateIndex(found_rect);
        emit signal_clicked();
    }
}


/****************************************************************************
* @function name: mousePressEvent - ---
*
* @param:
*
      QMouseEvent *pevent
* @description:
* @return: ( void )
****************************************************************************/
void MySwitch :: mousePressEvent(QMouseEvent *pevent)
{
    if (pevent->buttons() & Qt::LeftButton )
    {
        if (!mReadOnly)
        {
            switchClicked(pevent->pos());
        }
    }
}

/****************************************************************************
* @function name: setStatesList()
*
* @param:
*
       const QStringList &statesList
* @description:Sets state List in format name=:/prefix/resource
*                !cannot have spaces
* @return: ( void )
****************************************************************************/
void MySwitch::setStatesList (const QStringList &statesList)
{
    // QStringList l = statesList;
    QListIterator<QString> itr (statesList );
    while (itr.hasNext())
    {
        QStringList l = itr.next().split('=');
        addState(l.at(0), l.at(1));
    }
}








/****************************************************************************
* @function name:
*
* @param:
*
      MySwitchSet::SwitchMethod method
* @description:
* @return: (  )
****************************************************************************/
MySwitchSet::MySwitchSet(MySwitchSet::SwitchMethod method)
    : QObject(0)
{
    mMethod = method;
}


/****************************************************************************
* @function name: t - ---
*
* @param:
*             void
* @description:
* @return: (  )
****************************************************************************/
MySwitchSet::~MySwitchSet()
{
    while (mSwitches.count())
    {
        delete mSwitches.first();
    }
}


/****************************************************************************
* @function name: MySwitchSet::addSwitch - ---
*
* @param:
*
      MySwitch *pswitch
* @description:
* @return: ( void )
****************************************************************************/
void MySwitchSet::addSwitch(MySwitch *pswitch)
{
    if (pswitch != NULL)
    {
        mSwitches.append(pswitch);
        connect(pswitch, SIGNAL(signal_clicked()), this, SLOT(slot_SwitchClick()));
    }
}


/****************************************************************************
* @function name: *MySwitchSet::getSwitch - ---
*
* @param:
*
      qint32 sw_number
* @description:
* @return: ( MySwitch )
****************************************************************************/
MySwitch *MySwitchSet::getSwitch(qint32 sw_number)
{
    return mSwitches[sw_number];
}


/****************************************************************************
* @function name: MySwitchSet::setSwitchState - ---
*
* @param:
*
      qint32 sw_number
       qint32 sw_state
* @description:
* @return: ( void )
****************************************************************************/
void MySwitchSet::setSwitchState(qint32 sw_number, qint32 sw_state)
{
    mSwitches[sw_number]->setCurrentState(sw_state);
    slot_SwitchClick();
}


/****************************************************************************
* @function name: MySwitchSet::setSwitchStateByName - ---
*
* @param:
*
      const QString sw_name
       qint32 sw_state
* @description:
* @return: ( void )
****************************************************************************/
void MySwitchSet::setSwitchStateByName(const QString sw_name, qint32 sw_state)
{
    qint32 i;
    qint32 sw_number = -1;
    for ( i = 0; i < mSwitches.count(); i++)
    {
        if (mSwitches.at(i)->name().compare(sw_name))
        {
            sw_number = i;
            break;
        }
    }
    mSwitches[sw_number]->setCurrentState(sw_state);
    slot_SwitchClick();
}


/****************************************************************************
* @function name: MySwitchSet::slot_SwitchClick - ---
*
* @param:
*             void
* @description:
* @return: ( void )
****************************************************************************/
void MySwitchSet::slot_SwitchClick()
{
    MySwitch *psw = static_cast<MySwitch *>(QObject::sender());
    for (qint32 i = 0; i < mSwitches.count(); i++)
    {
        if (mSwitches[i] != psw)
        {
            switch (mMethod)
            {

            case Alltogether:
                mSwitches[i]->setCurrentState( psw->state() );
                break;

            case SingleOn:
                mSwitches[i]->setCurrentState(0);
                break;

            default:
                break;

            }
        }
    }
}

} //namespace::CustomControls
