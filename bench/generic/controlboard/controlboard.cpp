/**
 * @file:mcuprotocol.cpp   -
 * @description:
 * @project: BENCH OnSemiconductor
 * @date: 2014\11\27 13-50-31
 *
 */


#include "controlboard.h"
#include "mainapp.h"
#include <QDebug>
#include <QFileDialog>
#include <QProcess>
#include <QMessageBox>

// fast connection protocol

#define MCU_REQUEST     ('\x05')
#define MCU_ACK         ('\x06')
#define MCU_RESET       ("\x18\x18\x18\x18\x18\x18")

QMutex mutex;

#include "circularbuffer.h"

CircularBuffer rx_buf(5000);
CircularBuffer  tx_buf(5000);



/****************************************************************************
 * @function name: CONSTRUCTOR
 *
 ****************************************************************************/
ControlBoard::ControlBoard()
    :QThread(0)
{
    m_logMode = false;
    mp_channel = new SerialPortChannel();
    m_finish = false;
    m_timeout = DEFAULT_TIMEOUT;
    m_errors_count =0;
    m_suspend=false;
    m_connected = false;
    m_capture_mode = false;
    m_ain_vref  =3.30f;
    rx_buf.reset();
    tx_buf.reset();
    this->start(QThread::HighPriority);
}


/****************************************************************************
 * @function name: DESTRUCTOR
 *
 ****************************************************************************/
ControlBoard::~ControlBoard()
{
    m_finish = true;
    mp_channel->close();
    terminate();
}

/****************************************************************************
 * @function name: ControlBoard::connect()
 *
 * @param:
 *    const QString & port - port name
 * @description: connects mcu .
 * 1. open channel , 2. controls if there is some board connected
 * @return: ( void )
 ****************************************************************************/
bool ControlBoard::connectDevice(const QString & port,bool /*reset*/)
{
    //UNREFERENCED_PARAMETER(reset);
    bool result = false;
    if(port == "mockup")
    {
        m_mode = MOCKUP;
        initMockup();
        m_port = port;
        if(!MAINAPP->quietMode())
            QMessageBox::information(0,"","Software is in Mock-up mode. \n "
                                            "Communication is simulated!");
        result =true;
        m_connected = true;
    }
    else if(port == "script")
    {
        m_mode = SCRIPT;
        // TODO open file
        QString script_path;
        while(!QFile::exists(script_path))
        {
            QFileDialog dialog;
            script_path = dialog.getOpenFileName(0, "Script path",
                                                 "",
                                                 "Files (*.py)");
            if(dialog.result() == QDialog::Rejected) break;
        }
        if(!script_path.isEmpty() && QFile::exists(script_path))
        {
            m_script_file = script_path;
            m_port = port;
            result =true;
        }
    }
    else
    {
        m_mode = MICROCONTROLLER;
        mp_channel->setRTS(0);
        if( !port.isEmpty()
                && mp_channel->open(port)
                )
        {
            result = checkDevice();
            if(result)
            {
               // if(reset) resetDevice();
                m_port = port;
                m_connected =true;
            }
            else mp_channel->close();
            tx_buf.reset();
            rx_buf.reset();
            //checkVersion();
        }
    }    
    if(result)
    {
        log_message(LOG_MCU_INFO,"Connected to "%port);
        emit signal_connected();
    }
    return  result;
}

bool ControlBoard::setBaudrate(quint32 baudrate)
{
    if(mp_channel)
    {
        m_baudrate = baudrate;
        if(isConnected())
        {
            mutex.lock();
            m_suspend=true;
            mutex.unlock();
            mp_channel->sendData("set baudrate="+QByteArray::number(baudrate)+"\n");
            mp_channel->close();
            mp_channel->open(m_port,baudrate);
            mutex.lock();
            m_suspend=false;
            mutex.unlock();
        }

        return true;
    }
    return false;
}

quint32 ControlBoard::baudrate() const
{
    return m_baudrate;
}

/****************************************************************************
 * @function name: ControlBoard::run()
 *
 * @param:
 *     void
 * @description: thread runner
 * @return: ( void )
 ****************************************************************************/
void ControlBoard::run()
{
    bool reconnect = false;
    //timer for reconnect
    QTime tmr;
    tmr.start();
    while( !m_finish )
    {
        if(m_mode == MICROCONTROLLER && isConnected())
        {
            if(m_capture_mode)
            {
                QByteArray rx;
                mp_channel->sendData(m_capture_command.toLatin1() );
                mp_channel->setRTS(1);
                emit signal_stream(1);
                while(m_capture_mode)
                {
                    if( mp_channel->bytesAvailable())
                    {
                        mp_channel->readAll(rx);
                        emit signal_captureData(rx);
                    }
                    msleep(100);
                }
                mp_channel->setRTS(0);
                mp_channel->sendData(QByteArray("\x15\x15\x15",3));
                tmr.restart();
                emit signal_stream(0);
                msleep(100);
                mp_channel->readAll(rx);
            }

            //write new data to SerialPort
            if(tx_buf.size()>0)
            {
                mp_channel->sendData(tx_buf.readAll());
                emit signal_tx();
            }
            //wait for data on Serial Port

            while(mp_channel->bytesAvailable())
            {
                QByteArray rx;
                emit signal_rx();
                // if result ==  -1 -> connection lost
                if(mp_channel->readAll(rx) != -1)
                {
                    rx_buf.write(rx);
                    tmr.restart();
                }
            }

            // if is time to check connection
            if(tmr.elapsed() > 2000 )
            {
                tmr.restart();                
                bool connected = isConnected();
                bool check_passed = checkDevice();
                reconnect = !connected || (connected && !check_passed) ;
            }
        }
        else //no connected wait for connection
        {
            msleep(500);
        }

        //need reconnect
        if(reconnect)
        {
            reconnect=false;
            tmr.restart();
            int retries=10;
            disconnectDevice(NEED_RECONNECT);
            log_message(LOG_MCU_INFO,"Auto reconnecting...");
            while(retries--)
            {
                if(connectDevice(m_port,false)) {
                    break;
                }
                msleep(1000);
            }

            if(isConnected())
            {
                // [1] TODO check session
                // [2] TODO reconfigure
            }
            else disconnectDevice(DUE_ERROR);
        }

        while(m_suspend) {msleep(100);}
    }
    exec();
}


/****************************************************************************
 * @function name: ControlBoard::extractValue()
 *
 * @param:
 *           const QByteArray &data
 *           const QString &name
 *           QByteArray *pvalue
 * @description: extract value from string
 * @return: ( bool)
 ****************************************************************************/
bool ControlBoard::extractText(const QByteArray &data, const QString &name, QByteArray *pvalue)
{
    bool result = false;
    QList<QByteArray> l1,l2;
    if(pvalue && !data.isEmpty())
    {
        //l1 = data.split(' ');
        splitBySpace(data,&l1);
        foreach(const QByteArray &item, l1)
        {
            l2 = item.split('=');
            if(name == l2.at(0))
            {
                *pvalue = l2.at(1);
                result = true;
                break;
            }
        }
    }
    return result;
}

bool ControlBoard::extractByteArray(const QByteArray &data, const QString &name, QByteArray *pvalue)
{
    QByteArray text;
    bool result = extractText(data,name,&text);
    if(pvalue) *pvalue = QByteArray::fromHex(text);
    return result;
}


/****************************************************************************
 * @function name: ControlBoard::disconnectMcu()
 *
 * @param:
 *           void
 * @description:
 * @return: ( void)
 ****************************************************************************/
void ControlBoard::disconnectDevice(DisconnectReason reason)
{     
    do{
        if(m_capture_mode) stopStream();
        //        if(reason == DUE_ERROR)
        //        {
        //            if(!MAINAPP->quietMode())
        //            {
        //                QMessageBox msg;
        //                msg.setText("Communication errors detected");
        //                msg.setInformativeText("Do you want to continue(Ignore) or stop(Cancel)?");
        //                msg.setStandardButtons(QMessageBox::Ignore | QMessageBox::Cancel);
        //                msg.setDefaultButton(QMessageBox::Cancel);
        //                if(msg.exec() == QMessageBox::Ignore) break;
        //                else reason =USER_COMMAND ;
        //            }
        //        }

        if(reason == DUE_ERROR)
        {
            log_message(LOG_MCU_ERROR,"Disconnect due to errors");
        }
        else if(reason== NEED_RECONNECT)
        {
            log_message(LOG_MCU_INFO,"Reconnect");
        }
        else if(reason==USER_COMMAND){
            log_message(LOG_MCU_INFO,"Disconnect by user");
        }

        if(m_mode== MICROCONTROLLER && mp_channel)
        {
            setBaudrate(DEFAULT_BAUDRATE);
            mp_channel->setRTS(0);
            mp_channel->close();
        }
        emit signal_disconnected();
        m_connected = false;
    }while(0);
}

/****************************************************************************
 * @function name: ControlBoard::availablePorts()
 *
 * @param:
 *           QStringList *plist
 * @description:
 * @return: ( void)
 ****************************************************************************/
void ControlBoard::availablePorts(QStringList *plist)
{
    mp_channel->enumDevices(plist);
    plist->append("mockup");
    plist->append("script");
}

/****************************************************************************
 * @function name: ControlBoard::scanForMcu()
 *
 * @param:
 *           const QStringList &list_in
 *           QStringList *plist_out
 * @description:
 * @return: ( void)
 ****************************************************************************/
void ControlBoard::scanDevices(const QStringList &list_in, QStringList *plist_out)
{
    if(!isConnected())
    {
        foreach(const QString& port, list_in)
        {
            if(port == "script" || port == "mockup" )
            {
                plist_out->append(port);
            }
            else if( mp_channel->open(port) )
            {
                m_mode = MICROCONTROLLER;
                if(checkDevice())
                    plist_out->append(port);
                mp_channel->close();
            }
        }
    }
}

/****************************************************************************
 * @function name: ControlBoard::timeout()
 *
 * @param:
 *          const quint32 &timeout
 * @description:
 * @return: ( void )
 ****************************************************************************/
void ControlBoard::setTimeout(const quint32 &timeout)
{
    m_timeout = timeout;
}


/****************************************************************************
 * @function name: ControlBoard::isConnected()
 *
 * @param:
 *          void
 * @description:
 * @return: ( bool )
 ****************************************************************************/
bool ControlBoard::isConnected() const
{
    return m_connected;
}

/****************************************************************************
 * @function name: ControlBoard::transferData()
 *
 * @param:
 *          const QByteArray &data_write
 *          QByteArray *pdata_read
 * @description:
 * @return: ( bool )
 ****************************************************************************/
bool ControlBoard::transferData(const QByteArray &data_write, QByteArray *pdata_read,int timeout)
{
    bool result = false;
    QByteArray answer;
    if(isConnected() && !m_capture_mode)
    {
        if(m_logMode)
        {
            log_message(LOG_MCU_TX, QString(data_write));
        }

        switch( m_mode)
        {
        case (MOCKUP):
            transferMockup(data_write, &answer);
            log_message(LOG_MCU_RX,QString(answer));
            result =true;
            break;

        case (SCRIPT):
            transferScript(data_write, &answer);
            log_message(LOG_MCU_RX,QString(answer));
            result =true;
            break;

        case(MICROCONTROLLER):
            {
                int retry =1;
                rx_buf.reset();
                while(retry-- )
                {
                    if(data_write.count()>0)
                    {
                        int packets_send = data_write.count("\n");
                        if(data_write.contains('\n') && data_write.at(data_write.count()-1) =='\n')
                        {
                            tx_buf.write(data_write);
                        }
                        else{
                            packets_send +=1;
                            tx_buf.write(QByteArray(data_write).append('\n'));
                        }
                        QTime tmr;
                        tmr.start();

                        //if timeout parameter is -1 set default
                        if( timeout == -1 )
                            timeout = m_timeout;
                        else  // do not wait just send to port and return
                            if(timeout==0) {result=true;break;}

                        int count_down = packets_send;
                        int packets_ok=0;
                        log_message(LOG_MCU_RX,QString("TIMEOUT:%1").arg(timeout));///****
                        while( count_down && tmr.elapsed()<timeout)
                        {
                            bool ok=false;
                            answer = rx_buf.readLine(&ok);                                                        
                            if(ok)
                            {
                                tmr.start();
                                log_message(LOG_MCU_RX,QString(answer));
                                packets_ok += !answer.contains("err") ;
                                count_down--;
                            }
                        }                                                
                        if(packets_ok == packets_send)
                        {
                            result=true;
                            break;
                        }
                        else {
                        }
                    }
                }
            }
        break;

        default :
            break;
        }
        if(result)
        {
            if( pdata_read)
            {
                *pdata_read= answer;
            }
        }
        else
        {

            log_message(LOG_MCU_ERROR, answer);
        }
    }
    return result;
}


/****************************************************************************
 * @function name: ControlBoard::setData()
 *
 * @param:
 *          const QString &variable_name
 *          qint32 variable_value
 * @description:
 * @return: ( bool )
 ****************************************************************************/
void ControlBoard::transferScript(const QByteArray &data_out, QByteArray *pdata_in)
{
    Q_ASSERT(pdata_in);
    QByteArray tmp = data_out;
    tmp.replace("\n","");
    MAINAPP->runScript(m_script_file,tmp,pdata_in);
}

/****************************************************************************
 * @function name: ControlBoard::splitBySpace()
 *
 * @param:
 *          const QByteArray &data
 *          QList<QByteArray> *plist
 * @description:
 * @return: ( qint32 )
 ****************************************************************************/
qint32 ControlBoard::splitBySpace(const QByteArray &data, QList<QByteArray> *plist)
{
    qint32  len = data.count();
    qint32 i = 0;
    qint32 chains = 0;
    const char *pstr = data.constData();
    QByteArray buf;
    char prev = ' ';
    while (1)
    {
        char symb = pstr[i];
        if (symb == ' ' || len == 0)
        {
            if (prev != ' ')
            {
                if(plist)
                    plist->append(buf);
                buf.clear();
                chains++;
            }
        }
        else
        {
            buf.append(symb);
        }
        if (len == 0)
        {
            break;
        }
        else
        {
            len--;
            i++;
            prev = symb;
        }
    }
    return chains;
}

/****************************************************************************
 * @function name: ControlBoard::setData()
 *
 * @param:
 *          const QString &variable_name
 *          qint32 variable_value
 * @description:
 * @return: ( bool )
 ****************************************************************************/
bool ControlBoard::setData(const QString &variable_name, qint32 variable_value)
{
    bool result ;
    QByteArray request;
    request =QString("set %1=%2\n").arg(variable_name).arg(variable_value).toLatin1() ;
    result = transferData(request) ;
    return result;
}

/****************************************************************************
 * @function name: ControlBoard::setData()
 *
 * @param:
 *          const QString &variable_name
 *          QByteArray variable_value
 * @description:
 * @return: ( bool )
 ****************************************************************************/
bool ControlBoard::setData(const QString &variable_name, QByteArray variable_value)
{
    bool result ;
    QByteArray request;
    request = QString("set %1=%2\n").arg(variable_name).arg(QString(variable_value)).toLatin1() ;
    result = transferData(request) ;
    return result;
}

/****************************************************************************
 * @function name: ControlBoard::getData()
 *
 * @param:
 *          const QString &variable_name
 *          qint32 *pvariable_value
 * @description:
 * @return: ( bool )
 ****************************************************************************/
bool ControlBoard::getData(const QString &variable_name, qint32 *pvariable_value)
{
    bool result;
    QByteArray data;
    QByteArray request = QString("get %1").arg(variable_name).toLatin1();
    QByteArray answer;
    result = transferData(request, &answer);
    result &= extractText(answer, variable_name, &data);
    if(pvariable_value &&!data.isEmpty()) *pvariable_value = data.toInt();
    return result;
}

/****************************************************************************
 * @function name: ControlBoard::getData()
 *
 * @param:
 *          const QString &variable_name
 *          QByteArray *pvariable_value
 * @description:
 * @return: ( bool )
 ****************************************************************************/
bool ControlBoard::getData(const QString &variable_name, QByteArray *pvariable_value)
{
    bool result ;
    QByteArray data;
    QByteArray request = QString("get %1").arg(variable_name).toLatin1();
    QByteArray answer;
    result = transferData(request, &answer);
    answer.replace('\n',"");    
    result &= extractText(answer, variable_name, &data);
    if(pvariable_value && !data.isEmpty()) *pvariable_value = data;    
    return result;
}

/****************************************************************************
 * @function name: ControlBoard::setDataList()
 *
 * @param:
 *          const QList<Variable> &variables
 * @description:
 * @return: ( bool )
 ****************************************************************************/
bool ControlBoard::setDataList(const QList<Variable> &variables)
{
    bool result = false;
    QByteArray answer;
    QByteArray request;
    if(variables.count()>0)
    {
        request = "set ";
        foreach(const Variable &v, variables)
        {
            switch(v.type())
            {

            case Variable::INTEGER:
                request  += QString("%1=%2 ").arg(v.name()).arg((int)v.value()).toLatin1();
                break;
            case Variable::REAL:
                request  += QString("%1=%2 ").arg(v.name()).arg((double)v.value()).toLatin1();
                break;
            case Variable::ARRAY:
                request  += QString("%1=%2 ").arg(v.name()).arg(QString(v.array()));
                break;

            case Variable::NONE:
                break;
            default:
                break;
            }

        }
        result = transferData(request,&answer);
        if(result)
        {
            foreach(const Variable &v, variables)
            {
                if ( !answer.contains(v.name().toLatin1()) )
                {
                    result = false;
                    break;
                }
            }
        }
    }
    return result;
}

/****************************************************************************
 * @function name: ControlBoard::getDataList()
 *
 * @param:
 *          QList<Variable> *pvariables
 * @description:
 * @return: ( bool )
 ****************************************************************************/
bool ControlBoard::getDataList(QList<Variable> *pvariables)
{
    bool result = false;
    QByteArray answer;
    QByteArray request;
    if(pvariables &&pvariables->count()>0)
    {
        request ="get ";
        foreach(const Variable &v, *pvariables)
        {
            request += v.name() +' ';
        }
        result =transferData(request, &answer);
        for(int i=0;i<pvariables->size();i++)
        {
            QByteArray data;
            result &= extractText(answer, (*pvariables)[i].name(), &data);
            switch((*pvariables)[i].type())
            {
            case Variable::INTEGER:
                (*pvariables)[i].setValue(data.toInt(),RING_INTERNAL);
                break;
            case Variable::REAL:
                (*pvariables)[i].setValue(data.toDouble(),RING_INTERNAL);
                break;
            case Variable::ARRAY:
                (*pvariables)[i].setValue(data,RING_INTERNAL);
                break;
            case Variable::NONE:
                break;
            default:
                break;
            }
        }
    }

    return result;
}


int ControlBoard::versionMajor()
{
    return m_version.major;
}

int ControlBoard::versionMinor()
{
    return m_version.minor;
}

int ControlBoard::versionBuild()
{
    return m_version.build;
}

QString ControlBoard::versionTimeDate()
{
    return m_version.timestamp.toString("MM.dd.yy hh:mm:ss");
}

QString ControlBoard::project() const
{
    return m_project;
}


/****************************************************************************
 * @function name: ControlBoard::genClocks()
 *
 * @param:
 *          const char *dio DIO port name
 *          quint32 count - count of clocks
 *          quint32 inverted - inverted 1-yes
 *          quint32 delay - delay after each pin toggle in us
 * @description:
 * @return: ( bool )
 ****************************************************************************/
bool ControlBoard::genClocks(const char *dio, quint32 count, quint32 inverted, quint32 delay)
{
    char tmp[100];
    SPRINTF(tmp,"%s,%04d,%1d,%02d",dio,count,inverted,delay);
    return transferData("set genclk="+QByteArray(tmp));
}

/****************************************************************************
 * @function name: ControlBoard::sendSwSpi()
 *
 * @param:
 *          const char *dio_clk
 *          const char *dio_write
 *          const char *dio_read
 *          quint32 edge - edge 0- rising 1- falling
 *          quint32 half_period - half period in uS
 *          quint32 size_in_bits - size in bits (byte array can be longer)
 *          const QByteArray &data - data as byte array in hex .
 *              Bits will be processed 0,1,2,..7,0,1,.....
 * @description:
 * @return: ( bool )
 ****************************************************************************/
bool ControlBoard::sendSwSpi(const char *dio_clk, const char *dio_write
                             , const char *dio_read, quint32 edge
                             , quint32 half_period, quint32 size_in_bits
                             , const QByteArray &data)
{
    char tmp[1000];
    SPRINTF(tmp,"%s,%s,%s,%1d,%04d,%04d,", dio_clk, dio_write
            , dio_read, edge, half_period, size_in_bits);
    return setData("swspi",QByteArray(tmp)+data.toHex());
}

bool ControlBoard::readSwSpi(QByteArray *pdata)
{
    QByteArray tmp;
    bool result= getData("swspi",&tmp);
    if(pdata) *pdata = QByteArray::fromHex(tmp);
    return result;
}

/****************************************************************************
 * @function name: ControlBoard::genClocksP()
 *
 * @param:
 *          const char *dio -
 *          quint32 count - count in clocks
 *          quint32 time - half period
 *          const QString unit - unit name 'us' or 'ms'
 * @description:
 * @return: ( bool )
 ****************************************************************************/
bool ControlBoard::genClocksP(const char *dio, quint32 count
                              ,quint32 time,const QString unit)
{
    QString tmp;
    if(unit=="ms"){
        tmp += QString("%1,1,").arg(dio);
    }
    else{
        tmp += QString("%1,0,").arg(dio);
    }
    while(count--)
    {
        tmp+=QString("%1").arg(time,2,16);
    }
    return transferData("set DIOSEQ2=" + tmp.toLatin1());
}

/****************************************************************************
 * @function name: ControlBoard::captureData()
 *
 * @param:
 *          const char *dio - DIO clock for capture.
 *                             Need to setup this port as input
 *          quint32 count - clocks count
 *          quint32 delay - delay after each capture
 *          quint32 trigger -trigger when start
 *          QByteArray *pdata - data pointer where to put result
 * @description:
 * @return: ( bool )
 ****************************************************************************/
bool ControlBoard::captureData(const char * /*dio*/, quint32 count
                               , quint32 delay, quint32 trigger, QByteArray *pdata)
{
    //"DIO??,?????,????"
    char tmp[100];
    SPRINTF(tmp, "%s,%04d,%04d",count,delay,trigger);
    bool result= transferData("set DIOSEQ52="+QByteArray(tmp));
    result &= transferData("get DIOSEQ52",pdata);
    return result;
}

/****************************************************************************
 * @function name: ControlBoard::transferScanChain()
 *
 * @param:char operation = 'r' -read 'w'=write
 *         Register *pchain
 *
 * @description:
 * @return: ( bool )
 ****************************************************************************/
bool ControlBoard::transferScanChain(const char *dio_clk, const char *dio_write
                                     , const char *dio_read,
                                     char operation, quint32 size, QByteArray &data, QByteArray *presult)
{
    bool result=false;
    char tmp[1000];
    const char function_name[]="scanchain";
    QByteArray hex_data;
    if(operation == 'r')
    {
        SPRINTF(tmp,"%s,%s,%s,%04d,read",dio_clk, dio_write, dio_read,size);
        setData(function_name,tmp);
        if(presult) {
            result=getData(function_name,&hex_data);
            *presult=QByteArray::fromHex(hex_data);
        }
    }    
    else {
        SPRINTF(tmp,"%s,%s,%s,%04d,%s", dio_clk, dio_write, dio_read, size
                , data.toHex().constData());
        setData(function_name,tmp);
        if(presult) {
            result=getData(function_name,&hex_data);
            *presult=QByteArray::fromHex(hex_data);
        }
        else result=true;
    }
    return result;
}


/****************************************************************************
 * @function name: ControlBoard::checkDevice()
 *
 * @param:
 *
 * @description:
 * @return: ( bool )
 ****************************************************************************/
bool ControlBoard::checkDevice()
{
    bool result=false;
    int retry=3;
    if(this->m_mode == MICROCONTROLLER )
    {
        if(mp_channel->isOpened() && !mp_channel->bytesAvailable())
        {
            QByteArray data;
            while(!result && retry--)
            {
                mp_channel->sendData(QByteArray(1,MCU_REQUEST));
                int timeout = 10 ;
                while(timeout--)
                {
                    if( (mp_channel->readData(data,1)==1)
                            && (data == QByteArray(1,MCU_ACK)))
                    {
                        result = true;
                        break;
                    }
                    msleep(10);
                }
            }
        }
    }
    else if(m_mode == SCRIPT || m_mode==MOCKUP)
    {
        result=true;
    }
    return result;
}

/****************************************************************************
 * @function name: ControlBoard::resetDevice()
 * @description: Resets device and wait for responding
 * @return: ( void )
 ****************************************************************************/
void ControlBoard::resetDevice()
{    
    log_message(LOG_MCU_INFO,"Board reset");
    mp_channel->sendData(QByteArray(MCU_RESET, 6));

    QByteArray rx ;
    QTime tmr;
    tmr.start();
    do
    {
        mp_channel->sendData(QByteArray("\x5",1));
        msleep(10);
        mp_channel->readData(rx,1);
    }while(rx != QByteArray("\x6",1) && tmr.elapsed()<30000);
    mp_channel->readAll(rx);
}

/****************************************************************************
 * @function name: ControlBoard::setLogMode()
 *
 * @param:
 *          bool on
 * @description:
 * @return: ( void )
 ****************************************************************************/
void ControlBoard::setLogMode(bool on)
{
    m_logMode = on;
}

/****************************************************************************
 * @function name: ControlBoard::logMode()
 * @param:
 * @description: if  log mode is on
 * @return: ( void )
 ****************************************************************************/

bool ControlBoard::logMode() const
{
    return m_logMode;
}

/****************************************************************************
 * @function name: ControlBoard::logMode()
 * @param:
 * @description: if  log mode is on
 * @return: ( bool )
 ****************************************************************************/
bool ControlBoard::checkSession()
{
    // TODO
    return true;
}

/****************************************************************************
 * @function name: ControlBoard::logMode()
 * @param:
 *      ControlBoard::Version *pdata - version data
 * @description: this will populate version data
 * @return: ( bool) true=ok
 ****************************************************************************/
bool ControlBoard::checkVersion()
{
    bool result =false;// TODO
    QByteArray version;
    QByteArray project;
    msleep(500);
    m_version.minor = 0;
    m_version.major = 0;
    m_project = "";
    result = MCU.transferData("get VER",&version);
    result &= MCU.transferData("*IDN?",&project, 500);
    m_project = project.replace("\n","");
    QRegExp rx("\\w+=([\\w\\d]+) (\\w+) (\\w+) (\\d+) (\\d+):(\\d+):(\\d+) (\\d+)");
    // 0x00010013 Mon Jul 11 11:17:44 2016
    if(rx.indexIn(version)>=0)
    {
        m_version.build = rx.cap(1).toInt(0,16);
        m_version.minor = rx.cap(1).toInt(0,16) & 0xFFFF;
        m_version.major = (rx.cap(1).toInt(0,16) >> 16) & 0xFFFF;
        m_version.timestamp =
                QDateTime::fromString(QString(rx.cap(2)+" "+rx.cap(3)+" "+rx.cap(4)+" "
                                              +rx.cap(5)+":"+rx.cap(6)+":"+rx.cap(7)+" "+rx.cap(8)
                                              )
                                      ,"ddd MMM dd hh:mm:ss yyyy"
                                      );

    }
    return result;
}


/****************************************************************************
 * @function name: ControlBoard::clearBuffers()
 *
 * @param:
 *
 * @description:
 * @return: ( void )
 ****************************************************************************/
void ControlBoard::clearBuffers()
{
    //m_data_in.clear();
    rx_buf.reset();
    tx_buf.reset();
    // TODO
}


/****************************************************************************
 * @function name: ControlBoard::initMockup()
 *
 * @param:
 *
 * @description:
 * @return: ( void )
 ****************************************************************************/
void ControlBoard::initMockup()
{
    qsrand(100);
}


/****************************************************************************
 * @function name: ControlBoard::readMockUp()
 *
 * @param:
 *          const QByteArray &data_out
 *          QByteArray *pdata_in
 * @description: read simulation function
 * @return: ( void )
 ***************************************************************************/
void ControlBoard::transferMockup(const QByteArray &data_out, QByteArray *pdata_in)
{
    //set
    if( data_out.contains("set") )
    {
        QList<QByteArray> l = QByteArray(data_out).replace("set ","")
                .replace("\n","").trimmed().split(' ');
        foreach(const QByteArray &ba, l)
        {
            *pdata_in += QByteArray(ba).remove(ba.indexOf('='),ba.size()-1) +":ok ";
        }
    }
    //get
    else
    {
        QList<QByteArray> l = QByteArray(data_out).replace("get ","")
                .replace("\n","").trimmed().split(' ');
        foreach(const QByteArray &ba, l)
        {
            quint32 random = qrand();
            if(ba.contains("DIO"))
            {
                random %= 1;
            }
            else if(ba.contains("AIN"))
            {
                random %= 1023;
            }
            *pdata_in += " "+ ba + "=" + QByteArray::number(random);
        }
        *pdata_in = pdata_in->trimmed();
    }
}


void ControlBoard::log_message( LogMessage message, const QString message_text)
{
    static QTime time = QTime::currentTime();
    const char LOG_TIME_FORMAT[]="hh:mm:ss.zzz";
    int color;
    int time_diff=time.msecsTo(QTime::currentTime());
    QString text;
    QString unit = "ms";
    if(logMode())
    {
        if(time_diff>1000) {time_diff/=1000;unit="s";}
        if(message == LOG_MCU_TX)
        {
            color =LOG_COLOR(Qt::blue);

            text = QString("TX(%1):%2")
                    .arg(QTime::currentTime().toString(LOG_TIME_FORMAT))
                    .arg(QString(message_text).replace("\n","&lt;cr&gt; "));
        }
        else if(message == LOG_MCU_RX)
        {
            color =LOG_COLOR(Qt::green);
            text = QString("RX(+%1%2):%3")
                    .arg(time_diff)
                    .arg(unit)
                    .arg(QString(message_text).replace("\n","&lt;cr&gt; "));
        }
        else if(message == LOG_MCU_INFO)
        {
            color = LOG_COLOR(Qt::darkGray);
            text = QString("INF(%1):%2")
                    .arg(QTime::currentTime().toString(LOG_TIME_FORMAT))
                    .arg(message_text);
        }
        else if(message == LOG_MCU_ERROR)
        {
            color = -1000;
            text = QString("RX_ERR(+%1%2):%3")
                    .arg(time_diff)
                    .arg(unit)
                    .arg(message_text);
        }
        emit signal_log(color,text);
        time = QTime::currentTime();
    }
}


bool ControlBoard::setDIO(const char *dio, quint32 val, bool send_immediately)
{
    static QByteArray cache;
    bool result=false;
    if(!cache.startsWith("set")) cache+= "set ";
    cache += QString("%1=%2 ").arg(dio).arg(val).toLatin1();
    if(send_immediately) {result = transferData(cache);cache.clear();}
    return result;
}



bool ControlBoard::setDIODIR(const char *dio, quint32 val, bool send_immediately)
{
    static QByteArray cache;
    bool result=false;
    if(!cache.startsWith("set")) cache+= "set ";
    cache += QString("%1DIR=%2 ").arg(dio).arg(val);
    if(send_immediately) {
        result = transferData(cache);
        cache.clear();
    }
    return result;
}

bool ControlBoard::getDIO(const char *dio, qint32 *pval)
{
    return getData(dio,pval);
}

void ControlBoard::sendBreak()
{
    if(isConnected()){
         mp_channel->sendData(QByteArray("\x15",1));
    }
}

/****************************************************************************
 * @function name: ControlBoard::adcMeasure()
 *
 * @param:
 *          const char *dio_clock - dio clock
 *          const char *dio_read - dio where capture data
 *          quint32 t1 -  t1 time
 *          quint32 t2 - t2 time
 *          quint32 count - count of packets
 *          QList<int> *pdata - data pointer for result
 * @description: This will send request for ADC read. Pins should be set by
 *              DIO?DIR command
 * @return: ( bool ) - success
 ***************************************************************************/
bool ControlBoard::adcMeasure(const char *dio_clock, const char *dio_read
                              , quint32 t1, quint32 t2, quint32 count,QList<int> *pdata)
{
    bool result;
    int i=0;
    char tmp[100];
    QByteArray data;
    SPRINTF(tmp,"%s,%s,%02d,%02d,%04d",dio_clock,dio_read,t1,t2,count);
    setData("DIOSEQ9",QByteArray(tmp));
    result= getData("DIOSEQ9",&data);
    data = QByteArray::fromHex(data);
    if(result && data.size() == count && pdata)
    {
        pdata->append(data.at(i++));
    }
    return result;
}

bool ControlBoard::function(const char *name, const QByteArray &data, quint32 timeout
                            , QByteArray *presult1
                            , QByteArray *presult2
                            , QByteArray *presult3
                            , QByteArray *presult4
                            , QByteArray *presult5)
{
    bool result=false;
    QByteArray request;
    QByteArray answer;
    QByteArray *res[5];
    int count=0;
    do{
        if(presult1 != NULL) {res[count++]=presult1;}
        else break;
        if(presult2 != NULL) {res[count++]=presult2;}
        else break;
        if(presult3 != NULL) {res[count++]=presult3;}
        else break;
        if(presult4 != NULL) {res[count++]=presult4;}
        else break;
        if(presult5 != NULL) {res[count++]=presult5;}
    }
    while(0);
    request = "set " +QByteArray(name) + "=" + data;
    if(transferData(request, &answer, timeout))
    {
        //  to mcu => make
        for(int i=0;i<count;i++)
        {
            request = "get " + QByteArray(name) + "=" +QByteArray::number(i+1);
            bool trf_ok = transferData(request, &answer);
            if( trf_ok && extractText(answer, name, &answer))
            {
                *res[i] = QByteArray::fromHex(answer);
                result= true;
            }
            else {result=0; break;}
        }
    }
    return result;
}

double ControlBoard::ainVoltage(const char *ain)
{
    qint32 value;
    getData(ain,&value);    
    return (m_ain_vref/ADC_RESOLUTION)*value;
}


ControlBoard& ControlBoard::operator [](const char *dio)
{
    m_cur_dio = QString(dio);
    if(!m_cur_dio.startsWith("DIO") && !m_cur_dio.startsWith("AIN"))
    {
        TRACE("MCU[%s] - fail",dio);
    }
    return *this;
}

void ControlBoard::operator =(const int val)
{
    if(m_cur_dio.startsWith("DIO"))
        setData(m_cur_dio,val);
}

void ControlBoard::operator =(const QByteArray &seq)
{
    if(m_cur_dio.startsWith("DIO"))
    {
        QByteArray data;
        data = "set ";
        foreach(const QChar &c, seq)
        {
            data += m_cur_dio+"="+c +" ";
        }
        transferData(data);
    }    
}



ControlBoard::operator int()
{
    qint32 value=0;
    if(m_cur_dio.startsWith("DIO"))
    {
        getData(m_cur_dio,&value);
    }
    else if(m_cur_dio.startsWith("AIN"))
    {
        getData(m_cur_dio,&value);
    }
    return value;
}


bool ControlBoard::sendByProtocol(const QString &/*protocol*/, const QByteArray &/*data*/, QByteArray * /*presult*/)
{
    return false;
}

void ControlBoard::startStream(const QString &cmd, Applet *papplet)
{    
    connect(this,SIGNAL(signal_captureData(QByteArray))
            ,papplet,SLOT(slot_capturedData(QByteArray)));
    mp_capture_applet = papplet;
    log_message(LOG_MCU_INFO,"Start stream");
    log_message(LOG_MCU_INFO,"RTS=1");
    log_message(LOG_MCU_INFO,cmd);
    m_capture_command = cmd;
    if(! cmd.contains('\n')) m_capture_command += "\n";
    mutex.lock();
    m_capture_mode = true;
    mutex.unlock();
}

void ControlBoard::stopStream()
{    
    mutex.lock();
    m_capture_mode = false;
    mutex.unlock();
    disconnect(mp_capture_applet,SLOT(slot_capturedData(QByteArray)));
    log_message(LOG_MCU_INFO,"RTS=0");
    log_message(LOG_MCU_INFO,"Stop stream");
}


bool ControlBoard::configAin(quint32 options)
{
    bool result=false;
    if(options & VREF_EXTERNAL)
    {
        result = transferData("set AINCONF=1\n");
    }
    else if(options & VREF_INTERNAL1V)
    {        
        result=transferData("set AINCONF=0\n");
        m_ain_vref = 1.0f;
    }    
    return result;
}
