/**
 * @file:axconnector.h   -
 * @description:
 * @project: BENCH OnSemiconductor
 * @date: 2014\11\27 13-41-51
 *
 */


#ifndef AXCONNECTOR_H
#define AXCONNECTOR_H

#include <QApplication>
#include <QAxFactory>



class Bench;


/* ActiveX connector */
class Bench : public QObject
{
    Q_OBJECT

    Q_CLASSINFO("ClassID", "{5533dc46-a6f2-45f8-a812-2a06bde4ccc7}")
    Q_CLASSINFO("InterfaceID", "{cd1e1629-2331-45cd-a880-325361981b84}")
    Q_CLASSINFO("RegisterObject", "yes")

public:
    Bench(QObject *parent);

    /*ActiveX interface of application */
public slots:
    /* quit from software*/
    void quit(void );
    /* set for setting parameters for objects*/
    bool set(QString command, QVariant data);
    /* run and set has the same functionality only different name*/
    QVariant run(QString command, QVariant data);
    /* get is for getting parameter from obejcts*/
    QVariant get(QString command);

#ifdef USE_FTDI
#else
    bool open(QString portname);
    bool close();
    QByteArray write(QByteArray data);
#endif
private:

};


#endif // AXCONNECTOR_H
