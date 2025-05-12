#ifndef UNIVERSALCONTROL_H
#define UNIVERSALCONTROL_H

#include <QApplication>
#include <QCheckBox>
#include <QProgressBar>
#include <QDial>
#include <QMouseEvent>
#include <QLineEdit>
#include <QPushButton>
#include <QComboBox>
#include <QRadioButton>
#include <QFocusEvent>
#include <QSpinBox>
#include <QSlider>
#include <Qdebug>
#include <QMenu>
#include <QPainter>
#include <QDrag>
#include <QMimeData>
#include "translator.h"
#include "variable.h"

namespace CustomControls
{




/****************************************************************************
   * @class :
   * UniversalControl
   ****************************************************************************/
class UniversalControl
{


#define PAINT_MARKER do{\
        QPoint p = parentWidget()->mapFromGlobal(QCursor::pos());\
        if(this->geometry().contains(p))\
        {\
            QPainter p( this );\
            QBrush brush;\
            const QColor color =QColor(255, 170, 00, 100);\
            brush.setColor(Qt::red);\
            p.setBrush(brush);\
            p.fillRect(this->rect(), QBrush(color));\
            p.end();\
        }\
    }while(0)\
         

public:
    /* DRAG & DROP*/
    /* commands */
    typedef enum
    {
        DRAG_SET = 1,
        DRAG_GET = 2,
        DRAG_SETGET = 3,
        DRAG_RUN = 4
    } DragType;

    typedef enum
    {
        NONE = 0,
        CHECKBOX,
        PUSHBUTTON,
        SPINBOX,
        LINEEDIT,
        REALEDIT,
        COMBOBOX,
        BUTTONBOX,
        SWITCH,
        PROGRESSBAR,
        DIAL,
        SLIDER,
        RADIOBUTTON
    } CustomControlType ;

    typedef enum
    {
        PARAMETER,
        NODENAME,
        FULLPATH
    }DragName;

    explicit UniversalControl()
    {
        m_type = NONE;
        m_binded = false;
        m_already_constructed = false;
    }


    ~UniversalControl();

//    void setAppletName(const QString & name){m_appletName = name;}
//    const QString &appletName() const {return m_appletName;}

    void setBindName(const QString & name) {m_bindName = name;}
    const QString & bindName() const {return m_bindName;}

    /* returns if control is editable*/
    virtual bool editEnabled()const = 0 ;
    /* set editable mode */
    virtual void setEditEnabled(bool on) = 0;
    /* sets edit value for control */
    virtual void setEditValue(double v) = 0;
    /* reads edit value of control */
    virtual double editValue() = 0;
    /* name of control in formar regname[x],regname [x:y],..*/
    virtual const  QString name(DragName dragname =FULLPATH) = 0;
    /* binds control editValue changed */
    virtual bool bind(const char *slot, QObject *pobj) = 0;
    /* unbinds control from any register or applet*/
    virtual void unbind() =0;
    /* set description (toolTip)*/
    virtual void setDescription(const QString &) = 0;
    /* set caption for control (caption is text near control)*/
    virtual void setCaption(const QString &) =0;
    /* caption text */
    virtual const QString  caption()=0;
    /* reds description of control (toolTip)*/
    virtual const QString  description() = 0;
    /* setup control properties by string script*/
    virtual  void makeSetup(const QString &){}
    /* */
    virtual void setEditValueRange(double ,double ){}
    /* returns true if control is allready binded */
    const bool isBinded(void) const;   
    /* generates register name in format regname[n:y] from normal object name */
    static const QString generateRegisterName(const QString &input);
    /* */
    static bool extractParameter(const QString & name,QString *papplet, QString *pnode, QString *ppar);
    /* */
    static void startDrag(UniversalControl *pcontrol,  quint32 drag_options );

    /* stops drag procedure */
    static void stopDrag(void);
    /* returns control type */
    const CustomControlType controlType() const;

    virtual QByteArray array(){return QByteArray();}
    virtual void setArray(const QByteArray &){}

    /* constructing */
    virtual  void construct(){}
protected:

    /* sets control type */
    void setControlType(CustomControlType type);
    CustomControlType m_type;
    bool m_binded;

    /* names*/
    QString m_appletName;
    QString m_bindName;
    QString m_CachedPath;
    QString m_CachedName;
    QString m_CachedParameter;
    bool m_HasCachedName;
    bool m_already_constructed;
};



/* this is a list with all controls declared as UniversalControl*/
extern QList<UniversalControl *> ControlsList;

}

#endif // UNIVERSALCONTROL_H
