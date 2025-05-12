/**
 * @file:mainapp.cpp   -
 * @description: Main application class for BenchBoard software
 * @project: BENCH OnSemiconductor
 * @date: 2014\11\27 13-38-18
 *
 */


#include "mainapp.h"
#ifdef ACTIVEX
#include "axconnector.h"
#endif
#include "mainwindow.h"
#include "customdialogs.h"
#include <QCheckBox>
#include <QDialogButtonBox>
#include <QFileDialog>
#include <QListWidget>
#include <QInputDialog>
#include <QSettings>
#include <QProcess>
#include <QDesktopWidget>

#include "project_app.h"        // project specific


/* command format indexes ClassName.RegisterName*/
const int CLASS_NAME = 0;
const int PROPERTY_NAME = 1;
/* default updater path*/
const char DEFAULT_UPDATER_PATH[] = "\\\\CZBRSMB1\\labo\\SW\\#updates#\\Updater.exe";
/* */
const char DEFAULT_PYTHON_PATH[] = "c:\\Python27\\python.exe";
/* options filename */
const char DEFAULT_OPTIONS_FILE_NAME[] = QMAKE_TARGET ".ini";
/* global pointer to this class*/
MainApp  *g_BenchApplication;
/* FTDI channels pointer list*/
#ifdef USE_FTDI
FtdiSpiChannel *g_FtdiChannel1 = NULL;
FtdiSpiChannel *g_FtdiChannel2 = NULL;
#else

#endif
/* progressbar dialog form*/
DialogProgressBar *pdialog;
/* applets list */
Applet * MainApp::m_AppList[] = {0};
int MainApp::m_AppListCount =0;

static QSettings config_file(QString("%1\\%2").arg(QDir::currentPath()).arg(QMAKE_TARGET ".ini").toLower(),QSettings::IniFormat);





MainApp::MainApp()
{
    m_AppListCount=0;
#ifdef USE_CONTROLLER
    mp_fwbldr=NULL;
#endif
    pdialog = new DialogProgressBar(0, QPixmap("ON_Logo.png"));

}

/****************************************************************************
 * @function name:  MainApp::initialize()
 *
 * @param: void
 * @description: This function initializes modules
 * @return: (void)
 ****************************************************************************/
bool MainApp::initialize()
{    
    bool result;

    TRACE("MainApp Initialize...");
    try{
#ifdef ACTIVEX
        TRACE("ActiveX Start...");
        /* activex connection */
        static Bench appobject(0);
        /* starts activex server */
        QAxFactory::startServer();
        /* registers activex ovject */
        QAxFactory::registerActiveObject(&appobject);
#endif        
        pdialog->show();
        pdialog->setProgress(0, 100);
        pdialog->setText("Initializing ...");
        pdialog->setProgress(10, 100);
        mp_local_connector = new LocalServer();
        if(! mp_local_connector->initialize(PROJECT_NAME) ) return false;
        mp_MainWindow = new MainWindow(0);
        QPoint window_pos = QApplication::desktop()->screen(1)->rect().center();
        window_pos.setX(200);
        window_pos.setY(window_pos.y()- mp_MainWindow->rect().height());
        mp_MainWindow->move( window_pos);
        REGISTER_APPLET(Logs);
        REGISTER_APPLET(Settings);

#ifdef USE_VEC_INTERPRETER
        TRACE("Use VectorInterpreter");
        REGISTER_APPLET( VecInterpreter);
#endif    

#ifdef USE_SCANCHAIN
        TRACE("Use Scanchain");
        REGISTER_APPLET( Scanchain);
#endif

        TRACE("Register specific");

#ifdef PROJECT_TYPES
        DialogCheckBoxes dialog_choice(DialogCheckBoxes::RADIOBUTTON);
        dialog_choice.inflate(projectTypeList());
        dialog_choice.exec();
        dialog_choice.setWindowTitle("Project type");
        QStringList list ;
        dialog_choice.readStringList(list);
        setProjectType(list[0]);
#endif

        // initialize from project app
        SPECIFIC_INIT;
        LOGS->initialize_applet();
        pdialog->setProgress(20, 100);
        qint32 applet_shortcut = Qt::Key_0;
        for (qint32 i = 0; i < m_AppListCount; i++)
        {
            if ( m_AppList[i] != NULL)
            {
                pdialog->setText(m_AppList[i]->name());
                try
                {

                    TRACE("init %i %s",i,(m_AppList[i]->name().toLatin1().constData()));
                    result = m_AppList[i]->initialize_applet();
                    if(m_AppList[i]->windowType() == Applet::MDI)
                    {
                        m_AppList[i]->setShortCut(applet_shortcut);
                        m_AppList[i]->setTitle( QString("(Ctrl+%1) ").arg(applet_shortcut-Qt::Key_0)
                                                % m_AppList[i]->title());
                        applet_shortcut++;
                    }
                    if(!result) {TRACE("Error");break;}
                    else {TRACE("Ok");}
                }
                catch (...)
                {

                }
            }
            pdialog->setProgress(pdialog->progress() + i, 100);
        }
        /* connect log for mainapp class*/
        connect(this, SIGNAL(signal_log(int,QString))
                , gp_Logs->widget(), SLOT(slot_addLog(int, QString)));

#ifdef USE_FTDI
        TRACE("Using FTDI");
        g_FtdiChannel1  = new FtdiSpiChannel();
        g_FtdiChannel2  = new FtdiSpiChannel();
        QObject::connect(g_FtdiChannel2,SIGNAL(signal_StateChanged(bool)),mp_MainWindow,SLOT(slot_Connection_LED(bool)));

#elif USE_CONTROLLER
        TRACE("Using Controller OnMcu ");
        mp_McuProtocol = new ControlBoard();
        connect(this->mp_McuProtocol, SIGNAL(signal_log(int,QString))
                ,gp_Logs->widget(),SLOT(slot_addLog(int,QString)));
        connect(this->mp_McuProtocol, SIGNAL(signal_tx())
                ,mp_MainWindow,SLOT(slot_ActivityTx_LED()));
        connect(this->mp_McuProtocol, SIGNAL(signal_rx())
                ,mp_MainWindow,SLOT(slot_ActivityRx_LED()));
        connect(this->mp_McuProtocol, SIGNAL(signal_stream(bool))
                ,mp_MainWindow,SLOT(slot_stream(bool)));
        connect(mp_McuProtocol,SIGNAL(signal_disconnected())
                ,this,SLOT(slot_disconnected()));
        connect(mp_McuProtocol,SIGNAL(signal_connected())
                ,this,SLOT(slot_connected()));
#elif USE_MPP
        TRACE("Using MPP ");
        mp_MppBoard = new MppBoard();
        connect(this->mp_MppBoard, SIGNAL(signal_log(int,QString))
                ,gp_Logs->widget(),SLOT(slot_addLog(int,QString)));
        connect(this->mp_MppBoard, SIGNAL(signal_tx())
                ,mp_MainWindow,SLOT(slot_ActivityTx_LED()));
        connect(this->mp_MppBoard, SIGNAL(signal_rx())
                ,mp_MainWindow,SLOT(slot_ActivityRx_LED()));
        connect(this->mp_MppBoard, SIGNAL(signal_stream(bool))
                ,mp_MainWindow,SLOT(slot_stream(bool)));
        connect(mp_MppBoard,SIGNAL(signal_disconnected())
                ,this,SLOT(slot_disconnected()));
        connect(mp_MppBoard,SIGNAL(signal_connected())
                ,this,SLOT(slot_connected()));
#endif
        /* finally project specific initialization*/
    }
    catch(...)
    {
        TRACE("Error initialize applets");
    }

    return result;
}


/****************************************************************************
 * @function name: MainApp::start()
 *
 * @param:  void
 * @description: This function starts application.
 *              Creates mdi ,applets, channels initialisation
 * @return: ( void )
 ****************************************************************************/
void MainApp::start()
{
    TRACE("MainApp Starting...");
    MainWindow::WindowStyle style;
    pdialog->setText("Starting mainwindow");
    pdialog->setProgress(90, 100);
    MainWindow::MenuCategory menu_cat  ;
    for (qint32 i = 0; i < m_AppListCount; i++)
    {
        pdialog->setProgress(pdialog->progress() + i, 100);
        if(
                m_AppList[i] == gp_Logs
        #ifdef USE_VEC_INTERPRETER
                || m_AppList[i]== gp_VecInterpreter
        #endif
                )

        {
            menu_cat = MainWindow::MenuProject;
            style = MainWindow::WINDOW_STYLE_DOCK;
        }
        else if(m_AppList[i] == gp_Settings){
            menu_cat = MainWindow::MenuProject;
            style = MainWindow::WINDOW_STYLE_MDI;
        }
        else {

            menu_cat = MainWindow::MenuApplication;
            style = MainWindow::WINDOW_STYLE_MDI;
        }

        mp_MainWindow->addWindow(menu_cat, m_AppList[i], style);
    }
    //init communication channel connection
    pdialog->setText("Initializing channel connection...");
    TRACE("Add applets to MainWindow...\nInitialize Connection...");    
    initBoardConnection();
    pdialog->hide();
    delete pdialog;
    mp_MainWindow->show();
#ifdef ProjectApp_Start
    ProjectApp_Start();
#endif
}


/****************************************************************************
 * @function name: MainApp::option()
 * @param:
 *         const QString &name
 * @description: main app options
 * @return: (const QVariant)
 ****************************************************************************/
const QVariant MainApp::mainAppConfig(const QString &name)
{    
    QVariant default_value;

    if(name=="comm_timeout"){
        default_value = 50;
    }
    else if(name =="updater_path"){
        default_value = DEFAULT_UPDATER_PATH;
    }
    else if(name == "python_path"){
        default_value = "c:/Python27/python.exe";
    }

    return config_file.value(name,default_value);
}


/****************************************************************************
 * @function name: MainApp::setOption()
 * @param:
 *         const QString &name
 *         const QVariant &value
 * @description: main app options
 * @return: (void)
 ****************************************************************************/
void MainApp::setMainAppConfig(const QString &name, const QVariant &value)
{
    config_file.setValue(name,value);
}


/****************************************************************************
 * @function name: MainApp::resumeAllApplets()
 * @description: resumes all applets all applets. resume means start applet
 * @return: (void)
 ****************************************************************************/

void MainApp::resumeAllApplets()
{
    for (int i = 0; i < m_AppListCount; i++)
    {
        if (m_AppList[i])
        {
            TRACE("Resume:%s",m_AppList[i]->name().toLatin1().constData());
            m_AppList[i]->resume_applet();
        }
    }
}

/****************************************************************************
 * @function name: MainApp::suspendAllApplets()
 * @description: suspends all applets. stops applet for example
                    if it has thread or timer it will stop them
 * @return: (void)
 ****************************************************************************/
void MainApp::suspendAllApplets()
{
    if(isBoardConnected())
    {
        for (int i = 0; i < m_AppListCount; i++)
        {
            if (m_AppList[i] != NULL && m_AppList[i]->name()!="Logs" && m_AppList[i]->isActivated())
            {
                TRACE("Suspend:%s",m_AppList[i]->name().toLatin1().constData());
                m_AppList[i]->suspend_applet();
                m_AppList[i]->close_window();
            }
        }
    }
}

/****************************************************************************
 * @function name: MainApp::initChannelConnection()
 * @description: initializes channel connection
 * @return: (bool )
 ****************************************************************************/
bool MainApp::initBoardConnection()
{
#ifdef USE_FTDI
    mp_MainWindow ->setChannelsList(QStringList() << "FTDI");
#elif USE_CONTROLLER
    QStringList ports;
    mp_McuProtocol->availablePorts(&ports);
    QStringList ports_with_mcu;
    mp_McuProtocol->scanDevices(ports,&ports_with_mcu);
    mp_MainWindow->setChannelsList(ports_with_mcu);
#elif USE_MPP
    QStringList bb_list;
    mp_MppBoard->availablePorts(&bb_list);
    mp_MainWindow->setChannelsList(bb_list);
#endif
    return true;
}

/****************************************************************************
 * @function name: MainApp::board()
 *
 * @param:
 *             void
 * @description: returns text protocol pointer
 * @return: (TextProtocol * )
 ****************************************************************************/
#ifdef USE_FTDI

#elif USE_MPP
MppBoard *MainApp::board() const
{
    return mp_MppBoard;
}

#elif USE_CONTROLLER
ControlBoard *MainApp::board() const
{
    return mp_McuProtocol;
}
#endif

void MainApp::setBoardLogMode(bool enable)
{
#ifdef USE_FTDI
    g_FtdiChannel1->setLogMode(enable);
    g_FtdiChannel2->setLogMode(enable);
#else
    board()->setLogMode(enable);
#endif
    this->localServer()->setLogMode(enable);
}

bool MainApp::boardLogMode() const
{
#ifdef USE_FTDI
    return g_FtdiChannel1->logMode();
#else
    return board()->logMode();
#endif
}

/****************************************************************************
 * @function name: MainApp::mainappSet()
 *
 * @param:
 *      const QString &name
 *      const QVariant &value
 * @description:
 * @return: (bool )
 ****************************************************************************/
void MainApp::mainappSet(const QString &, const QVariant &)
{

}
/****************************************************************************
 * @function name: MainApp::protocolGet()
 *
 * @param:
 *      const QString &name
 *      QVariant *pvalue
 * @description:
 * @return: (bool)
 ****************************************************************************/
void MainApp::mainappGet(const QString &name, QVariant *pvalue)
{    
    if(name.toLower() == "version")
    {
        if(pvalue) *pvalue = VERSION_DATE;
        *pvalue = true;
    }
}

/****************************************************************************
 * @function name: MainApp::mainappRun()
 *
 * @param:
 *      const QString &name
 *      const QVariant &value
 * @description:
 * @return: (bool)
 ****************************************************************************/
void MainApp::mainappRun(const QString &name, const QVariant &value,QVariant *presult)
{
    if(name.toLower() == "shutdown")
    {
        shutdown();
        *presult = true;
    }
    else if(name.toLower() == "close")
    {
        closeBoardConnection();
        *presult = true;
    }
    else if(name.toLower() == "open")
    {
        openBoardConnection(value.toString());
        *presult = true;
    }
    else if(name.toLower() == "set_timeout")
    {

#ifdef USE_FTDI

#elif USE_CONTROLLER
        MCU.setTimeout(value.toInt());
#endif
        *presult = true;
    }
    else if(name.toLower() == "write" && isBoardConnected())
    {
#ifdef USE_CONTROLLER
        QByteArray data;
        MCU.transferData(value.toByteArray(),&data);
        *presult = data;
#endif
    }
}

/****************************************************************************
 * @function name: MainApp::runScript()
 *
 * @param:
 *      const QString &filename
 *      const QByteArray &parameter
 *      QByteArray *presult
 * @description:
 * @return: (bool)
 ****************************************************************************/
bool MainApp::runScript(const QString &filename, const QByteArray &parameter, QByteArray *presult)
{
    static QProcess prc;
    prc.start(MAINAPP->mainAppConfig("python_path").toString(), QStringList()<<filename<<QString(parameter));
    prc.waitForFinished();
    if(presult)
    {
        *presult = prc.readAllStandardOutput();
        presult->replace("\n","");
        presult->replace("\r","");
    }
    return true;
}

LocalServer *MainApp::localServer() const
{
    return mp_local_connector;
}


/****************************************************************************
 * @function name:  MainApp::isBoardConnected()
 *
 * @param:
 *             void
 * @description: if board is connected
 * @return: (bool )
 ****************************************************************************/
bool MainApp::isBoardConnected()
{
    bool result ;
#ifdef USE_FTDI
    result = (g_FtdiChannel1 && g_FtdiChannel2
              && g_FtdiChannel1->isOpened() && g_FtdiChannel2->isOpened() );

#elif USE_MPP
    result = mp_MppBoard->isConnected();
#elif USE_CONTROLLER
    result = mp_McuProtocol->isConnected();
#endif
    return result;
}

/****************************************************************************
 * @function name: MainApp::openBoard()
 *
 * @param:
 *             const QString &name
 * @description: opens channel connection
 * @return: (bool )
 ****************************************************************************/
bool MainApp::openBoardConnection(const QString &channel_name)
{
    bool result = false;
    CURSOR_WAIT();
    // open two channels FTDI SPI
#ifdef USE_FTDI

    g_FtdiChannel1->enableAutoConnector(true);
    g_FtdiChannel2->enableAutoConnector(true);
    QStringList list;
    FtdiSpiChannel::enumDevices(&list);
    qint32 latency = mainAppConfig("timeout").toUInt()>10 ?mainAppConfig("timeout").toUInt():200;
    qint32 timeoutTX = mainAppConfig("timeoutTX").toUInt()>10 ?mainAppConfig("timeoutTX").toUInt():50000;
    qint32 timeoutRX = mainAppConfig("timeoutRX").toUInt()>10 ?mainAppConfig("timeoutRX").toUInt():50000;
    g_FtdiChannel1->setTimeouts(latency,timeoutTX,timeoutRX);
    g_FtdiChannel2->setTimeouts(latency,timeoutTX,timeoutRX);
    if ( list.size() > 2 )
    {
        DialogCheckBoxes dialog;
        dialog.setWindowTitle("Available FTDI channels...");
        dialog.inflate(list);
        dialog.exec();
        QStringList selection;
        dialog.readStringList(selection);//reading back
        if ( selection.size() == 2 )
        {
            result = g_FtdiChannel1->open(list.indexOf(selection.at(0)))
                    && g_FtdiChannel2->open(list.indexOf(selection.at(1)));
        }
    }
    else
    {
        result = g_FtdiChannel1->open(0) && g_FtdiChannel2->open(1);
    }
    if (result )
    {
        g_FtdiChannel1->setFrequency(1.0f);
        g_FtdiChannel2->setFrequency(1.0f);
        resumeAllApplets();
    }
    else
    {
        QMessageBox::warning(0,"Connection","Can't establish connection with FTDI chip!",QMessageBox::Ok);
    }
#elif USE_MPP
    result = MPP.connectDevice(channel_name, true);    
    MPP.setTimeout(SETTINGS->setting("CommTimeout").toUInt());
#elif USE_CONTROLLER
    result = MCU.connectDevice(channel_name, true);
    MCU.setBaudrate(mainWindow()->connectionSpeed());
#endif
    CURSOR_RESTORE();

    return result;
}

/****************************************************************************
 * @function name:  MainApp::closeBoardConnection(void)
 *
 * @param:
 *             void
 * @description: closes channel connection
 * @return: (void )
 ****************************************************************************/
void MainApp::closeBoardConnection(void)
{    
    CURSOR_WAIT();
    if(isBoardConnected()) saveWorkspace();
#ifdef USE_FTDI
    suspendAllApplets();
    g_FtdiChannel1->close();
    g_FtdiChannel2->close();

#elif USE_MPP
    suspendAllApplets();
    mp_MppBoard->disconnectDevice();

#elif USE_CONTROLLER
    suspendAllApplets();
    mp_McuProtocol->disconnectDevice(ControlBoard::USER_COMMAND);
#endif
    CURSOR_RESTORE();
}

/****************************************************************************
 * @function name:  MainApp::shutdown()
 *
 * @param:
 *             void
 * @description: this function makes shutdown
 * @return: ( void )
 ****************************************************************************/
void MainApp::shutdown()
{
    closeBoardConnection();

#ifdef ACTIVEX
    /* stops Activex server after application close*/
    QAxFactory::stopServer();
#endif
    try
    {
        for (int i = 0; i < m_AppListCount; i++)
        {
            delete m_AppList[i];
        }
    }
    catch (...)    {    }
#ifdef USE_FTDI
    if (g_FtdiChannel1)
    {
        g_FtdiChannel1->close();
        delete g_FtdiChannel1;
        g_FtdiChannel1 = NULL;
    }
    if (g_FtdiChannel2)
    {
        g_FtdiChannel2->close();
        delete g_FtdiChannel2;
        g_FtdiChannel2 = NULL;
    }
#elif USE_MPP
    delete mp_MppBoard;
#elif USE_CONTROLLER
    delete mp_McuProtocol;
#endif

}


/****************************************************************************
 * @function name:  MainApp::update()
 *
 * @param:
 *             void
 * @description: opens update window
 * @return: ( void )
 ****************************************************************************/
void MainApp::update()
{
    QString updater_path =mainAppConfig("updater_path").toString();
    while(!QFile::exists(updater_path))
    {
        QInputDialog dialog;
        dialog.setLabelText("Enter updater path:");
        dialog.setTextValue(DEFAULT_UPDATER_PATH);
        if(dialog.exec() == QDialog::Rejected) break;
        updater_path = dialog.textValue();
    }
    QString run = QString("\"%1\" %2 %3")
            .arg( updater_path )
            .arg( PROJECT_NAME )
            .arg( QDir::currentPath() );
    if(!QFile::exists(updater_path) || QProcess::startDetached(run) == false )
    {
        QMessageBox::warning(0,
                             QString(PROJECT_NAME),
                             "Error starting updater. \n Updater is not installed!",
                             QMessageBox::Ok);
    }
    else{
        setMainAppConfig("updater_path",updater_path);
    }
}

void MainApp::updateFw()
{
#ifdef USE_CONTROLLER
    if(mp_fwbldr==NULL)
        mp_fwbldr = new Bootloader();
    mp_fwbldr->show();
#endif
}



/****************************************************************************
 * @function name:   MainApp::set()
 *
 * @param:
 *       QString &name
 *       QVariant &value
 * @description: This function finds module(Applet) and sets data
 * @return: ( bool )
 ****************************************************************************/
bool MainApp::set(const QString &name, QVariant &value)
{
    bool result = false;

    QStringList l = name.split('.');
    if ( l.size() == 2)
    {
        Applet *papplet = findApplet(l.at(CLASS_NAME),MainApp::BY_NAME);
        if ( papplet && isBoardConnected())
        {
            result = papplet->setAppData(l.at(PROPERTY_NAME), value);
        }
        else
        {
            LERROR(QString("Error SET:%1").arg(name));
        }
    }
    else{
        mainappSet(name,value);
        result = true;
    }
    return result;
}

/****************************************************************************
 * @function name:  MainApp::get()
 *
 * @param:
 *        QString &name
 * @description: This function finds module(Applet) and gets data from it
 * @return: ( QVariant )
 ****************************************************************************/
bool MainApp::get(const QString &name, QVariant *pdata_result)
{
    bool result = false;
    QStringList l = name.split('.');
    if ( l.size() == 2)
    {
        Applet *papplet = findApplet(l.at(CLASS_NAME),BY_NAME);
        if ( papplet && isBoardConnected())
        {
            *pdata_result = papplet->appData(l.at(PROPERTY_NAME));
            result = !pdata_result->isNull();
        }
        else
        {
            LERROR(QString("Error GET %1").arg(name));
        }
    }
    else {
        mainappGet(name,pdata_result);
        result = true;
    }
    return result;
}


/****************************************************************************
 * @function name:  MainApp::run()
 *
 * @param:
 *        QString &name
 *        QVariant &value
 * @description: This function finds module(Applet) and runs command
 * @return: ( bool )
 ****************************************************************************/
bool MainApp::run(const QString &name, QVariant &value, QVariant *pdata_result)
{
    bool result= false;
    QStringList l = name.split('.');
    if ( l.size() == 2)
    {
        Applet *papplet = findApplet(l.at(CLASS_NAME),BY_NAME);
        if ( papplet && isBoardConnected())
        {
            *pdata_result = papplet->runAppCommand(l.at(PROPERTY_NAME), value);
            result=true;
        }
        else
        {
            LERROR(QString("Error RUN:%1").arg(name));
        }
    }
    else{
        mainappRun(name,value,pdata_result);
        result =true;
    }
    return result;
}




/****************************************************************************
 * @function name:  MainApp::loadSettings()
 *
 * @param:
 *
 *       void
 * @description: Loads settings to application applets
 * @return: ( void )
 ****************************************************************************/
void MainApp::loadSettings(QString filename)
{
    const int MAX_BYTES = 1000;
    if(filename.isEmpty())
        filename = QFileDialog::getOpenFileName(0, "Load workspace",
                                                "",
                                                "Files (*.bench)");
    if ( filename.isEmpty() )
    {
        return;
    }



    QFile file(filename);
    if ( file.open(QFile::ReadOnly) )
    {
        char data[MAX_BYTES] = {0};
        qint32 bytes = 0;
        Applet *papplet = NULL;
        bool in_brackets = false;
        QByteArray line;
        QByteArray buffer;
        while ( (bytes = file.readLine(data, MAX_BYTES)) > 0 )
        {
            line = QByteArray(data, bytes - 1);
            if (line == "{" )
            {
                in_brackets = true;
                buffer.clear();
            }
            else if (line == "}" && in_brackets)
            {
                if (papplet)
                {
                    papplet->loadAppData(buffer);
                }
                in_brackets = false;
                buffer.clear();
            }
            else if ( !in_brackets )
            {
                papplet = findApplet(QString(line),BY_NAME);
                buffer.clear();
            }
            else if (in_brackets)
            {
                buffer += QByteArray(data);
            }
        }
        file.close();
    }
}

/****************************************************************************
 * @function name: MainApp::findApplet()
 *
 * @param:
 *       const QVariant &parameter
 *       AppletFind find
 *                  BY_NAME,
 *                  BY_KEY,
 *                  BY_ID
 * @description: finds applet in the list
 * @return: ( Applet *) - if no applet found - return NULL
 ****************************************************************************/
Applet * MainApp::findApplet(const QVariant &parameter, AppletFind find)
{
    for (qint32 i = 0; i < m_AppListCount; i++)
    {
        Applet *papplet = m_AppList[i];
        if ( papplet == NULL) continue;

        switch(find)
        {

        case(BY_NAME):
        {
            if (  papplet->name() == parameter.toString())
            {
                return papplet;
            }
        }
            break;

        case(BY_ID):
        {
            return papplet;
        }
            break;

        case(BY_KEY):
        {
            quint32 key = papplet->shortCut();
            if(key == parameter.toUInt())
            {
                return papplet;
            }
        }
            break;
        }
    }
    return NULL;
}

/****************************************************************************
 * @function name: MainApp::log_applet()
 *
 * @param:
 *       void
 * @description: returns Log aplet pointer
 * @return: ( Applet * )
 ****************************************************************************/
Applet *MainApp::log_applet()
{
    return gp_Logs;
}

/****************************************************************************
 * @function name: MainApp::appletsCount()
 *
 * @param:
 *       void
 * @description: returns applets count
 * @return: ( quint32 )
 ****************************************************************************/
qint32 MainApp::appletsCount()
{
    return m_AppListCount;
}

/****************************************************************************
 * @function name: MainApp::closeAllAplets()
 *
 * @param:
 *       void
 * @description:
 * @return: ( void )
 ****************************************************************************/
void MainApp::closeAllApplets()
{
    for(qint32 i=0;i<appletsCount();i++)
    {
        if(m_AppList[i] ) m_AppList[i]->close_window();
    }
}

void MainApp::closeAllButActive()
{
    for(qint32 i=0;i<appletsCount();i++)
    {
        if(m_AppList[i]  && mp_MainWindow->activeChild() != m_AppList[i]) m_AppList[i]->close_window();
    }
}

/****************************************************************************
 * @function name: MainApp::saveSettings()
 *
 * @param:
 *       void
 * @description: saves data from application applets.
 * @return: ( void )
 ****************************************************************************/
void MainApp::saveSettings(void)
{
    DialogCheckBoxes dialog;
    QStringList list;
    for (qint32 i = 0; i <  m_AppListCount; i++)
    {
        Applet *papplet = m_AppList[i];
        if ( papplet && !papplet->name().isEmpty())
        {
            list.append(papplet->name());
        }
    }



    dialog.setWindowTitle("Select windows save...");
    dialog.inflate(list);// populate listbox
    dialog.exec();
    list.clear();
    dialog.readStringList(list);//reading back

    if ( (dialog.result() == QDialog::Accepted) && list.count())
    {
        QString filename = QFileDialog::getSaveFileName(0, "Save workspace",
                                                        "",
                                                        "Files (*.bench)");
        if (filename.isEmpty())
        {
            return;
        }


        putDirToCache(QFileInfo(filename).path());

        QFile file(filename);
        if ( file.open(QFile::WriteOnly) )
        {
            for (qint32 i = 0; i < m_AppListCount; i++)
            {
                Applet *papplet = m_AppList[i];
                if ( papplet  && (list.indexOf(papplet->name()) >= 0) )
                {
                    QByteArray data = papplet->saveAppData();
                    file.write(papplet->name().toLatin1());//name
                    file.write("\n{\n" + data + "\n}\n");//data
                }
            }
            file.close();
        }
    }
}

QStringList MainApp::cachedDirs()
{
    QStringList dirs;
    dirs.append(QDir::currentPath());
    int i=0;
    while(!mainAppConfig(QString("_Path%1").arg(i)).isNull()) {
        dirs.append(MAINAPP->mainAppConfig(QString("_Path%1").arg(i)).toString());
        i++;
    }
    return dirs;
}

void MainApp::putDirToCache(const QString &dir)
{
    //check path exists
    QStringList l = cachedDirs();
    if(!l.contains(dir))
    {
        // add path
        int i=0;
        while(!mainAppConfig(QString("_Path%1").arg(i)).isNull()) i++;
        setMainAppConfig(QString("_Path%1").arg(i),  dir);
    }
}


/****************************************************************************
 * @function name: MainApp::mainWindow()
 *
 * @param:
 *
 *       void
 * @description:
 *
 * @return: ( MainWindow * )
 ****************************************************************************/
MainWindow *MainApp::mainWindow()
{
    return mp_MainWindow;
}

/****************************************************************************
 * @function name: static MainApp::getInstance()
 *
 * @param:
 *
 *       void
 * @description:
 *
 * @return: ( MainApp *)
 ****************************************************************************/
MainApp *MainApp::getInstance()
{
    static MainApp *instance = NULL;
    if (instance == NULL)
    {
        instance = new MainApp;
    }
    return instance;
}


/****************************************************************************
 * @function name: MainApp::saveAppletStates()
 *
 * @param:
 *
 *       QByteArray &state - save state of windows to string
 * @description:
 * @return: ( void )
 ****************************************************************************/

void MainApp::saveWorkspace()
{
    QByteArray state;
    QFile state_settings(QMAKE_TARGET".state");
    if(state_settings.open(QFile::WriteOnly))
    {
        state += mp_MainWindow->saveGeometry().toHex()+"\n";
        for (qint32 i = 0; i < m_AppListCount; ++i)
        {
            Applet *papplet = m_AppList[i];
            QRect geometry = papplet->containerWindow()->geometry();
            state += papplet->name();
            state += ',';
            state += QString::number(geometry.x(), 10);
            state += ',';
            state += QString::number(geometry.y(), 10);
            state += ',';
            state += QString::number(geometry.width(), 10);
            state += ',';
            state += QString::number(geometry.height(), 10);
            state += ',';
            state += QString::number(papplet->isOpened(), 10);
            state += '\n';
        }
        state_settings.write(state);
    }
}


/****************************************************************************
 * @function name: MainApp::loadAppletStates()
 *
 * @description: loads windows position.last workspace
 * @return: ( void )
 ****************************************************************************/
void MainApp::loadWorkspace()
{
    QByteArray state;

    QFile state_settings(QMAKE_TARGET".state") ;
    if(isBoardConnected() && state_settings.open(QFile::ReadOnly))
    {
        state = state_settings.readAll();
        const int WINDOW_NAME =0;
        const int WINDOW_LEFT = 1;
        const int WINDOW_TOP = 2;
        const int WINDOW_WIDTH = 3;
        const int WINDOW_HEIGHT = 4;
        const int WINDOW_VISIBLE = 5;
        QList<QByteArray> l1 = state.split('\n');
        for (qint32 i = 0; i < l1.size(); i++)
        {
            if(i==0){
                mp_MainWindow->restoreGeometry(QByteArray::fromHex(l1.at(0)));
                continue;
            }
            QList<QByteArray> l2 = l1.at(i).split(',');
            if ( l2.size() == 6 )
            {
                Applet *papplet = MainApp::findApplet( l2.at(WINDOW_NAME) ,BY_NAME);
                if (papplet)
                {
                    papplet->containerWindow()->setGeometry(
                                l2.at(WINDOW_LEFT).toInt()
                                , l2.at(WINDOW_TOP).toInt()
                                , l2.at(WINDOW_WIDTH).toInt()
                                , l2.at(WINDOW_HEIGHT).toInt()
                                );
                    if(l2.at(WINDOW_VISIBLE).toInt())
                        papplet->open_window();
                    else
                        papplet->close_window();
                }
            }
        }
    }
    else {
        if(!MAINAPP->quietMode())
            QMessageBox::warning(0,"Warning","Board is not connected");
    }
}





void MainApp::slot_connected()
{
#ifdef USE_FTDI
#else
    if(mp_MainWindow->currentChannel() == "mockup" )
    {

        mp_MainWindow->setVersionText("<span style=\"background-color:#FF0000;\">!!! SIMULATION !!!</span>");
    }
    else {        
        board()->checkVersion();
        mp_MainWindow->setVersionText(QString("   %1 : 0x%2%3 ")
                                      .arg(board()->project())
                                      .arg(board()->versionMajor(), 4, 16, QChar('0'))
                                      .arg(board()->versionMinor(), 4, 16, QChar('0')));
    }
#endif
    mp_MainWindow->setStateConnected(MainWindow::CONNECTED);
    resumeAllApplets();
}

void MainApp::slot_disconnected()
{
    suspendAllApplets();
    mp_MainWindow->setStateConnected(MainWindow::DISCONNECTED);
}

void MainApp::PreventSleep(bool on)
{
    if(on){
        SetThreadExecutionState(ES_CONTINUOUS | ES_SYSTEM_REQUIRED | ES_AWAYMODE_REQUIRED |ES_DISPLAY_REQUIRED);
    }
    else SetThreadExecutionState(ES_CONTINUOUS);
}











/* columns define */
const int COL_NUM =     (1);
const int COL_BITN =    (2);
const int COL_NAME =    (3);
const int COL_ANAME =   (4);
const int COL_CFGS =    (5);
const int MAX_CFGS =    (100);
const int START_ROW =   (6);
const int HEADER_ROW =   (5);
const int MAX_ROWS =    (10000);



bool MainApp::loadExcelConfig()
{



    bool result = false;
/*
    // obtaining filename
    QString filename = QFileDialog::getOpenFileName(0, "Load file",
                       "",
                       "Files (*.xlsx)");
    if ( filename.isEmpty() )
    {
        return false;
    }

    // make dialog for progress bar
    DialogProgressBar dialog;
    dialog.show();
    dialog.setText("Opening Excel");

    // open excel file
    ExcelData excel(filename);
    if ( excel.open('r')  )
    {
        excel.setCurrentSheet("ScanChains");
        excel.setVisible(0);
        QVariant name;
        QVariant aname;
        QVariant bitn;
        QVariant chain;
        QVariant modes1;
        QVariant modes2;

        // configuration read
        excel.read(1,3,modes1);
        excel.read(2,3,modes2);
        SCANCHAIN->setModesList(modes1.toString().split(';',QString::SkipEmptyParts)
                                   ,modes2.toString().split(';',QString::SkipEmptyParts));



        // read data

        dialog.setText("Evaluating file...");
        QVariantList l ;

        for(int i=0;i<20;i++)
        {
            QVariantList l2 =excel.readRange(QString("A%1").arg(i*500+1),QString("AZ%1").arg(i*500+500));
            l.append(l2);
            if(l2[0].toList()[1].isNull()) break;
        }


        qint32 rows=0;
        // evaluate size
        for(int i=0;i<l.count();i++)
        {
            if(l[i+START_ROW-1].toList()[1].isNull()) break;
            rows++;
        }


        // scan chain populating
        dialog.setText("Loading...");
        Register scanchain;
        for (qint32 i = 0; i < rows; i++)
        {
            dialog.setProgress(i,rows);
            QColor foreground,background;
            chain = l[i+START_ROW-1].toList()[0];
            // getting color of bitn field
            excel.color(i+START_ROW, COL_BITN, background, foreground);
            // reading bitn contens
            bitn = l[i+START_ROW-1].toList()[1];
            // reading digital name
            name = l[i+START_ROW-1].toList()[2];
            // reading analog name
            aname = l[i+START_ROW-1].toList()[3];
            if(name.toString() == "<<<>>>") {
                int spare_count = bitn.toInt();
                while(spare_count--){
                    Bit *pbit = new Bit;
                    pbit->setName("...");
                    pbit->setExtra("color", "#808080");
                    scanchain.addBit(pbit);
                }
                continue;
            }
            Bit *pbit =  new Bit;//create bit

            if(background != Qt::white)
            {
                char tmp[10];
                SPRINTF(tmp,"#%02x%02x%02x"
                          ,background.toRgb().blue()
                          ,background.toRgb().green()
                          ,background.toRgb().red());
                pbit->setExtra("color",QByteArray(tmp));
            }
            pbit->setDescription(name.toString());
            //analog or digital
            if (aname.toString().isEmpty() || aname.toString() == "unused")
            {
                pbit->setExtra("type", "DIGITAL");
                pbit->setName("_");
            }
            else
            {
                pbit->setExtra("type", "ANALOG");
                pbit->setName(aname.toString());//analog name to name2
            }
            //chain.subchain
            {
                QStringList lchainsub = chain.toString().split('.');
                switch(lchainsub.size())
                {
                    case 2:
                        pbit->setExtra("sub",lchainsub[1]);
                    case 1:
                        {
                        pbit->setExtra("num",lchainsub[0]);
                        }
                        break;
                    case 0:
                    default:
                        break;
                }
            }

            scanchain.addBit(pbit);
        }

        SCANCHAIN->setScanchain(&scanchain);
        // configurations load
        dialog.setText("Loading configurations...");
        QVariant data;
        Register config;
        config = scanchain;
        for(int j=0;j< MAX_CFGS; j++)// max configs 100
        {
            data = l[HEADER_ROW-1].toList()[j+COL_CFGS-1];
            if(data.toByteArray().isEmpty()) break;
            config.setName(data.toString());
            dialog.setText(data.toString());
            qint32 bit =0;
            for (qint32 i = 0 ;i < rows; i++)
            {
                dialog.setProgress(i,rows);
                data = l[i+START_ROW-1].toList()[j+COL_CFGS-1];
                config.setBit(bit++,data.toInt());
            }
            gp_Scanchain->addConfig(config.name(),&config);
        }
        // finally refresh
        SCANCHAIN->rebuild();
        // close xls file
        excel.close();
        result = true;
        m_last_xls_file = filename;
    }

*/
    return result;
}


void MainApp::saveBackToExcel(const QString &/*applet_name*/)
{
  /*  bool result =false;
    bool add_new_config = false;
    DialogProgressBar dialog;
    dialog.show();
    dialog.setText("Opening Excel");
    QString name = QInputDialog::getText(0,"","New name of configuration.\n"
                                              "If save to existing - don't change anything."
                                              , QLineEdit::Normal
                                                ,SCANCHAIN->currentConfig());

    if(name.isEmpty()) return;
    add_new_config =  !SCANCHAIN->containsConfig(name);
    SCANCHAIN->addConfig(name, SCANCHAIN->scanchain());

    if(QFile::exists(m_last_xls_file))
    {
        int col=-1;
        ExcelData excel(m_last_xls_file);
        if(excel.open('w'))
        {
            excel.setCurrentSheet("ScanChains");
            excel.setVisible(0);
            QVariant data;
            dialog.setText("Finding config");
            if(add_new_config){
                for(int i=0;i<MAX_CFGS;i++)
                {
                    if(!excel.read(HEADER_ROW, COL_CFGS+i, data)) break;
                    if(data.toString().trimmed() == ""){
                        excel.write(HEADER_ROW, COL_CFGS+i, name);
                        col = COL_CFGS+i;
                        break;
                    }
                }
            }
            else {
                for(int i=0;i<MAX_CFGS;i++)
                {
                    if(!excel.read(HEADER_ROW, COL_CFGS+i, data)) break;
                    if(data.toString() == SCANCHAIN->currentConfig()){
                        col = COL_CFGS+i;
                        break;
                    }
                }
            }
            if(col >= COL_CFGS){
                for(int i=0;i<SCANCHAIN->size();i++){
                    dialog.setText(QString("Appling data %1").arg(i));
                    dialog.setProgress(i,SCANCHAIN->size());
                    if(!excel.write(HEADER_ROW+1+i, col, QString::number(SCANCHAIN->scanchain()->value(i))))
                    {result=false;break;}
                    else result=true;
                }
            }
            else {
            }
            excel.save();
            excel.close();
        }
        else {
            QMessageBox::warning(0,"",QString("Can't open configuration file %1").arg(m_last_xls_file));
        }
    }
    if(result) QMessageBox::information(0,"",QString("Data saved back to excel file %1").arg(m_last_xls_file));*/
}
