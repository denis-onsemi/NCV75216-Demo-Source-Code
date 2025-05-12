/**
 * @file:mainwindow.h   -
 * @description:
 * @project: BENCH OnSemiconductor
 * @date: 2014\11\27 13-47-57
 *
 */


#ifndef MAINWINDOW_H
#define MAINWINDOW_H

class MainWindow;

#include "applet.h"
#include <QMainWindow>
#include <QMdiArea>
#include <QCloseEvent>
#include <QMdiSubWindow>
#include <QStatusBar>
#include <QMenuBar>
#include <QApplication>
#include <QLabel>
#include <QToolBar>
#include <QPushButton>
#include <QComboBox>
#include <QDockWidget>
#include "customcontrols.h"
#include <QTimer>
#include <QToolButton>

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    typedef enum
    {
        WINDOW_STYLE_DOCK,
        WINDOW_STYLE_MDI
    } WindowStyle;

    typedef enum
    {
        MenuHelp,
        MenuApplication,
        MenuProject
    } MenuCategory;


    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    /* add window to menu category with style MDI,DOCK*/
    void addWindow(MenuCategory category, Applet *pwindow, MainWindow::WindowStyle style);
    /* sets available channels list*/
    void setChannelsList(const QStringList &list);
    /*adding custom push button. may be added from applet*/
    void addWidget(QWidget *ppb);
    /* active window in main window.can be only one*/
    Applet *activeChild();
    /* current selected channel name*/
    const QString currentChannel(void);
    /* if communication in log mode*/
    bool logComm() const;
    typedef enum { DISCONNECTED,CONNECTED }State;
    void setStateConnected(MainWindow::State state);
    void setVersionText(const QString &text);
    quint32 connectionSpeed() ;
    void updateConfigBenchList();

public slots:

    void slot_ActivityRx_LED();
    void slot_ActivityTx_LED();
    void slot_Connection_LED(bool);
    void slot_stream(bool);
    void slot_SaveConfig();
    void slot_LoadConfig();
    void slot_LoadConfigName();
    void slot_supress_led();

protected:
    void closeEvent(QCloseEvent *ev);
    void contextMenuEvent(QContextMenuEvent *event);

private slots:
    void slot_RestorePreviousWorkspace();
    void slot_pbConnectBoard_click();
    void slot_Rescan();
    void slot_LoadProjectData();
    void slot_AboutApp();
    void slot_UpdateBenchSoftware();
    void slot_UpdateFirmware();
    void slot_CloseAll();
    void slot_CloseAllButActive();     
    void slot_cbLogComm_toggled(bool enable);
    void slot_ChangeConenctionSpeed();
    void slot_LogOn(bool);

#ifdef USE_MPP
    void slot_pbMppShutdown();
    void slot_pbMppRestart();
#endif

    void slot_pbDontSleep(bool);
    void slot_timeout();

private:
    virtual bool eventFilter(QObject *pobj, QEvent *pevent);
    void createUi();
    void createConnectionBar();

    /****** UI *******/
    QTimer *mp_timer;
    QMenu *mp_menu;
    QMdiArea *mp_MdiArea;
    QMenu *mp_MenuProject;
    QMenu *mp_MenuConfig;
    QMenu *mp_MenuApplication;
    QMenu *mp_MenuHelp;
    QAction *mp_updateFirmware;
    QAction *mp_neverSleep;
    QAction *mp_updateSoftware;
    QLabel *mp_lbStatus;
    /* log communication checkbutton*/
    QCheckBox *mp_cbLogComm;
    /* channel connect box*/
    QPushButton *mp_pbConnectBoard;
    QPushButton *mp_pbRescan;    
    QComboBox   *mp_cmbConnectChannel;
    QComboBox   *mp_cmbConnectionSpeed;

#ifdef USE_MPP
    QPushButton *mp_pbMppRestart;
    QPushButton *mp_pbMppShutdown;
#endif

    QLabel *mp_lbMcuVersion;
    /* colored leds */
    CustomControls::MySwitch *mp_cswLed0;
    CustomControls::MySwitch *mp_cswLed1;
    CustomControls::MySwitch *mp_cswLed2;
    QList<QAction*> m_configs;

    QTimer m_tmr;
};

#endif // MAINWINDOW_H
