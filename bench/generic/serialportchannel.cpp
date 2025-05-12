#include "compiler.h"
#include "serialportchannel.h"
#include "windows.h"
#include <QDebug>



/****************************************************************************
 * @function name:SerialPortChannel
 ****************************************************************************/
SerialPortChannel::SerialPortChannel()
{
     m_Name = "";
     m_IsOpened = false;     
}


/****************************************************************************
 * @function name: SerialPortChannel
 * @param:
 *     const QString &portname
 * @description:
 ****************************************************************************/
SerialPortChannel::SerialPortChannel(const QString &portname)
{ 
    m_Name =  portname;
    m_IsOpened = false;
}

/****************************************************************************
 * @function name: SerialPortChannel::isOpened()
 * @description:
 * @return: ( bool ) true - opened
 ****************************************************************************/
bool SerialPortChannel::isOpened() const
{
    return m_IsOpened;
}

/****************************************************************************
 * @function name: &SerialPortChannel::name()
 * @param:
 *             void
 * @description:
 * @return: ( const QString) - name of channel
 ****************************************************************************/
const QString &SerialPortChannel::name() const
{
    return m_Name;
}

/****************************************************************************
 * @function name: SerialPortChannel::setName - ---
 * @param:
 *     const QString name
 * @description:
 * @return: ( void )
 ****************************************************************************/
void SerialPortChannel::setName(const QString name)
{
    if(!isOpened())
        m_Name = name;
}



/****************************************************************************
 * @function name: SerialPortChannel::open_channel - ---
 * @param:
 *   const QString &name,quint32 baudrate
 * @description:
 * @return: ( bool )
 ****************************************************************************/
bool SerialPortChannel::open(const QString &name,quint32 baudrate)
{
    if (!name.isEmpty()) m_Name = name;
    if (!isOpened() && !m_Name.isEmpty())
    {
        int   bStatus;
        DCB          comSettings;          // Contains various port settings
        COMMTIMEOUTS CommTimeouts;
        m_handle =  CreateFileA(name.toAscii(),
                                GENERIC_READ | GENERIC_WRITE, // for reading and writing
                                0,                            // exclusive access
                                NULL,                         // no security attributes
                                OPEN_EXISTING,
                                FILE_ATTRIBUTE_NORMAL,
                                NULL);

        // Open COM port
        if (m_handle == INVALID_HANDLE_VALUE)
        {
            // error processing code goes here
            m_handle = NULL;
        }
        else
        {            
            // Set timeouts in milliseconds
            CommTimeouts.ReadIntervalTimeout         = 0;
            CommTimeouts.ReadTotalTimeoutMultiplier  = 1;
            CommTimeouts.ReadTotalTimeoutConstant    = 5; // Read time out 5ms.
            CommTimeouts.WriteTotalTimeoutMultiplier = 1;
            CommTimeouts.WriteTotalTimeoutConstant   = 5; // Write time out 100ms.

            SetCommTimeouts(m_handle, &CommTimeouts);
            // Set Port parameters.
            // Make a call to GetCommState() first in order to fill
            // the comSettings structure with all the necessary values.
            // Then change the ones you want and call SetCommState().
            GetCommState(m_handle, &comSettings);


            comSettings.BaudRate = (DWORD)baudrate;
            comSettings.StopBits = ONESTOPBIT;
            comSettings.ByteSize = 8;
            comSettings.Parity   = NOPARITY;   // No Parity
            comSettings.fParity  = FALSE;
            comSettings.fRtsControl = RTS_CONTROL_DISABLE;
            bStatus = SetCommState(m_handle, &comSettings);
            if (bStatus == 0)
            {
                // error processing code goes here
                qWarning()<< "open_port: Unable to open ";
                close();
            }
            m_Name = name;
            m_IsOpened = true;
        }
    }
    return m_IsOpened;
}


/****************************************************************************
 * @function name: SerialPortChannel::close - ---
 * @param:
 *     void
 * @description:
 * @return: ( void )
 ****************************************************************************/
void SerialPortChannel::close(void)
{
    CloseHandle(m_handle);
    m_IsOpened = false;
    m_handle = NULL;
}


/****************************************************************************
 * @function name: SerialPortChannel::readAll()
 * @param:
 *    QByteArray &data  - where to place data
 * @description:
 * @return: ( qint32 ) - data read from port
 ****************************************************************************/
qint32 SerialPortChannel::readAll(QByteArray &data)
{
    qint32 bytes_to_read = bytesAvailable();
    return readData(data, (bytes_to_read>RX_BUF_SIZE)?RX_BUF_SIZE:bytes_to_read);
}


/****************************************************************************
 * @function name: SerialPortChannel::sendData - ---
 * @param:
 *    const QByteArray &data 
 * @description:
 * @return: ( qint32 )
 ****************************************************************************/
qint32 SerialPortChannel::sendData(const QByteArray &data)
{
    const char *pdata = data.constData();
    DWORD bytes;
    qint32 bytesWritten=0;
    if(WriteFile(m_handle,              // Handle
              (LPCVOID) pdata,      // Outgoing data
              (DWORD) data.size(),              // Number of bytes to write
              (PDWORD)&bytes,  // Number of bytes written
              NULL))
    {
        bytesWritten = static_cast<qint32>(bytes);
    }
    return bytesWritten ;
}


/****************************************************************************
 * @function name: SerialPortChannel::readData - ---
 * @param:
 *           QByteArray &data
 *           qint32 sizeToRead
 * @description:
 * @return: ( qint32 )
 ****************************************************************************/
qint32 SerialPortChannel::readData(QByteArray &data, qint32 sizeToRead )
{
    qint32 result;
    DWORD max_bytes = (DWORD)sizeToRead;
    static char pdata[RX_BUF_SIZE];
    DWORD bytes;
    result = ReadFile(m_handle,   // Handle
             (PVOID) pdata,            // Incoming data
             (DWORD) max_bytes ,      // Number of bytes to read
             (PDWORD)&bytes,           // Number of bytes read
             NULL);
    if(result != 0)
    {
        result = static_cast<qint32>(bytes);
    }
    else result = -1;
    data = QByteArray(pdata, result);

    return result;
}


/****************************************************************************
 * @function name: SerialPortChannel::bytesAvailable - ---
 * @description:
 * @return: ( qint32 )
 ****************************************************************************/
qint32 SerialPortChannel::bytesAvailable(void)
{
    COMSTAT stat;
    DWORD dwErrors;
    if (!ClearCommError(m_handle, &dwErrors, &stat))
    {
        return 0;
    }
    qint32 bytes = static_cast<qint32>(stat.cbInQue);
    return bytes;
}

void SerialPortChannel::setRTS(bool rts)
{
    if(rts) EscapeCommFunction(m_handle, CLRRTS);
    else EscapeCommFunction(m_handle, SETRTS);
}

bool SerialPortChannel::getCTS()
{
    // TODO
    return false;
}


/****************************************************************************
 * @function name: SerialPortChannel::flush - ---
 * @description:
 * @return: ( void )
 ****************************************************************************/
void SerialPortChannel::flush(void)
{
    FlushFileBuffers(m_handle);
}

/****************************************************************************
 * @function name: SerialPortChannel::enumDevices()
 * @param:
 *      QStringList *pchannels_list
 * @description:
 * @return: ( qint32 )
 ****************************************************************************/
qint32 SerialPortChannel::enumDevices(QStringList *pchannels_list)
{
    char name[20];
    HANDLE tmp_handle;
    if(pchannels_list)
        pchannels_list->clear();
    for (qint32 i = 1; i < 200; i++)    //pk
    {        
        SPRINTF(name, "\\\\.\\COM%u", i);

        tmp_handle =  CreateFileA(name,
                                  GENERIC_READ , // for reading and writing
                                  0,                            // exclusive access
                                  NULL,                         // no security attributes
                                  OPEN_EXISTING,
                                  FILE_ATTRIBUTE_NORMAL,
                                  NULL);
        if (tmp_handle != INVALID_HANDLE_VALUE)
        {
            pchannels_list->append(QString(name));
            CloseHandle(tmp_handle);
        }

    }

    return pchannels_list->size();
}
