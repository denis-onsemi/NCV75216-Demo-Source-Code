

#include "mppboard.h"
#include "mainapp.h"
#include <QDebug>
#include <QFileDialog>
#include <QMessageBox>
#include <QElapsedTimer>
#include <QUdpSocket>
#include <QHostInfo>

static QVariant StringToQVariant(const QString &s){
    QVariant v;
    if(s.startsWith('{') && s.endsWith('}')){
        QVariantMap m;
        QRegExp rx("([\\w]+=[\\w\\.\\-\"]");
        int pos =0;
        while(rx.indexIn(s.mid(1,s.count()-2),pos)>=0){
            m[rx.cap(1)] = rx.cap(2);
            pos += rx.matchedLength();
        }
        v = m;
    }
    else if(s.startsWith('[') && s.endsWith(']')){
        QStringList l = s.mid(1,s.count()-2).split(',');
        v = l;
    }
    else{
        v = s;
    }
    return v;
}

static QString QVariantToString(const QVariant &v ){
    QString txt;
    if(v.type() == QVariant::ByteArray ){
        txt+="[";
        for(int i=0;i<v.toByteArray().count();i++){
            txt += QString("%1,").arg((unsigned int)v.toByteArray()[i],2,16,QChar('0'));
        }
        if(txt.endsWith(',')) txt.chop(1);
        txt +="]";
    }
    else if(v.type() == QVariant::StringList ){
        txt+="[";
        for(int i=0;i<v.toStringList().count();i++){
            txt += QString("%1,").arg(v.toStringList()[i]);
        }
        if(txt.endsWith(',')) txt.chop(1);
        txt +="]";
    }
    else if(v.type() == QVariant::Map){
        txt += "{";
        foreach(const QString &key, v.toMap().keys()){
            txt += QString("%1=%2,").arg(key).arg(v.toMap()[key].toString());
        }
        if(txt.endsWith(',')) txt.chop(1);
        txt += "}";
    }
    else if(v.type() == QVariant::Bool)
    {
        txt = QString::number(v.toInt());
    }
    else{
        txt = v.toString();
    }
    return txt;
}


/****************************************************************************
 * @function name: CONSTRUCTOR
 *
 ****************************************************************************/
MppBoard::MppBoard()
    :QObject()
{
    m_logMode = false;
    m_finish = false;
    m_timeout = 2000;
    m_suspend=false;
    m_connected = false;
    mp_socket  = new QTcpSocket();
    mp_socket->setSocketOption(QAbstractSocket::KeepAliveOption,1);
    mp_socket->setSocketOption(QAbstractSocket::LowDelayOption,1);

    connect(mp_socket,SIGNAL(stateChanged(QAbstractSocket::SocketState))
            ,this, SLOT(slot_stateChanged(QAbstractSocket::SocketState)));

}


/****************************************************************************
 * @function name: DESTRUCTOR
 *
 ****************************************************************************/
MppBoard::~MppBoard()
{
    m_finish = true;
}

/****************************************************************************
 * @function name: MppBoard::connect()
 *
 * @param:
 *    const QString & port - port name
 * @description: connects mcu .
 * 1. open channel , 2. controls if there is some board connected
 * @return: ( void )
 ****************************************************************************/
bool MppBoard::connectDevice(const QString & ip_and_socket, bool reset)
{
    QRegExp rx("([\\w]+)\\(([\\d\\.]+)\\)");
    m_connected = false;

    if(rx.exactMatch(ip_and_socket)){

        mp_socket->connectToHost(rx.cap(2), 4000);

        if( mp_socket->waitForConnected(500) )
        {
            m_connected = checkConnected() ;

            if(m_connected) {
                if(reset) mp_socket->write("RESET\n");
                emit signal_connected();
            }
            else{
                QMessageBox::critical(0,"Error","Can't connect MPP Server . There is only one connection allowed. ");
            }
        }

        if(!m_connected)
        {
            mp_socket->disconnectFromHost();
            m_connected = false;
        }
    }
    return m_connected ;
}


/****************************************************************************
 * @function name: MppBoard::slot_stateChanged()
 * @param:
 *    QAbstractSocket::SocketState s
 * @description:
 * @return: ( void )
 ****************************************************************************/
void MppBoard::slot_stateChanged(QAbstractSocket::SocketState s)
{

    switch(s)
    {
    case QAbstractSocket::UnconnectedState:
    case QAbstractSocket::ClosingState    :
        emit signal_disconnected();
        break;

    case QAbstractSocket::ConnectedState:

        break;

    default:
    case QAbstractSocket::ListeningState:

    case QAbstractSocket::BoundState:
    case QAbstractSocket::ConnectingState:
    case QAbstractSocket::HostLookupState:
        break;
    }
}



/****************************************************************************
 * @function name: MppBoard::disconnectDevice()
 *
 * @param:
 *           void
 * @description:
 * @return: ( void)
 ****************************************************************************/
void MppBoard::disconnectDevice()
{     
    if( isSocketConnected() ){
        mp_socket->disconnectFromHost();
    }
}


/****************************************************************************
 * @function name: MppBoard::availablePorts()
 *
 * @param:
 *           QStringList *plist
 * @description:
 * @return: ( void)
 ****************************************************************************/
void MppBoard::availablePorts(QStringList *plist)
{

    QStringList ip_to_check = QStringList()<<"255.255.255.255"<<"192.168.7.2";
    foreach(const QString &ipaddr, ip_to_check)
    {
        // todo find available devices
        QUdpSocket udp;
        udp.bind(40001);


        /* send the data to the TCP-server */
        qint64 length = udp.writeDatagram("who?", QHostAddress(ipaddr), 40000);
        if(length<=0) {
            QMessageBox::critical(0,"Error","Error scanning MPP boards due to UDP broadcast error");
            return ;
        }

        QTime t;
        t.start();
        while(t.elapsed()<500   )
        {
            if(udp.waitForReadyRead(10)) {
                char data[100] ;
                QHostAddress sender; // maak een nieuwe QHostAddress aan
                quint16 senderPort; // maak een variabele aan voor de poortnummer van de zender
                int a = udp.readDatagram(data, 100, &sender, &senderPort); // lees het UDP-pakket en stop het in de QByteArray
                if(data[0]==0 && a>1 )
                {
                    QRegExp rx("([\\d+.]+.[\\d]+.[\\d]+.[\\d]+)\\s+-\\s+([\\w]+)");
                    if(rx.indexIn(QByteArray(&data[1],a-1))>=0){
                        plist->append(QString("%1(%2)").arg(rx.cap(2)).arg(rx.cap(1)));
                    }
                }

            }
            else break;
        }
   }

}




/****************************************************************************
 * @function name: MppBoard::isConnected()
 * @description: if board is connected
 * @return: ( bool )
 ****************************************************************************/
bool MppBoard::isConnected()
{
    return m_connected ;
}

/****************************************************************************
 * @function name: MppBoard::transferData()
 * @param:
 *          const QByteArray &data_write
 *          QByteArray *pdata_read
 *          int to -timeout
 *          int *time_elapsed - time elapsed
 * @description:
 * @return: ( bool )
 ****************************************************************************/
bool MppBoard::transferData(const QByteArray &data_write, QByteArray *pdata_read,int to,int *time_elapsed)
{
    bool result;
    QByteArray answer;
    QElapsedTimer tmr;
    emit signal_tx();
    log_message(LOG_MPP_TX, QString(data_write));

    mp_socket->write(data_write);
    to = (to >timeout()) ?to:timeout();

    if( !mp_socket->waitForBytesWritten(to) ) {
        mp_socket->disconnectFromHost();
        return false;
    }
    tmr.start();
    if(mp_socket->waitForReadyRead(to))
    {
        emit signal_rx();
        answer = mp_socket->read(2048);
    }
    else {
        result = false;
    }
    if(time_elapsed) *time_elapsed = tmr.elapsed();
    log_message(LOG_MPP_RX,QString(answer));
    if(pdata_read) *pdata_read = answer;

    return result;
}


/****************************************************************************
 * @function name: MppBoard::setData()
 *
 * @param:
 *          const ItemsList &data
 *          int to
 *          int *time_elapsed
 * @description:
 * @return: ( bool )
 ****************************************************************************/
bool MppBoard::setData(const ItemsList &data, int to, int *time_elapsed)
{
    QStringList control_names;
    QByteArray raw_data;
    raw_data+="set";
    foreach(const Item &item, data){
        raw_data += QString(" %1=%2").arg(item.name()).arg(item.valueToString());
        control_names.append(item.name());
    }
    raw_data+="\n";

    transferData(raw_data,&raw_data,to,time_elapsed);

    const QRegExp  item_and_value("([\\w]+):([\\w]+)");
    bool err = true;
    int pos = 0;
    int i=0;
    while ((pos = item_and_value.indexIn(raw_data, pos)) != -1)
    {
        if(item_and_value.capturedTexts().count() == 3 )
        {
            if(item_and_value.cap(2) == "err" || item_and_value.cap(1) != control_names[i])
            {
                err=true;
                break;
            }
            else err=false;
        }
        pos += item_and_value.matchedLength();
        i++;
    }

    return !err;
}


/****************************************************************************
 * @function name: MppBoard::getData()
 *
 * @param:
 *          ItemsList *pdata
 *          int to
 *          int *time_elapsed
 * @description:
 * @return: ( bool )
 ****************************************************************************/

bool MppBoard::getData(ItemsList *pdata, int to, int *time_elapsed)
{       
    QByteArray raw_data;
    QStringList control_names;
    raw_data+="get";
    foreach(const Item &item, *pdata){
        raw_data += QString(" %1").arg(item.name());
        control_names.append(item.name());
    }
    raw_data+="\n";

    transferData(raw_data,&raw_data, to, time_elapsed);

    const QRegExp  item_and_value("([\\w]+)[:=]([\\w\\.\\-\\]\\[\\{\\},]+)");
    bool err =true;
    int pos = 0;
    int i=0;
    while ((pos = item_and_value.indexIn(raw_data, pos)) != -1)
    {
        if(item_and_value.capturedTexts().count() == 3
                && (i<control_names.count())
                && (control_names[i] == item_and_value.cap(1) ))
        {
            // error setting/getting
            if(item_and_value.cap(2) == "err"|| item_and_value.cap(1) != control_names[i])
            {
                err=true;
                break;
            }
            //valid value
            else {
                (*pdata)[i].valueFromString(item_and_value.cap(2));
                err = false;
            }
        }
        pos += item_and_value.matchedLength();
        i++;
    }

    return !err;
}

bool MppBoard::setData(Register &reg, int timeout, int *time_elapsed)
{
    MppBoard::ItemsList l;
    for(int i=0;i<reg.size();i++){
        l.append(MppBoard::Item(reg.bitAt(i)->name(),reg.bitAt(i)->value()));
    }
     return setData(l);
}

bool MppBoard::getData(Register *preg, int timeout, int *time_elapsed)
{
    MppBoard::ItemsList l;
    for(int i=0;i<preg->size();i++){
        l.append(MppBoard::Item(preg->bitAt(i)->name()));
    }
    bool r = getData(&l);
    for(int i=0;i<preg->size();i++){
        preg->bitAt(i)->setValue(l.at(i).value().toInt());
    }
    return r;
}

bool MppBoard::setDataItem(const Item &item, int to, int *time_elapsed)
{
    QString control_name = item.name();
    QByteArray raw_data = QString("set %1=%2\n").arg(item.name()).arg(item.valueToString()).toLatin1();
    transferData(raw_data, &raw_data, to, time_elapsed);
    const QRegExp  item_and_value("([\\w]+):([\\w]+)");
    bool err =true;
    if(item_and_value.indexIn(raw_data, 0)>=0 && item_and_value.capturedTexts().count() == 3)
    {
        if(item_and_value.cap(2) == "err" || item_and_value.cap(1) != control_name)    {
            err=true;
        }
        else err= false;
    }
    return !err;
}

bool MppBoard::getDataItem(MppBoard::Item *pitem, int to, int *time_elapsed)
{
    QString control_name = pitem->name();
    QByteArray raw_data = QString("get %1\n").arg(pitem->name()).toLatin1();
    transferData(raw_data,&raw_data, to, time_elapsed);

    const QRegExp  item_and_value("([\\w]+)[:=]([\\w\\.\\-\\]\\[\\{\\},]+)");
    bool err =true;
    if(item_and_value.indexIn(raw_data, 0)>=0 && (item_and_value.capturedTexts().count() == 3) )
    {
        // error setting/getting
        if(item_and_value.cap(2) == "err" || item_and_value.cap(1) != control_name)
        {
            err=true;
        }
        //valid value
        else {
            pitem->valueFromString(item_and_value.cap(2));
            err = false;
        }
    }

    return !err;
}

bool MppBoard::runFunction(const QString &funcname, const QVariant  &params,QVariant *res, int to, int *time_elapsed)
{
    QByteArray raw_data = QString("run %1=%2\n").arg(funcname).arg(QVariantToString(params)).toLatin1();
    transferData(raw_data, &raw_data, to, time_elapsed);
    const QRegExp  item_and_value("([\\w]+)[:=]([\\w\\.\\-\\]\\[\\{\\},]+)");
    bool err =true;
    if(item_and_value.indexIn(raw_data, 0)>=0 && item_and_value.capturedTexts().count() == 3)
    {
        if(item_and_value.cap(2).toUpper() == "ERR" || item_and_value.cap(1).toUpper() != funcname.toUpper())    {
            err=true;
            QMessageBox::critical(0,"Error",QString("Error running %1\nServer:%2").arg(funcname).arg(QString(raw_data)));
        }
        else{
            if(res) *res = StringToQVariant(item_and_value.cap(2));
            err = false;
        }
    }
    return !err;
}

bool MppBoard::restart()
{
    return mp_socket->write("RESTART\n");
}

bool MppBoard::reset()
{
    return mp_socket->write("RESET\n");
}

bool MppBoard::shutdown()
{
    return mp_socket->write("SHUTDOWN\n");
}




/****************************************************************************
 * @function name: MppBoard::logMode()
 * @param:
 *      MppBoard::Version *pdata - version data
 * @description: this will populate version data
 * @return: ( bool) true=ok
 ****************************************************************************/
bool MppBoard::checkVersion()
{
    Item i("VER");
    bool result =getDataItem(&i);
    m_version = i.valueToString();

    return result;
}





void MppBoard::log_message( LogMessage message, const QString message_text)
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
        if(message == LOG_MPP_TX)
        {
            color =LOG_COLOR(Qt::blue);

            text = QString("TX(%1):%2")
                    .arg(QTime::currentTime().toString(LOG_TIME_FORMAT))
                    .arg(QString(message_text).replace("\n","&lt;cr&gt; "));
        }
        else if(message == LOG_MPP_RX)
        {
            color =LOG_COLOR(Qt::green);
            text = QString("RX(+%1%2):%3")
                    .arg(time_diff)
                    .arg(unit)
                    .arg(QString(message_text).replace("\n","&lt;cr&gt; "));
        }
        else if(message == LOG_MPP_INFO)
        {
            color = LOG_COLOR(Qt::darkGray);
            text = QString("INF(%1):%2")
                    .arg(QTime::currentTime().toString(LOG_TIME_FORMAT))
                    .arg(message_text);
        }
        else if(message == LOG_MPP_ERROR)
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

bool MppBoard::checkConnected()
{
    return checkVersion();
}




MppBoard& MppBoard::operator [](const char *dio)
{
    m_cur_dio = QString(dio);
    if(!m_cur_dio.startsWith("DIO") && !m_cur_dio.startsWith("AIN"))
    {
        TRACE("MPP[%s] - fail",dio);
    }
    return *this;
}

void MppBoard::operator =(const int val)
{
    Item item(m_cur_dio,val);
    setDataItem(item);
}

void MppBoard::operator =(const QByteArray &seq)
{
    QByteArray data;
    data = "set ";
    foreach(const QChar &c, seq)
    {
        data += m_cur_dio+"="+c +" ";
    }
    transferData(data);
}

MppBoard::operator int()
{
    qint32 value=0;
    Item i(m_cur_dio) ;
    getDataItem(&i);
    value = i.value().toInt();
    return value;
}


QString MppBoard::Item::valueToString() const
{
   return QVariantToString(this->second);
}

void MppBoard::Item::valueFromString(const QString &txt)
{
    second = StringToQVariant(txt);
}
