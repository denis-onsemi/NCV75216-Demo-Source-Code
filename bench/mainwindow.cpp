
/**
 * @file:mainwindow.cpp   -
 * @description:
 * @project: BENCH OnSemiconductor
 * @date: 2014\11\27 13-47-57
 *
 */


#include "mainwindow.h"
#include "mainapp.h"
#include <QFileDialog>
#include <QMessageBox>
#include "bootloader.h"


class MdiArea : public QMdiArea
{
public:

protected:
    void paintEvent(QPaintEvent *event)
    {
        QMdiArea::paintEvent(event);

        QPainter painter(viewport());
        QFont font;
        double pix_size = ((double)(viewport()->width()/1.5)/sizeof(QMAKE_TARGET))*1.76;
        font.setPixelSize((int)pix_size);
        painter.setPen(QColor(0xa7,0xa7,0xa7));
        painter.setFont(font);
        painter.drawText(viewport()->width()/4,viewport()->height()/2+pix_size/2,QMAKE_TARGET);
        painter.end();
    }
};


/****************************************************************************
 * @function name: CONSTRUCTOR
 *
 * @param:
 *       QWidget *parent
 * @description:
 ****************************************************************************/
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent)
{
    createUi();
    createConnectionBar();
    qApp->installEventFilter(this);
}


/****************************************************************************
 * @function name: DESTRUCTOR
 * @description:
 ****************************************************************************/
MainWindow::~MainWindow()
{
    mp_timer->stop();
}


/****************************************************************************
 * @function name: MainWindow::createUi()
 *
 * @param:
 *             void
 * @description:
 * @return: ( void )
 ****************************************************************************/
void MainWindow::createUi()
{
    setWindowTitle(QString("ON Semiconductor BenchBoard <<< %1 >> %2")
                   .arg(PROJECT_NAME)
                   .arg(__DATE__ "," __TIME__));
    mp_MdiArea = new MdiArea;
    mp_MdiArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    mp_MdiArea->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    mp_timer = new QTimer();
    setCentralWidget(mp_MdiArea);
    setStatusBar(new QStatusBar(this));
    QMenuBar *menubar = new QMenuBar(this);
    setMenuBar(menubar);
    mp_MenuHelp = menubar->addMenu("&Service");
    mp_MenuApplication = menubar->addMenu("&Application");
    mp_MenuApplication->setEnabled(0);
    mp_MenuProject = menubar->addMenu("&Project");
    mp_MenuConfig = menubar->addMenu("&Config");
    /* save load configuration menus*/
#ifdef ProjectApp_Load_Data
    QAction *paction = new QAction(QIcon(":/buttons/excel"), "&Load Project Data", this);
    mp_MenuProject->addAction(paction);
    connect(paction, SIGNAL(triggered()), this, SLOT(slot_LoadProjectData()));
#endif
    QAction *paction1 = new QAction(QIcon(":/buttons/export"), "&Save config", this);
    mp_MenuConfig->addAction(paction1);
    QAction *paction2 = new QAction(QIcon(":/buttons/import"), "&Load config", this);
    mp_MenuConfig->addAction(paction2);
    mp_MenuConfig->addSeparator();
    QAction *about_action = new QAction(QIcon(),"About",this);
    mp_MenuHelp->addAction(about_action);
    mp_updateSoftware = new QAction(QIcon(),"Update Software",this);
    mp_MenuHelp->addAction(mp_updateSoftware);
    mp_updateFirmware = new QAction(QIcon(),"Update Firmware",this);
    mp_MenuHelp->addAction(mp_updateFirmware);        

    mp_neverSleep = new QAction(QIcon(":/buttons/monitor"),"Never sleep",this);
    mp_neverSleep->setCheckable(1);
    connect(mp_neverSleep,SIGNAL(toggled(bool)),this,SLOT(slot_pbDontSleep(bool)));
    mp_MenuHelp->addAction(mp_neverSleep);

    mp_menu = new QMenu();
    QAction *closeAll = mp_menu->addAction("Close All");
    QAction *closeAllButCurrent =  mp_menu->addAction("Close All But Active");
    connect(mp_updateSoftware,SIGNAL(triggered()),this,SLOT(slot_UpdateBenchSoftware()));
    connect(mp_updateFirmware,SIGNAL(triggered()),this,SLOT(slot_UpdateFirmware()));
    connect(paction1, SIGNAL(triggered()), this, SLOT(slot_SaveConfig()));
    connect(paction2, SIGNAL(triggered()), this, SLOT(slot_LoadConfig()));
    connect(about_action, SIGNAL(triggered()), this, SLOT(slot_AboutApp()));
    connect(closeAll,SIGNAL(triggered()),this,SLOT(slot_CloseAll()));
    connect(closeAllButCurrent,SIGNAL(triggered()),this,SLOT(slot_CloseAllButActive()));
    connect(mp_timer,SIGNAL(timeout()),this,SLOT(slot_supress_led()));

    connect(&m_tmr,SIGNAL(timeout()),this,SLOT(slot_timeout()));
    updateConfigBenchList();
}

/****************************************************************************
 * @function name: MainWindow::contextMenuEvent()
 *
 * @param:
 *         QContextMenuEvent *event
 * @description: context menu execution
 * @return: ( void )
 ****************************************************************************/
void MainWindow::contextMenuEvent(QContextMenuEvent *event)
{
    mp_menu->exec(event->globalPos());
}

void MainWindow::slot_RestorePreviousWorkspace()
{
    BENCH()->loadWorkspace();
}

/****************************************************************************
 * @function name: MainWindow::createConnectionBar()
 *
 * @param:    void
 * @description:
 * @return: ( void )
 ****************************************************************************/

void MainWindow::createConnectionBar()
{

    mp_cmbConnectChannel = new QComboBox(0);    
    mp_cmbConnectChannel->setFixedSize(150, 20);
#ifdef USE_CONTROLLER
    mp_cmbConnectionSpeed =new QComboBox(0);
    mp_cmbConnectionSpeed->setFixedSize(100, 20);
    mp_cmbConnectionSpeed->addItems(QStringList()<<"Normal"<<"Turbo");
    mp_cmbConnectionSpeed->setItemData(0,MAINAPP_UART_NORMAL);
    mp_cmbConnectionSpeed->setItemData(1,MAINAPP_UART_TURBO);
#elif USE_MPP
    mp_pbMppRestart =  new QPushButton("Restart");
    mp_pbMppRestart->setFixedHeight(20);
    mp_pbMppShutdown =  new QPushButton("Shutdown");
    mp_pbMppShutdown->setFixedHeight(20);
    connect(mp_pbMppRestart,SIGNAL(clicked()),this,SLOT(slot_pbMppRestart()));
    connect(mp_pbMppShutdown,SIGNAL(clicked()),this,SLOT(slot_pbMppShutdown()));
#endif
    /* connect button*/
    mp_pbConnectBoard = new QPushButton(0);
    mp_pbConnectBoard->setFixedSize(100, 20);
    mp_pbConnectBoard->setCheckable(true);
    /* led0*/
    mp_cswLed0 = new CustomControls::MySwitch(0);
    mp_cswLed0->setReadOnly(true);
    mp_cswLed0->addState("1", ":/leds/img_led_gray");
    mp_cswLed0->addState("2", ":/leds/img_led_green");
    mp_cswLed0->addState("3", ":/leds/img_led_red");
    mp_cswLed0->setFixedSize(20, 20);
    mp_cswLed0->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    /* led1*/
    mp_cswLed1 = new CustomControls::MySwitch(0);
    mp_cswLed1->setReadOnly(true);
    mp_cswLed1->addState("1", ":/leds/img_led_gray");
    mp_cswLed1->addState("2", ":/leds/img_led_green");
    mp_cswLed1->addState("3", ":/leds/img_led_red");
    mp_cswLed1->setFixedSize(20, 20);
    mp_cswLed1->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    /* led2*/
    mp_cswLed2 = new CustomControls::MySwitch(0);
    mp_cswLed2->setReadOnly(true);
    mp_cswLed2->addState("", ":/leds/img_led_gray");
    mp_cswLed2->addState("", ":/leds/img_led_green");
    mp_cswLed2->addState("", ":/leds/img_led_red");
    mp_cswLed2->setFixedSize(20, 20);
    mp_cswLed2->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    mp_pbRescan = new QPushButton(QIcon(":/buttons/refresh"),"");
    mp_pbRescan->setFixedSize(20,20);
    mp_lbMcuVersion = new QLabel();
    mp_lbMcuVersion->setFixedWidth(250);
    mp_cbLogComm = new QCheckBox("Log");
    mp_timer->start(100);
    setStateConnected(DISCONNECTED);
    statusBar()->addWidget(mp_pbConnectBoard);
    statusBar()->addWidget(mp_cmbConnectChannel);
    statusBar()->addWidget(mp_pbRescan);
#ifdef USE_CONTROLLER
    statusBar()->addWidget(mp_cmbConnectionSpeed);
#elif USE_MPP
    statusBar()->addWidget(mp_pbMppRestart);
    statusBar()->addWidget(mp_pbMppShutdown);
#endif
    statusBar()->addWidget(mp_lbMcuVersion);
    mp_lbStatus = new QLabel("Connection");
    statusBar()->addWidget(mp_lbStatus);
    statusBar()->addWidget(mp_cswLed0);
    statusBar()->addWidget(new QLabel(" rx "));
    statusBar()->addWidget(mp_cswLed1);
    statusBar()->addWidget(new QLabel(" tx "));
    statusBar()->addWidget(mp_cswLed2);
    statusBar()->addWidget(mp_cbLogComm);
    QPushButton *pbutton = new QPushButton("Restore last");
    connect(pbutton,SIGNAL(clicked()),this,SLOT(slot_RestorePreviousWorkspace()));
    statusBar()->addWidget( pbutton);
    connect(mp_cbLogComm        ,SIGNAL(toggled(bool))
            ,this               ,SLOT(slot_cbLogComm_toggled(bool)));

    connect(mp_pbConnectBoard   , SIGNAL(clicked())
            , this              , SLOT(slot_pbConnectBoard_click()));

    connect(mp_pbRescan         ,SIGNAL(clicked())
            ,this               ,SLOT(slot_Rescan()));


#ifdef USE_CONTROLLER
    connect(mp_cmbConnectionSpeed,SIGNAL(currentIndexChanged(int)),
            this, SLOT(slot_ChangeConenctionSpeed()));
#endif

    connect(mp_cbLogComm ,SIGNAL(toggled(bool))
            , this, SLOT(slot_LogOn(bool)));

}

/****************************************************************************
 * @function name: MainWindow::eventFilter()
 *
 * @param:
 *        QObject *pobj
 *        QEvent *peventt
 * @description: event filter for key press.
 *               This functionality used to provide keyboard shortcut for applet
 * @return: ( bool )
 ****************************************************************************/
bool MainWindow::eventFilter(QObject *pobj, QEvent *pevent)
{
    if (pevent->type() == QEvent::KeyPress)
    {
        QKeyEvent *pev = static_cast<QKeyEvent *>(pevent);
        if(pev->modifiers() & Qt::ControlModifier)
        {
            if(pev->key() >= Qt::Key_0 && pev->key() <= Qt::Key_9)
            {
                if(MAINAPP->isBoardConnected())
                {
                    Applet *papplet = MAINAPP->findApplet(pev->key(), MainApp::BY_KEY);
                    //if found
                    if(papplet)  {
                        papplet->open_window();
                        mp_MdiArea->setActiveSubWindow(static_cast<QMdiSubWindow*>(papplet->containerWindow()));
                    }
                }
                else {
                    if(!MAINAPP->quietMode())
                        QMessageBox::information(0,"","Connect board before window open.");
                }
            }
        }
    }
    return qApp->eventFilter(pobj,pevent);
}


/****************************************************************************
 * @function name: MainWindow::setChannelsList()
 *
 * @param:
 *        const QStringList &list
 * @description: channels list
 * @return: ( void )
 ****************************************************************************/
void MainWindow::setChannelsList(const QStringList &list)
{
    mp_cmbConnectChannel->clear();
    mp_cmbConnectChannel->addItems(list);
}

/****************************************************************************
 * @function name: MainWindow::addWidget()
 *
 * @param:
 *        QWidget *ppb
 * @description: adds widget to status bar
 * @return: ( void )
 ****************************************************************************/
void MainWindow::addWidget(QWidget *ppb)
{
    statusBar()->addWidget(ppb);
}

/****************************************************************************
 * @function name: MainWindow::activeChild()
 *
 * @param:
 * @description: returns pointer to active applet window
 * @return: ( Applet * )
 ****************************************************************************/
Applet *MainWindow::activeChild()
{
    Applet *pchild = static_cast<Applet *>(mp_MdiArea->activeSubWindow()->widget());
    return pchild;
}

/****************************************************************************
 * @function name: MainWindow::currentChannel()
 *
 * @param:
 *       void
 * @description:
 * @return: ( const QString )
 ****************************************************************************/
const QString MainWindow::currentChannel()
{
    return mp_cmbConnectChannel->currentText();
}

/****************************************************************************
 * @function name: MainWindow::logComm()
 *
 * @param:
 *       void
 * @description:
 * @return: ( bool )
 ****************************************************************************/
bool MainWindow::logComm() const
{
    return mp_cbLogComm->isChecked();
}

void MainWindow::setStateConnected(MainWindow::State state)
{
    const char *state_text[2] = {"Connect","Disconnect"};
    mp_pbConnectBoard->setText(state_text[(int)state]);
    mp_pbConnectBoard->setChecked(state== CONNECTED);
    mp_MenuApplication->setEnabled(state== CONNECTED);
    mp_cmbConnectChannel->setEnabled(state== DISCONNECTED);
    mp_cswLed0->setCurrentState(state== CONNECTED);
    mp_pbRescan->setEnabled(state== DISCONNECTED);
    mp_updateFirmware->setEnabled(state== CONNECTED);
}

void MainWindow::setVersionText(const QString &text)
{
    mp_lbMcuVersion->setText(text);
}

quint32 MainWindow::connectionSpeed()
{
#ifdef USE_CONTROLLER
    return mp_cmbConnectionSpeed->itemData(mp_cmbConnectionSpeed->currentIndex()).toUInt();
#else
    return 0;
#endif
}


/****************************************************************************
 * @function name: MainWindow::addWindow()
 *
 * @param:
 *       MenuCategory category - menu category
 *       Applet *pwindow - applet window pointer
 *       WindowStyle style - window DOCK or MDI
 * @description:
 * @return: ( void )
 ****************************************************************************/
void MainWindow::addWindow(MenuCategory category, Applet *pwindow, WindowStyle style)
{
    if (pwindow == NULL ) return;
    QMenu *pmenu;
    switch (category)
    {

    case MenuHelp:
        pmenu = mp_MenuHelp;
        break;
    case MenuApplication:
        pmenu = mp_MenuApplication;
        break;

    case MenuProject:
        pmenu = mp_MenuProject;
        break;

    default:
        return;
        break;
    }

    if (style == WINDOW_STYLE_DOCK)
    {
        QDockWidget *pdock = static_cast<QDockWidget*>(pwindow->createContainerWindow(Applet::DOCK,this));
        addDockWidget(Qt::RightDockWidgetArea, pdock );
        pmenu->addAction(pwindow->appletAction());
    }
    else
    {
        QMdiSubWindow *p_subwindow = static_cast<QMdiSubWindow*>(pwindow->createContainerWindow(Applet::MDI,this));
        mp_MdiArea->addSubWindow(p_subwindow);
        pmenu->addAction(pwindow->appletAction());
    }
}


/****************************************************************************
 * @function name: MainWindow::closeEvent()
 *
 * @param:
 *       QCloseEvent *ev
 * @description:
 * @return: ( void )
 ****************************************************************************/
void MainWindow::closeEvent(QCloseEvent *ev)
{
    BENCH()->closeBoardConnection();
    ev->accept();
    qApp->quit();
}

/****************************************************************************
 * @function name: MainWindow::slot_LoadProjectData()
 *
 * @param:
 *        void
 * @description:
 * @return: ( void )
 ****************************************************************************/
void MainWindow::slot_LoadProjectData()
{
    //ProjectApp_Load_Data();
}

/****************************************************************************
 * @function name: MainWindow::slot_AboutApp()
 * @param:
 *        void
 * @description:
 * @return: ( void )
 ****************************************************************************/
void MainWindow::slot_AboutApp()
{    
    QMessageBox msg;
    msg.setIcon(QMessageBox::Information);
    msg.setStandardButtons(QMessageBox::Ok);
    msg.setText(QString("<p>PROJECT:<b>%1</b></p>"
                        "<p>VERSION<b> %4</b> from:<b>%2</b></p>"
                        "<p>COMPILATION:<font color=blue> %3</color></p>")
                .arg(QMAKE_TARGET)
                .arg(VERSION_DATE)
                .arg(__DATE__ "  "__TIME__)
                .arg(VERSION)
                );
    msg.setWindowTitle("About");
    QFile changes(":/project/changes.txt");
    QTextEdit txt;
    txt.setFixedHeight(500);
    txt.setFixedWidth(400);
    if(changes.open(QFile::ReadOnly))
    {
        QGridLayout *playout = static_cast<QGridLayout*>(msg.layout());
        txt.setText(changes.readAll());
        playout->addWidget(&txt,playout->rowCount(),0,1,playout->columnCount());
    }
    msg.exec();
}


/****************************************************************************
 * @function name: MainWindow::slot_UpdateBenchSoftware()
 * @param:
 *        void
 * @description:
 * @return: ( void )
 ****************************************************************************/
void MainWindow::slot_UpdateBenchSoftware()
{
    MAINAPP->update();
}

/****************************************************************************
 * @function name: MainWindow::slot_UpdateFirmware()
 * @param:
 *        void
 * @description:
 * @return: ( void )
 ****************************************************************************/
void MainWindow::slot_UpdateFirmware()
{
    MAINAPP->updateFw();
}

/****************************************************************************
 * @function name: MainWindow::slot_CloseAll()
 * @param:
 *        void
 * @description: close all windows
 * @return: ( void )
 ****************************************************************************/
void MainWindow::slot_CloseAll()
{
    BENCH()->closeAllApplets();
}

/****************************************************************************
 * @function name: MainWindow::slot_CloseAllButActive()
 * @param:
 *        void
 * @description: closes all windows(Applets) except active
 * @return: ( void )
 ****************************************************************************/
void MainWindow::slot_CloseAllButActive()
{
    BENCH()->closeAllButActive();
}


/****************************************************************************
 * @function name: MainWindow::slot_cbLogComm_toggled()
 * @param:    bool enable
 * @description: communication logger enable
 * @return: ( void )
 ****************************************************************************/
void MainWindow::slot_cbLogComm_toggled(bool enable)
{
    MAINAPP->setBoardLogMode(enable);
}

void MainWindow::updateConfigBenchList()
{
    //delete old
    while(m_configs.count()){
        mp_MenuConfig->removeAction(m_configs.takeAt(0));
    }

    // find all possiible directories
    QStringList dirs = MAINAPP->cachedDirs();

    // scan all possible directories
    foreach(const QString &dir, dirs)
    {
        QDir currentDir = QDir(dir);
        QStringList bench_config_list = currentDir.entryList(QStringList()<<"*.bench");
        foreach(const QString &item, bench_config_list)
        {
            QAction *paction = new QAction(QString(item).replace(".bench",""),this);
            paction->setToolTip(dir);
            paction->setStatusTip(QString("Configuration file %1 located %2").arg(item).arg(dir));
            m_configs.append(paction);
            connect(paction,SIGNAL(triggered()),this,SLOT(slot_LoadConfigName()));
            mp_MenuConfig->addAction(paction);
        }
    }
}

void MainWindow::slot_ChangeConenctionSpeed()
{
#ifdef USE_CONTROLLER
    MCU.setBaudrate(connectionSpeed());
#endif
}

void MainWindow::slot_LogOn(bool on)
{
    if(on)
        LOGS->open_window();
    else LOGS->close_window();
}

#ifdef USE_MPP
void MainWindow::slot_pbMppShutdown()
{
    if(MAINAPP->board()) MAINAPP->board()->shutdown();
}

void MainWindow::slot_pbMppRestart()
{
    if(MAINAPP->board()) MAINAPP->board()->restart();
}
#endif

/****************************************************************************
 * @function name: MainWindow::slot_Connection_LED()
 * @param:   void
 * @description:
 * @return: ( void )
 ****************************************************************************/
void MainWindow::slot_Connection_LED(bool on)
{
    mp_cswLed0->setCurrentState(on);
}

void MainWindow::slot_stream(bool on)
{
    static QString prev_text;
    static int prev_state;
    if(on){
        prev_text = mp_lbStatus->text();
        prev_state =  mp_cswLed0->state();
        mp_lbStatus->setText("Stream...");
        mp_cswLed0->setCurrentState(2);
    }
    else {
        mp_lbStatus->setText(prev_text);
        mp_cswLed0->setCurrentState(prev_state);
    }
}


/****************************************************************************
 * @function name: MainWindow::slot_ActivateRx_LED()
 *
 * @param:
 *       bool state
 * @description:
 * @return: ( void )
 ****************************************************************************/
void MainWindow::slot_ActivityRx_LED()
{
    mp_cswLed1->setCurrentState(1);
}


/****************************************************************************
 * @function name: MainWindow::slot_ActivateTx_LED()
 *
 * @param:
 *       bool state
 * @description:
 * @return: ( void )
 ****************************************************************************/
void MainWindow::slot_ActivityTx_LED()
{
    mp_cswLed2->setCurrentState(1);
}

/****************************************************************************
 * @function name: MainWindow::slot_SaveConfig()
 *
 * @param:   void
 * @description:
 * @return: ( void )
 ****************************************************************************/
void MainWindow::slot_SaveConfig()
{
    MAINAPP->saveSettings();
    updateConfigBenchList();
}

/****************************************************************************
 * @function name: MainWindow::slot_LoadConfig()
 * @description:
 * @return: ( void )
 ****************************************************************************/
void MainWindow::slot_LoadConfig()
{
    MAINAPP->loadSettings();
}

void MainWindow::slot_LoadConfigName()
{
    QAction *paction = static_cast<QAction*>(QObject::sender());
    MAINAPP->loadSettings(QString("%1\\%2.bench").arg(paction->toolTip()).arg(paction->text()));
}

/****************************************************************************
 * @function name: MainWindow::slot_supress_led()
 * @param:  this function suppress led(turn off)
 * @description:
 * @return: ( void )
 ****************************************************************************/
void MainWindow::slot_supress_led()
{
    static quint32 led_queue =0;
    if((++led_queue)%2 == 0)
    {
        if( mp_cswLed1->state() !=0 )  mp_cswLed1->setCurrentState(0);
    }
    else
    {
        if( mp_cswLed2->state() !=0 )  mp_cswLed2->setCurrentState(0);
    }
}


/****************************************************************************
 * @function name: MainWindow::slot_ConnectBoard()
 * @param:
 *       bool on
 * @description:
 * @return: ( void )
 ****************************************************************************/
void MainWindow::slot_pbConnectBoard_click()
{
    bool connected = false;
    if (mp_pbConnectBoard->isChecked())
    {
        QString channel_name = mp_cmbConnectChannel->currentText();
        connected = MAINAPP->openBoardConnection(channel_name);
    }
    else
    {
        MAINAPP->closeBoardConnection();
        connected = false;
    }
    setStateConnected((MainWindow::State)connected);

}

/****************************************************************************
 * @function name: MainWindow::slot_Rescan()
 * @param:  this function issues rescanning
 * @description:
 * @return: ( void )
 ***************************************************************************/
void MainWindow::slot_Rescan()
{
    CURSOR_WAIT();
    mp_cmbConnectChannel->clear();
    MAINAPP->initBoardConnection();
    CURSOR_RESTORE();
}

void MainWindow::slot_pbDontSleep(bool on)
{
    if(on){
        m_tmr.start(200/*sec*/*1000);
        MAINAPP->PreventSleep(1);
    }
    else {
        m_tmr.stop();
        MAINAPP->PreventSleep(0);
    }
}

void MainWindow::slot_timeout(){
    MAINAPP->PreventSleep(1);
}
