/**
 * @file:axconnector.cpp   -
 * @description:
 * @project: BENCH OnSemiconductor
 * @date: 2014\11\27 13-41-51
 *
 */


#include <QApplication>
#include <QAxFactory>
#include "mainapp.h"
#include "axconnector.h"


QAXFACTORY_BEGIN("{3b0025ab-5059-4a89-93ca-b4313522092a}", "{93c91a8f-c963-4983-9146-163e69847d20}")

QAXCLASS(Bench)
QAXFACTORY_END()



/****************************************************************************
 * @function name: Constructor
 *
 * @param:
 *
 ****************************************************************************/
Bench::Bench(QObject *parent): QObject(parent)
{
    setObjectName("benchboard");    
}


/****************************************************************************
 * @function name: Bench::quit - ---
 *
 * @param:
 *             void
 * @description:  quit procedure . Closes app immediately
 * @return: ( void )
 ****************************************************************************/
void Bench::quit()
{
    qApp->quit();
}


/****************************************************************************
 * @function name: Bench::set - ---
 *
 * @param:
 *       QString command - command for applet
 *       QVariant data  - data for applet
 * @description:
 * @return: ( bool )
 ****************************************************************************/
bool Bench::set(QString command, QVariant data)
{
    bool result =MAINAPP->set(command, data);
    return result;
}


/****************************************************************************
 * @function name: Bench::set - ---
 *
 * @param:
 *         QString command -command for applet
 *        QVariant data - data for applet
 * @description:
 * @return: ( bool )
 ****************************************************************************/
QVariant Bench::run(QString command, QVariant data)
{    
    QVariant result;
    MAINAPP->run(command,data,&result) ;
    return result;
}

/****************************************************************************
 * @function name: Bench::get - ---
 *
 * @param:
 *         QString command  - command for applet . what to read
 * @description:
 * @return: ( QVariant )
 ****************************************************************************/
QVariant Bench::get(QString command)
{    
    QVariant result;
    MAINAPP->get(command,&result);
    return result;
}

#ifdef USE_FTDI
#else
bool Bench::open(QString portname)
{
    return MAINAPP->openBoardConnection(portname);
}

bool Bench::close()
{
    if(MAINAPP->isBoardConnected())
    {
        MAINAPP->closeBoardConnection();
        return true;
    }
    return false;
}

QByteArray Bench::write(QByteArray data)
{
    QByteArray answer;
    if(MAINAPP->board() && MAINAPP->board()->isConnected())
    {
        MAINAPP->board()->transferData(data, &answer);
    }
    return answer;
}

#endif
