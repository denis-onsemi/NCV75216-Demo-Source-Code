/**
 * @file:localconnector.cpp
 * @description:
 *    This is named pipe connector for controlling app.
 * @project: BENCH OnSemiconductor
 * @date: 2014\11\27 13-50-31
 *
 */
#include "localserver.h"
#include <QMessageBox>
#include <QLocalSocket>
#include "mainapp.h"
#include <QInputDialog>
#include <QVariant>


QVariant QString_to_QVariant(const QString &str )
{
    QVariant result;
    char c = str[0].toLatin1();
    /* types f- float, i -integer, s- string */
    switch (c)
    {
    case ( 'f' ):
        result = str.mid(1).toDouble();
        break;

    case ( 'i' ):
        result = str.mid(1).toInt();
        break;

    case ( 's' ):
        result = str.mid(1);
        break;

    case ('x'):
        result = QByteArray::fromHex(str.mid(1).toLatin1());
        break;
    }
    return result;
}

QString QVariant_to_QString(const QVariant &var)
{
    QString result;
    switch (var.type())
    {

    case QVariant::Double:
        result = 'f' + var.toString().toLatin1();
    break;

    case QVariant::Bool:
        result = QString("i%1").arg(var.toInt()).toLatin1();
        break;
    case QVariant::UInt:
    case QVariant::Int:
        result = 'i' + var.toString().toLatin1();
        break;
    case QVariant::String:
        result = 's' + var.toString().toLatin1();
        break;
    case QVariant::ByteArray:
        result = 'x' + var.toByteArray().toHex();
        break;
    default:
        break;
    }
    return result;
}

/****************************************************************************
 * @function name: LocalConnector::LocalConnector()
 *
 * @param:
 *
 * @description:
 * @return: ( void )
 ****************************************************************************/
LocalServer::LocalServer()
{    
    m_log_mode=false;
}

/****************************************************************************
 * @function name: LocalConnector::initialize()
 *
 * @param:const QString &pipename - pipename to connect to
 *
 * @description:
 * @return: ( void )
 ****************************************************************************/
bool LocalServer::initialize(const QString &pipename)
{
    mp_server = new QLocalServer(this);
    QString new_file_pipe_name =  QString("\\\\.\\pipe\\%1").arg(pipename);
    do
    {
        if(QFile::exists(new_file_pipe_name)
                || !new_file_pipe_name.startsWith("\\\\.\\pipe\\"))
        {
            QMessageBox msg;
            msg.setText("Pipe is already used by another software\nUse another pipename?(Yes)");
            msg.addButton(QMessageBox::Yes);
            msg.addButton(QMessageBox::Close);
            int res = msg.exec();
            if(res== QMessageBox::Yes)
            {
                bool ok;
                QString text = QInputDialog::getText(0, "",
                                                     "Pipename", QLineEdit::Normal,
                                                     "", &ok);
                if(ok && !text.isEmpty())
                    new_file_pipe_name = QString("\\\\.\\pipe\\%1").arg(text);
            }
            else {
                return false;
            }
        }
        else {
            m_file_pipe_name = new_file_pipe_name;
            break;
        }
    }while(1);

    if (!mp_server->listen(m_file_pipe_name))
    {
        QMessageBox::critical(0, "Local server failed",
                              QString("Unable to start interprocess server: \n%1.")
                              .arg(mp_server->errorString()));
        return false;
    }
    mp_server->setMaxPendingConnections(1);
    connect(mp_server, SIGNAL(newConnection()), this, SLOT(processConnection()));
    m_pipename = pipename;    
    return true;
}

const QString &LocalServer::pipeName() const
{
    return m_pipename;
}


/****************************************************************************
 * @function name: Bench::processInput()
 *
 * @param:
 *      void
 * @description: processes string protocol
 * @return: ( void )
 ****************************************************************************/
QByteArray LocalServer::parse(const QByteArray &data_in)
{
    //constants
    const int COMMAND = 0;
    const int VARIABLE = 1;
    //const int PARAMETER = 2;

    QByteArray result;
    QList<QByteArray> l = data_in.split('\t');

    // SET
    if (l.at(COMMAND) == "set" && l.size()==3 )
    {
        QVariant value_in = QString_to_QVariant(l.at(2));
        bool ok = LOCALSERVER_SET(QString(l.at(VARIABLE)), value_in);
        if(ok)     result ="ok\n";
        else result = "err\n";
    }


    // RUN
    else if( l.at(COMMAND)== "run" && l.size()>=2)
    {
        QVariant value_in;
        QVariant value_out;
        if(l.size() == 3)
        {
            value_in = QString_to_QVariant(l.at(2));
        }
        else{
            QVariantList        var_l;
            foreach(const QByteArray &txt, l.mid(2))
            {
                var_l.append(QString_to_QVariant(txt));
            }
            value_in = var_l;
        }

        bool ok =  LOCALSERVER_RUN(QString(l.at(VARIABLE)), value_in, &value_out);
        result = QVariant_to_QString(value_out).toLatin1();
        if(!ok ) result = "err\n";
    }

    // GET
    else if (l.size() == 2 && l.at(COMMAND) == "get")
    {        
        QVariant value_out;
        bool ok =  LOCALSERVER_GET(QString(l.at(VARIABLE)), &value_out);
        result = QVariant_to_QString(value_out).toLatin1();
        if(!ok) result = "err\n";
    }
    else {
        result = "err\n";        
    }
    return result;
}



void LocalServer::setLogMode(bool enable)
{
    m_log_mode = enable;
}

void LocalServer::log_message(const QString &text)
{
    if( logMode() )
    {
        gp_Logs->slot_addLog(LOG_COLOR(Qt::cyan), QString(text).replace('\t'," "));
    }
}


/****************************************************************************
 * @function name: Bench::processConnection()
 *
 * @param:
 *
 *
 * @description:
 * @return: ( void )
 ****************************************************************************/
void LocalServer::processConnection()
{
    QLocalSocket *clientConnection = mp_server->nextPendingConnection();
    connect(clientConnection, SIGNAL(disconnected()),
            clientConnection, SLOT(deleteLater()));
    qint32 count = 10;    

    while (count--)
    {
        clientConnection->waitForReadyRead(10);
        input.append(clientConnection->readAll());
        if (input.contains("\n")) break;
    }

    if(!input.isEmpty())
    {
        QList<QByteArray> l = input.replace("\r", "").split('\n');
        QByteArray last_line;
        foreach(const QByteArray &line, l)
        {
            if(last_line == line || line.trimmed().isEmpty()) continue;
            log_message(line);
            output += parse(line);
            last_line = line;
        }
    }    
    log_message(output);
    clientConnection->write(output);
    clientConnection->flush();
    clientConnection->disconnectFromServer();
    input.clear();
    output.clear();
}


bool LocalServer::logMode() const
{
    return m_log_mode;
}
