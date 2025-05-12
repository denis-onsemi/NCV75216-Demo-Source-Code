/**
 * @file:main.cpp   -
 * @description:
 * @project: BENCH OnSemiconductor
 * @date: 2014\11\27 13-59-17
 *
 */



#include <QApplication>
#include <QAxFactory>
#include "mainapp.h"

/* MAIN */

/****************************************************************************
 * @function name: main()
 *
 * @param:
 *      int argc
 *      char **argv
 * @description:
 * @return: ( int )
 ****************************************************************************/
int main(int argc, char **argv)
{
    int result=0;
    QApplication app(argc, argv);
    app.setQuitOnLastWindowClosed(false);
    /* stylesheet for aplication*/
    QFile file(":/stylesheet/stylesheet") ;
    file.open(QFile::ReadOnly);
    qApp->setStyleSheet(file.readAll());
    file.close();
    /* get bench application instance (singletone)*/
    MAINAPP = MainApp::getInstance(); 
    /* application initilize*/
    if( MAINAPP->initialize())
    {
        /* starts application */
        MAINAPP->start();
        /* qt app execution */
        result = app.exec();
        /* when finish shutdown bench application */
        MAINAPP->shutdown();
    }
    return result;
}

