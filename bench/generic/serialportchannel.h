#ifndef SERIALPORTCHANNEL_H
#define SERIALPORTCHANNEL_H
#include <QString>
#include "windows.h"
#include <QStringList>

#define RX_BUF_SIZE      (4096u)
#define DEFAULT_BAUDRATE (115200ul)
/*channel  interface class*/
class SerialPortChannel
{

public:
    SerialPortChannel();
    SerialPortChannel(const QString &portname);

    const QString &name() const;
    /* sets name of channel */
    void setName(const QString name);
    /* tries to open channel */
    bool open(const QString &name=QString(),quint32 baudrate=DEFAULT_BAUDRATE);
    /* if channel is allrady exclusive opened*/
    bool isOpened() const;
    /* close channel*/
    void close(void);
    /* sends data to channel if sizeToSend = 0 - sends all bytearray,return size written */
    qint32 sendData(const QByteArray &data);
    /* reads data from channel to bytearray, return size read */
    qint32 readData(QByteArray &data, qint32 sizeToRead );
    /* reads all data from channel */
    qint32 readAll(QByteArray &data);
     /* bytes available to read in channel */
    qint32 bytesAvailable(void);
    void setRTS(bool rts);
    bool getCTS();
    /* flush all data in channel(deletes tx,rx buffer) */
    void flush(void);
     /* enumerating devices.return string list of channels */
    static qint32 enumDevices(QStringList *pchannels_list);    

private:
    HANDLE      m_handle;   // Handle COM port
    bool m_IsOpened;    
    bool m_parity;
    qint32 m_timeout;
    QString m_Name;

};

#endif // SERIALPORTCHANNEL_H
