/**
 * @file:applet.h   -
 * @description: This is applpet for creating new windows
 * @project: BENCH OnSemiconductor
 * @date: 2014\11\27 13-52-19
 * 
 */

#ifndef CUSTOMMDICHILD_H
#define CUSTOMMDICHILD_H

class Applet;

#include <QWidget>
#include <QCloseEvent>
#include <QVariant>
#include <QObject>
#include <QAction>
#include <QMdiSubWindow>
#include <QDebug>
#include <QSpinBox>
#include <QFileDialog>
#include <QMessageBox>
#include <QMetaMethod>
#include <QStringBuilder>
#include <QVBoxLayout>
#include "register.h"
#include "variable.h"
#include "customcontrols.h"
#include "exceldata.h"
#include <QTextEdit>
#include <QDockWidget>
#include <QStringBuilder>
#include "parameterparser.h"
#include "applethelper.h"

/* custom controls namespace
*/
using namespace CustomControls;



#define QT_USE_FAST_OPERATOR_PLUS
#define QT_USE_FAST_CONCATENATION



/*
 * main applet class
*/
class Applet : public QWidget
{
    Q_OBJECT
    friend class AppletHelper;
public:

    #define ASSERT(V)           do{if(V==NULL) qFatal("zero ptr");}while(0)
    #define CURSOR_WAIT()       QApplication::setOverrideCursor(Qt::WaitCursor)
    #define CURSOR_RESTORE()    QApplication::restoreOverrideCursor()
    /*
     *This is making logs macros
    */
    #define LOG(CODE,TEXT)      do{emit signal_log(CODE,(TEXT));}while(0)
    #define LERROR(TEXT)        do{emit signal_log(LOG_ERROR,(TEXT));}while(0)
    #define LWARNING(TEXT)      do{emit signal_log(LOG_WARNING,(TEXT));}while(0)

    /*
     * This tool is for describing some external function
     **/
    #define HELP(NAME,DESCR)            if(!m_functions.contains(NAME)) m_functions[(NAME)]=(DESCR);



    #define FUNCTION( NAME, DESCR)      HELP(NAME, DESCR);\
                                        if(QRegExp(NAME,Qt::CaseSensitive,QRegExp::Wildcard).exactMatch(arg_name))

    #define FUNCTION_LIST()         runAppCommand(const QString &arg_name, const QVariant &arg_data)

    #define WILDCARD(MASK,VAR)      QRegExp(MASK,Qt::CaseSensitive,QRegExp::Wildcard).exactMatch(VAR)

    /*
     *Setting and getting refisters for this applet
     *bretter tyo use this functions than call directly Register or Variable.
     *This macro will update ui and register simulatenously
     */

    /* to set data using external interface*/
    #define SET_REG(TXT,V)      setAppData((TXT),(V),RING_INTERNAL);
    /* to get data using external interface*/
    #define GET_REG(TXT)        appData((TXT),RING_INTERNAL)
    /* run function using external interface*/
    #define RUN(TXT,V)          runAppCommand((TXT),(V));

    /* updates register controls and update hardware*/
    #define UPDATE(REG_NAME) do{\
                                slot_updateControls(REG_NAME);\
                                updateHardware(HARDWARE_SET,REG_NAME);\
                             }while(0)


    // update hardware

    #define TO_HARDWARE(TXT)        uploadHardware((TXT));
    #define FROM_HARDWARE(TXT)      downloadHardware((TXT))

    typedef enum
    {
        MDI,/* normal sub window in MainWindow*/
        DOCK/*docking means when it can embed to MainWindow*/
    }Type;

    explicit Applet(QWidget *parent = 0);
    virtual ~Applet();
    QWidget *widget();
    const QString name()const ;
    void setName(const QString &name);
    // [INTERFACE]
    /* SET applet data/registers */
    virtual bool setAppData(const QString &name, const QVariant &data, quint32 ring=RING_EXTERNAL);
    /* GET applet data/register*/
    virtual const QVariant appData(const QString &name, quint32 ring=RING_EXTERNAL);
    /* RUN running commands issued by pushbutton*/
    virtual QVariant runAppCommand(const QString &name, const QVariant &data )=0;
    /* initializes applet*/
    bool initialize_applet();
    /* initializes applet*/
    void resume_applet();
    /* initializes applet*/
    void suspend_applet();
    /* list of all register*/
    QStringList registers_list() const ;
    /* list of all controls*/
    QStringList controls_list() const;
    QStringList functions_list() const;

    // [COMMON REDEFINABLE FUNCTIONS]
    typedef enum {
        HARDWARE_SET, // -> to hardware
        HARDWARE_GET// <- from hardware
    }Direction;

    QStringList m_update_list;
    /* this is ardwar updater function used in setAppData*/
    virtual bool updateHardware(Direction , const QString &) { return true;}
    bool updateHardwareNonBlocking(Direction direction, const QString &item);

    void setEnabled(bool on);
    /* uploads all data to hardware*/
    bool uploadHardware(const QString &list_as_text);
    /* downloads all data from harware*/
    bool downloadHardware(const QString &list_as_text);

    /* for stop/suspend applet window*/
    virtual void suspend() {}
    /* on applet resume */
    virtual void resume() {}
 


    //[used in MAINWINDOW]
    /* not implemented*/
    QAction *appletAction() ;
    /* mdi subwindow pointer*/
    QWidget *createContainerWindow(Type type, QWidget *parent);
    /* container window for MDI MainWindow*/
    QWidget *containerWindow();  
    /**/
    void removeContainerWindowAndAction();


    // [EXPORT/ IMPORT current state]
    /* export all registers to bytearray  */
    virtual const QByteArray saveAppData();
    /* load all registers from bytearray*/
    virtual bool loadAppData(const QByteArray &data);




    // [APPLET OPTIONS]    
    bool m_fixedHeight;
    bool m_fixedWidth;
    bool fixedHeight() const {return m_fixedHeight;}
    bool fixedWidth() const {return m_fixedWidth;}
    void setFixedHeight(bool on) {m_fixedHeight=on;}
    void setFixedWidth(bool on){m_fixedWidth=on;}

    void setShortCut(quint32 shortcut){m_shortcut=shortcut;}
    quint32 shortCut() const {return m_shortcut;}

    /* activate window. bring to front*/
    void open_window();
    /*  deactivate. hide window*/
    void close_window();
    void setTitle(const QString &text);
    const QString title();
    bool isOpened();
    /* window type if MDI or DOCK */
    Type windowType() const;
    /* drag something*/
    static void dragFunction(QWidget *pwidget,const char *namepath, const char *parameter);
    /* when applet not resumed or suspeneded == false*/
    bool isActivated() const ;

protected:    
    /*
     *data type REGISTER is for register and VARIANT for variant
    */
    typedef enum
    {
      NODE_REGISTER,
      NODE_VARIABLE
    }NodeType;

    /* container for Register and its controls */
    typedef struct
    {
      /* type of node variant or register */
      NodeType type;
      /* pointer to register */
      Register *pRegister;
      /* pointer to variant variable . (pRegister or pVariant)*/
      Variable *pVariable;
      /* list of controls related to register of variant */
      QList<UniversalControl*> controls;
    }DataNode;

    /* data map . Registers. Data(QVariant)*/
    typedef QHash<QString,DataNode *> AppDataMap;
    typedef QHash<QString,DataNode *>::const_iterator AppDataMapIterator;

    DataNode * dataNode(const QString &name);

    /* initialize of applet user function */
    virtual bool initialize(){ return true;}

    /* event on close window*/
    void closeEvent(QCloseEvent *pevent);        
    /* add data as register*/
    bool addDataNode(Register *preg, qint32 control_size=-1);
    /* add data as variable*/
    bool addDataNode(Variable *pvar);
    /* add data as register with the given name*/
    bool addDataNode(const QString name, Register *preg);
    /* add data as variable with given name*/
    bool addDataNode(const QString name, Variable *pvar);    
    /*removes datanode by reg*/
    void removeDataNode(Register *preg);
    /*removes datanode by var*/
    void removeDataNode(Variable *pvar);
    /*removes datanode by name*/
    void removeDataNode(const QString &node_name);
    /* binds control to register*/
    void bindControl(UniversalControl *pctrl);
    /* binds all controls to register automatically*/
    void autoBind();
    /* unbind all controls from register with name*/
    void unbindNode(const QString &node_name);
    /* event for F1*/
    void keyPressEvent(QKeyEvent *pev);
    /* returns help text */
    const QString &help() const;
    /* scans applet and opuilates all widgets*/
    void populateControlsList();


    /* pointer to mdi subwindow object */
    QMdiSubWindow *mp_MdiSubWindow;
    /* pointer to dock widget*/
    QDockWidget *mp_DockWidget;
    /* action object pointer<not implemented>*/
    QAction *mp_action;
    /* registers/variables : data list*/
    AppDataMap  m_applet_data;
    /*return true if variable or register is readonly*/
    bool nodeReadOnly(const QString &name);
    /**/
    bool m_activated;
    quint32 m_shortcut;
    /**/
    Type m_window_type;
    QMap<QString,QString> m_functions;
    /* help window */
    AppletHelper *mp_applet_helper;
    /* aplet controls which are CustomControls::...*/
    QList<UniversalControl*> m_applet_controls;
    /* list of all widgets in applet*/
    QList<QWidget*> m_sub_widgets;

    /**/
    void enableControlsByMode(const QString &mode);

    bool m_call_from_ui;
    bool calledFromUi() const {return m_call_from_ui;}

/* SIGNALS / SLOTS */
signals:
    /* signal hide window */
    void signal_HideWindow(void);
    /* signal when some of register is changed . */
    /* Sends signal when at least one register/variable changed*/
    void signal_RegisterChanged(void);
    /* signal for logging . Sends signal to LOG window*/
    void signal_log(int ,QString);

private slots:
    /* slot action of this window is triggered from mainwindow */
    void slot_ActionTriggered();
    /* slot makes window inactive from mainwindow */
    void slot_SetInactiveWindow();
    /* when control triggered */
    void slot_ControlTriggered();
    /* shows help window */
    void slot_showHelp();


public slots:    
    // [VECTOR INTERPRETER]	
    /* transfers vector data this function is called each VectorInterpreter step*/
    virtual void transferVecData(QByteArray &){ }
    /* this function is called on VecInterpereter start */
    virtual void startVecData(){ }
    /* this function is called on VecInterperter stop */
    virtual void stopVecData(){ }

    void slot_updateSet(const QString &name);
    void slot_updateGet(const QString &name);
    /* updates all controls binded to register*/
    void slot_updateControls(const QString &name);
    /* the same as above but by register instance*/
    void slot_updateControls(const Register &reg);
    /* the same as above but by variable instance*/
    void slot_updateControls(const Variable &var);
    /* update all controls of all registers */
    void slot_updateAllControls();
    /* adjusting size to its contens*/
    void slot_adjustSize();
    virtual void slot_capturedData(const QByteArray){}

};

#endif // CUSTOMMDICHILD_H
