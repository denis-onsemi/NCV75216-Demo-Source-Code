/**
 * @file:ftdispichannel.cpp   -
 * @description: FTDI chip communication functionality
 * @project: BENCH OnSemiconductor
 * @date: 2014\11\27 13-48-31
 *
 */

#include <QThread>
#include "ftdispichannel.h"
#include <QDebug>
#include <logs.h>
#include <QTime>




/****************************************************************************
 * @function name: CONSTRUCTOR
 ****************************************************************************/
FtdiSpiChannel::FtdiSpiChannel():
    QThread(0)
{  
    m_autoconnector_finish = false;
    m_autoconnector_suspend = true;
    m_isOpened = false;
    /* default settings*/
    configuration.mode = FT2232_MSB_FALLING_EDGE;
    configuration.readTimeout = FTDI_READ_TIMEOUT;
    configuration.writeTimeout = FTDI_WRITE_TIMEOUT;
    configuration.latencyTimer = FTDI_LATENCY_TIMER;
    configuration.negativeCS = true;
    configuration.lengthInBits = true;
    configuration.clockPeriod = FTDI_SPI_FREQUENCY;
    configuration.normal_DO_state = true;
    m_ibuffer_index =0;
    m_obuffer_index =0;
    m_log_mode = false;
    start(QThread::IdlePriority);
}


/****************************************************************************
 * @function name: DESTRUCTOR
 ****************************************************************************/
FtdiSpiChannel::~FtdiSpiChannel()
{          
    close();
    m_autoconnector_finish = true;
    terminate();
}

/****************************************************************************
 * @function name: FtdiSpiChannel::isOpened()
 * @param:   void
 * @description: this fnction returns only state that should be.
 *              If you use autorefunctionality.
 *              Control Thread will continuosly try to restore connection
 *              if port is physically not connected
 * @return: ( bool ) true - opened
 ****************************************************************************/
bool FtdiSpiChannel::isOpened() const
{
    return m_isOpened;
}


/****************************************************************************
 * @function name: FtdiSpiChannel::close()
 * @param:     void
 * @description: This closes connection channel.
 *                  Control thread will give up to restore connection
 * @return: ( void )
 ****************************************************************************/
void FtdiSpiChannel::close()
{    
    m_autoconnector_suspend =  true;
    if(m_handle)
    {
        FT_SetBitMode(m_handle, 0x00, FTDI_RESET );
        FT_Close(m_handle);
        m_handle =  NULL;
        m_number = -1;
        m_isOpened =  false;
        emit signal_StateChanged(0);
        log_message(LOG_FTDI_INFO, "Connected");
    }
}


/****************************************************************************
 * @function name: FtdiSpiChannel::bytesAvailable()
 * @param: void
 * @description: returns bytes available to read from FTDI channel
 * @return: ( qint32 ) - 0 - no bytes to read
 *                      -1 port is currently closed
 *                      >0 number of bytes to read
 ****************************************************************************/
qint32 FtdiSpiChannel::bytesAvailable(void)
{
    qint32 result=-1;
    DWORD bytesInQueue =0;
    if( isOpened() )
    {
        if(FT_GetQueueStatus(m_handle, &bytesInQueue) == FT_OK )
        {
            return (qint32)bytesInQueue;
        }
    }
    return result;
}


/****************************************************************************
 * @function name: FtdiSpiChannel::setConfiguration()
 * @param:
 *       FtdiSpiChannel::Config *pconfig -pointer to struct with data
 * @description:  - sets configuration to module.
 *                  This function require to close and open port
 *                  if issued while running
 * @return: ( void )
 ****************************************************************************/
void FtdiSpiChannel::setConfiguration(FtdiSpiChannel::Config *pconfig)
{
    if(pconfig)
    {
        configuration =  *pconfig;
    }
}


/****************************************************************************
 * @function name: FtdiSpiChannel::getConfiguration()
 * @param:
 *   FtdiSpiChannel::Config *pconfig  -pointer to configuration struct
 * @description:    - reads current configuration
 * @return: ( void )
 ****************************************************************************/
void FtdiSpiChannel::getConfiguration(FtdiSpiChannel::Config *pconfig)
{
    if(pconfig)
    {
        *pconfig = configuration;
    }
}


/****************************************************************************
 * @function name: FtdiSpiChannel::addToBuffer()
 * @param:
 *          quint8 byte - byte to add to output buffer
 * @description: Adds byte to buffer . Use sendToFtdi
 *                  function to physically send data to FTDI chip
 * @return: ( void )
 ****************************************************************************/
void FtdiSpiChannel::addToBuffer(quint8 byte)
{
    m_obuffer[m_obuffer_index++] = byte;
}


/****************************************************************************
 * @function name: FtdiSpiChannel::open()
 * @param:
 *    qint32 number -id  of FTDI from list
 *                    (list can be obtained by enumDevices())
 * @description:
 * @return: ( bool ) true - is success
 ****************************************************************************/
bool FtdiSpiChannel::open(qint32 number)
{   
    DWORD bytesInQueue;
    m_autoconnector_suspend =  true;
    if( number >= 0)
    {
        if(! isOpened()  && (FT_Open(number, &m_handle) == FT_OK) )
        {
            m_ibuffer_index=0;
            m_obuffer_index=0;
            if(FT_GetQueueStatus(m_handle, &bytesInQueue) == FT_OK)
            {
                m_isOpened = true;
                m_number = number;
                if( !initMPSSE_Controller(true) )
                {
                    close();
                }
            }
        }
    }
    emit signal_StateChanged(m_isOpened);
    log_message(LOG_FTDI_INFO, "Connected");
    if(m_autoconnector_enabled)  m_autoconnector_suspend =  false;
    return m_isOpened;
}

/****************************************************************************
 * @function name: FtdiSpiChannel::enumDevices()
 * @param:
 *              QStringList *pchannels_list
 * @description:  enumerates ftdi port
 * @return: ( qint32 ) returns number of items(devices)
 ****************************************************************************/
qint32  FtdiSpiChannel::enumDevices(QStringList *pchannels_list){
    FT_STATUS status;
    DWORD numDevs;
    qint32 i;
    FT_DEVICE_LIST_INFO_NODE *pdev_list;
    pchannels_list->clear();
    status = FT_CreateDeviceInfoList(&numDevs);
    if (status == FT_OK && numDevs >0)
    {
        pdev_list =
                (FT_DEVICE_LIST_INFO_NODE*)malloc(sizeof(FT_DEVICE_LIST_INFO_NODE) * numDevs);
        if(FT_GetDeviceInfoList (pdev_list,&numDevs)== FT_OK)
        {
            for(i=0; i<(qint32)numDevs; i++)
            {
                QString text;
                text = QString("N%1 ID:%2 SERIAL:%3")
                        .arg(i)
                        .arg(QString::number((quint32)pdev_list[i].ID,16))
                        .arg((const char*)pdev_list[i].SerialNumber);
                if(pdev_list[i].Flags &FT_FLAGS_OPENED)
                {
                    text += " - BUSY";
                }
                pchannels_list->append(text);
            }
        }
        free(pdev_list);
    }
    return (qint32)numDevs;
}


/****************************************************************************
 * @function name: FtdiSpiChannel::flush()
 * @param:
 *       void
 * @description: clears input output of port
 * @return: ( void )
 ****************************************************************************/
void FtdiSpiChannel::flush()
{
    char tmp[100];
    DWORD bytesInQueue;
    DWORD bytesRead;
    while (FT_GetQueueStatus(m_handle, &bytesInQueue) == FT_OK)
    {
        FT_Read(m_handle, &tmp, bytesInQueue, &bytesRead);
        if(bytesRead ==0 ) break;
    }
}

/****************************************************************************
 * @function name: FtdiSpiChannel::setTimeouts()
 * @param:
 *       qint32 latency -
 *       quint32 timeoutRead -
 *       quint32 timeoutWrite -
 * @description: sets exect parameters. Requires close-> open
 * @return: ( void )
 ****************************************************************************/
void FtdiSpiChannel::setTimeouts(qint32 latency,quint32 timeoutRead,quint32 timeoutWrite)
{
    configuration.latencyTimer =  latency;
    configuration.readTimeout = timeoutRead;
    configuration.writeTimeout = timeoutWrite;
}

/****************************************************************************
 * @function name: FtdiSpiChannel::initMPSSE_Controller()
 * @param:
 *   void
 * @description:  ftdi chip initialization. And presence check
 * @return: ( bool ) success = true
 ****************************************************************************/
bool FtdiSpiChannel::initMPSSE_Controller(bool check_channel){
    /*
   * 0x80 Set Data Bits Low Byte
   * 0x08 TCK TDI low, TMS high
   * 0x0B TCK, TDI, TMS output, TDO and GPIOL0->GPIOL3 input
   */
    const quint32 init_direction =  FT2232_INIT_GPIO_DIR;
    const quint32 init_pins = FT2232_INIT_GPIO_PIN;
    bool result = false;
    if( m_handle )
    {
        FT_ResetDevice(m_handle);
        FT_SetFlowControl(m_handle,FT_FLOW_RTS_CTS, 0x00, 0x00 );
        FT_SetLatencyTimer(m_handle,configuration.latencyTimer);
        FT_SetUSBParameters(m_handle, FTDI_MAX_IN_BUFFER, FTDI_MAX_OUT_BUFFER);
        //Set USB  request transfer size
        FT_SetChars(m_handle, false, 0,false ,0);
        //Disable event and error characters
        FT_SetTimeouts(m_handle, configuration.readTimeout, configuration.writeTimeout);
        //Sets the
        FT_SetBitMode(m_handle, 0x00, FTDI_RESET ); // reset controller
        FT_SetBitMode(m_handle, 0x00, FTDI_MPSSE_MODE); // enable MPSSE controller

        bool check_ok;
        if(check_channel) check_ok = checkMPSSE_Controller();
        if( check_ok)
        {
            /* configuring speed and pins*/
            //  sleep(20); // wait for all the USB stuff to complete
            m_GPIO_Pins = init_pins;
            m_GPIO_Direction = init_direction;
            // low pins
            addToBuffer(FT2232_CMD_SET_GPIOLOW); // set SK,DO,CS as out
            addToBuffer((quint8)m_GPIO_Pins); // all low
            addToBuffer((quint8)m_GPIO_Direction); // inputs on GPIO0-7
            //high pins
            addToBuffer(FT2232_CMD_SET_GPIOHIGH); // outputs on GPIO8-15
            addToBuffer((quint8)(m_GPIO_Pins >> 8));
            addToBuffer((quint8)(m_GPIO_Direction >> 8));
            //clock
            addToBuffer(FT2232_CMD_CLOCK_DIVISOR); // set clk divisor 6 Mhz
            quint32 period_value = (quint32)(12.0f/(configuration.clockPeriod*2.0f)-1.0f);
            addToBuffer((quint8)(period_value&0xff));
            addToBuffer((quint8)((period_value>>8)&0xff));
            // turn off loop back
            addToBuffer(FT2232_CMD_DIS_LOOPBACK);
            // send off the command
            result = sendToFtdi();
        }
    }
    return result;
}

/****************************************************************************
 * @function name: FtdiSpiChannel::checkMPSSE_Controller()
 *
 * @param:     void
 * @description: wiil try to send dummy bytes to controller.
 *              FTDI will return wrong bytes back.
 *              If controller returns the same byte connection is ok.
 * @return: ( bool ) success = true
 ****************************************************************************/
bool FtdiSpiChannel::checkMPSSE_Controller(void){
    //
    // This should satisfy outstanding commands.
    //
    // We will use $AA and $AB as commands which
    // are invalid so that the MPSSE block should echo these
    // back to us preceded with an $FA
    //
    bool result = false;
    DWORD bytesInQueue =0;
    DWORD bytes=0;

    if( isOpened() )
    {
        qint32 retry=100;
        while( bytesInQueue == 0 && retry--)
        {
            flush();
            addToBuffer(FT2232_CMD_UNKNOWN1); // bad command
            addToBuffer(FT2232_CMD_SEND_NOW); // bad command
            sendToFtdi();
            if(FT_GetQueueStatus(m_handle, &bytesInQueue) != FT_OK )
            {break;}
        }

        FT_Read(m_handle, m_ibuffer, bytesInQueue,&bytes);
        result = (m_ibuffer[0] == 0xFA && (m_ibuffer[1] == FT2232_CMD_UNKNOWN1));
    }
    return result;
}



/****************************************************************************
 * @function name: FtdiSpiChannel::sendToFtdi()
 * @param:  void
 * @description: sends data to FTDI chip
 * @return: ( bool ) success = true
 ****************************************************************************/
bool FtdiSpiChannel::sendToFtdi(void)
{
    bool result =false;
    DWORD bytesWritten=0;
    log_message(LOG_FTDI_TX, QString(QByteArray((const char*)m_obuffer,m_obuffer_index).toHex()));

    if(FT_Write(m_handle, m_obuffer, m_obuffer_index, &bytesWritten) == FT_OK)
    {
        if(m_obuffer_index == bytesWritten)
        {
            result = true;
        }
    }
    m_obuffer_index =0;
    emit signal_Active();
    return result;
}



/****************************************************************************
 * @function name: FtdiSpiChannel::readFromFtdi - ---
 * @param:
 *      qint32 max_bytes - size of mximum size of data to read
 * @description: reads data from FTDI
 * @return: ( qint32 ) number of bytes read
 ****************************************************************************/
qint32 FtdiSpiChannel::readFromFtdi(int max_bytes)
{
    qint32 result = 0;
    DWORD bytesInQueue =0;

    if( isOpened() )
    {
        QTime t;
        t.start();
        while((quint32)t.elapsed()< configuration.readTimeout/1000)
        {
            if(FT_GetQueueStatus(m_handle, &bytesInQueue) != FT_OK ) break;
            if(bytesInQueue==0) continue;
            DWORD bytesRead =0;
            if(FT_Read(m_handle, &m_ibuffer[m_ibuffer_index], bytesInQueue, &bytesRead) != FT_OK) break;
            if( bytesRead>0) t.start();
            result += (qint32)bytesRead;
            m_ibuffer_index +=  (qint32)bytesRead;
            if(max_bytes>0 && result>=max_bytes ) break;
        }
    }
    log_message(LOG_FTDI_RX,QString(QByteArray((const char*)m_ibuffer,m_ibuffer_index).toHex()));
    emit signal_Active();

    return result;
}

/****************************************************************************
 * @function name: FtdiSpiChannel::readSetupGPIO()
 * @param:
 *          qint32 gpio_number
 *         FtdiSpiChannel::Direction *pdir
 * @description: reads gpio_number direction INPUT/OUTPUT
 * @return: ( bool ) - true - function succeeded
 ****************************************************************************/
bool FtdiSpiChannel::readSetupGPIO(qint32 gpio_number, FtdiSpiChannel::Direction *pdir)
{
    if( ((quint32)1 << gpio_number) & m_GPIO_Direction)
    {
        *pdir = FtdiSpiChannel::INPUT;
    }
    else
    {
        *pdir = FtdiSpiChannel::OUTPUT;
    }
    return true ;
}

/****************************************************************************
 * @function name: FtdiSpiChannel::readSetupAll_GPIO()
 * @param:
 *          quint32 *pvalue - data pointer
 * @description: this returns GPIO direction
 * @return: ( bool ) - true = succeeded
 ****************************************************************************/
bool FtdiSpiChannel::readSetupAll_GPIO(quint32 *pvalue)
{    
    *pvalue = ~m_GPIO_Direction;
    return true;
}

/****************************************************************************
 * @function name: FtdiSpiChannel::setupGPIO()
 * @param:
 *          qint32 gpio_number
 *          FtdiSpiChannel::Direction dir
 *          bool send_immediately - if no need to send data immediately.
 *          Buffer data will be sent after this command will be called with true parameter.
 *          It is usefull if you need to obtain maximum speed.
 * @description:
 * @return: ( bool )
 ****************************************************************************/
bool FtdiSpiChannel::setupGPIO(qint32 gpio_number, FtdiSpiChannel::Direction dir, bool send_immediately)
{
    bool result;
    quint8 val_u8;
    quint8 dir_u8 ;
    quint32 gpio_direction = (dir == OUTPUT) << gpio_number;
    m_GPIO_Direction &= ~ (1<<gpio_number);
    m_GPIO_Direction |= gpio_direction;
    if(gpio_number <8)
    {
        //This will setup the direction of the first 8 lines
        val_u8 =  (quint8)m_GPIO_Pins;
        dir_u8 = (quint8) m_GPIO_Direction;
        addToBuffer(FT2232_CMD_SET_GPIOLOW);
    }
    else
    {
        //This will setup the direction of the high 8 lines
        val_u8 =  (quint8)(m_GPIO_Pins >> 8);
        dir_u8 = (quint8)( m_GPIO_Direction >> 8);
        addToBuffer(FT2232_CMD_SET_GPIOHIGH);
    }

    addToBuffer(val_u8);
    addToBuffer(dir_u8);
    if(send_immediately)
    {
        addToBuffer(FT2232_CMD_SEND_NOW);
        result = sendToFtdi();
    }
    return result ;
}

/****************************************************************************
 * @function name: FtdiSpiChannel::setupAll_GPIO()
 * @param:
 *          quint32 value - values for GPIO DIRection
 * @description: willl set whole port direction
 * @return: ( bool )  true - success
 ****************************************************************************/
bool FtdiSpiChannel::setupAll_GPIO(quint32 value)
{
    bool result;
    m_GPIO_Direction = ~value;
    addToBuffer(FT2232_CMD_SET_GPIOLOW);
    addToBuffer((quint8) m_GPIO_Pins);
    addToBuffer((quint8)m_GPIO_Direction);
    addToBuffer(FT2232_CMD_SET_GPIOHIGH);
    addToBuffer((quint8)( m_GPIO_Pins>>8) );
    addToBuffer((quint8)(m_GPIO_Direction >>8));
    addToBuffer(FT2232_CMD_SEND_NOW);
    result = sendToFtdi();
    return result;
}

/****************************************************************************
 * @function name: FtdiSpiChannel::writeAll_GPIO()
 * @param:
 *          quint32 value - values for GPIO
 * @description:  this function update GPIO port on FTDI chip immediately
 * @return: ( bool )  true - success
 ****************************************************************************/
bool FtdiSpiChannel::writeAll_GPIO( quint32 value)
{
    bool result;
    m_GPIO_Pins = value;
    addToBuffer(FT2232_CMD_SET_GPIOLOW);
    addToBuffer((quint8) m_GPIO_Pins);
    addToBuffer((quint8)m_GPIO_Direction);
    addToBuffer(FT2232_CMD_SET_GPIOHIGH);
    addToBuffer((quint8)( m_GPIO_Pins>>8) );
    addToBuffer((quint8)(m_GPIO_Direction >>8));
    addToBuffer(FT2232_CMD_SEND_NOW);
    result = sendToFtdi();
    return result;
}

/****************************************************************************
 * @function name: FtdiSpiChannel::writeGPIO()
 * @param:
 *    qint32 gpio_number - gpio pin number
 *    bool value - value
 *    bool send_immediately if no need to send data immediately.
 *          Buffer data will be sent after this command will be called
 *          with true parameter.It is usefull if you need to obtain
 *          maximum speed.
 * @description:
 * @return: ( bool ) success =true
 ****************************************************************************/
bool FtdiSpiChannel::writeGPIO(qint32 gpio_number, quint32 value, bool send_immediately)
{
    bool result;
    quint8 val_u8;
    quint8 dir_u8 ;
    quint32 gpio_value = (quint32)value << gpio_number;
    m_GPIO_Pins &= ~ (1<<gpio_number);
    m_GPIO_Pins |= gpio_value;

    if(gpio_number <8)
    {
        //This will setup the direction of the first 8 lines
        val_u8 =  (quint8)m_GPIO_Pins;
        dir_u8 = (quint8)m_GPIO_Direction;
        addToBuffer(FT2232_CMD_SET_GPIOLOW);
    }
    else
    {
        //This will setup the direction of the high 8 lines
        val_u8 =  (quint8)(m_GPIO_Pins >> 8);
        dir_u8 = (quint8)(m_GPIO_Direction >>8);
        addToBuffer(FT2232_CMD_SET_GPIOHIGH);
    }
    addToBuffer(val_u8);
    addToBuffer(dir_u8);
    ;
    if(send_immediately)
    {
        addToBuffer(FT2232_CMD_SEND_NOW);
        result = sendToFtdi();       
    }
    return result ;
}

/****************************************************************************
 * @function name: FtdiSpiChannel::pinSequnce()
 * @param:
 *    qint32 gpio_number - number of pin
 *    const QByteArray &sequence - sequency of 1 and 0
 * @description: this function will generate sequence on exact pin ~560nS
 *              switch time
 * @return: ( bool ) success =true
 ****************************************************************************/
bool FtdiSpiChannel::pinSequence(qint32 gpio_number, const QByteArray &sequence)
{
    bool result = false;
    quint8 value;
    quint8 val_u8;
    quint8 dir_u8 ;
    quint32 gpio_value;

    if(gpio_number <16 && gpio_number>=0 && sequence.size()>0 &&sequence.size()<=(65535/3) )
    {
        for(qint32 i=0;i<sequence.size();i++)
        {
            if(sequence[i] == '0')
            {
                value = 0;
            }
            else if(sequence[i] == '1')
            {
                value =1;
            }
            else continue;//ignore other symbols
            gpio_value = (quint32)value << gpio_number;
            m_GPIO_Pins &= ~ (1<<gpio_number);
            m_GPIO_Pins |= gpio_value;
            if(gpio_number <8)
            {
                //This will setup the direction of the first 8 lines
                val_u8 =  (quint8)m_GPIO_Pins;
                dir_u8 = (quint8)m_GPIO_Direction;
                addToBuffer(FT2232_CMD_SET_GPIOLOW);
            }
            else
            {
                //This will setup the direction of the high 8 lines
                val_u8 =  (quint8)(m_GPIO_Pins >> 8);
                dir_u8 = (quint8)(m_GPIO_Direction >>8);
                addToBuffer(FT2232_CMD_SET_GPIOHIGH);
            }
            addToBuffer(val_u8);
            addToBuffer(dir_u8);
        }
        addToBuffer(FT2232_CMD_SEND_NOW);
        result = sendToFtdi();
    }
    return result;
}

/*bool FtdiSpiChannel::pinSequence(qint32 gpio_number, const QByteArray &sequence)
{
    bool result = false;

    if(gpio_number <32 && gpio_number>=0 && sequence.size()>0 &&sequence.size()<=(65535/3) )
    {
        for(qint32 i=0;i<sequence.size();i++)
        {
            if(sequence[i] == '0')
            {
                writeGPIO(gpio_number, 0,false);
            }
            else if(sequence[i] == '1')
            {
                writeGPIO(gpio_number, 1,false);
            }
            else continue;//ignore other symbols
        }
        result = sendToFtdi();
        readFromFtdi();
    }
    return result;
}
*/
/****************************************************************************
 * @function name: FtdiSpiChannel::readGPIO()
 * @param:
 *      qint32 gpio_number - pin number
 *    bool *value - value returned here
 * @description: reads value of gpio port. value will be returned to *value
 * @return: ( bool )success =true
 ****************************************************************************/
bool FtdiSpiChannel::readGPIO(qint32 gpio_number, quint32 *value)
{
    bool result = false;
    quint32 gpio_mask;
    if(gpio_number <8)
    {
        // This will read the current state of
        // the first 8 pins and send back 1 byte
        gpio_mask = (quint32)1 << gpio_number;
        addToBuffer(FT2232_CMD_READ_GPIOLOW);
    }
    else
    {
        // This will read the current state of
        // the high 8 pins and send back 1 byte
        gpio_mask = (quint32)1 << (gpio_number - 8);
        addToBuffer(FT2232_CMD_READ_GPIOHIGH);
    }
    addToBuffer(FT2232_CMD_SEND_NOW);
    result = sendToFtdi();
    if( readFromFtdi(1) == 1)
    {
        if(gpio_mask & (quint32)m_ibuffer[0])
        {
            *value = true;
        }
        else
        {
            *value = false;
        }
    }
    m_ibuffer_index = 0;
    return result;
}

/****************************************************************************
 * @function name: FtdiSpiChannel::readAll_GPIO()
 * @param:
 *    qint32 gpio_number - pin number
 *    bool *value - value returned here
 * @description: reads all gpio state
 * @return: ( bool )success =true
 ****************************************************************************/
bool FtdiSpiChannel::readAll_GPIO(quint32 *value)
{
    bool result;
    addToBuffer(FT2232_CMD_READ_GPIOLOW);
    addToBuffer(FT2232_CMD_READ_GPIOHIGH);
    addToBuffer(FT2232_CMD_SEND_NOW);
    result = sendToFtdi();
    if(result && readFromFtdi(2) >= 2 )
    {
        *value = m_ibuffer[0] | (m_ibuffer[1]<<8);
        result = true;
    }
    m_ibuffer_index = 0;
    return result;
}


/****************************************************************************
 * @function WRITE name: FtdiSpiChannel::setFrequency()
 *           READ       FtdiSpiChannel::frequency()
 * @param:
 *     double freq in Mhz
 * @description: setup SPI frequency
 * @return: ( bool ) true success
 ****************************************************************************/
bool FtdiSpiChannel::setFrequency(double freq)
{
    if(this->isOpened())
    {
        addToBuffer(FT2232_CMD_CLOCK_DIVISOR); // set clk divisor 6 Mhz
        quint32 period_value = (quint32)(12.0f/(freq*2.0f)-1.0f);
        configuration.clockPeriod = period_value;
        addToBuffer((quint8)(period_value&0xff));
        addToBuffer((quint8)((period_value>>8)&0xff));
    }
    return sendToFtdi();
}


double FtdiSpiChannel::frequency()
{
    return (double)12.0f/((configuration.clockPeriod+1)*2);
}


/****************************************************************************
 * @function name: FtdiSpiChannel::setCS()
 * @param:
 *     bool on
 * @description: CS 1- on 0-off according to setting
 *              (if negative(default) inverted)
 * @return: ( bool )
 ****************************************************************************/
bool  FtdiSpiChannel::setCS(bool on, bool send_immediately)
{
    bool result = false;
    if( isOpened() )
    {
        if(configuration.negativeCS) on= !on;
        result = writeGPIO(FT2232_CS, on,send_immediately);
    }
    return result;
}


/****************************************************************************
 * @function name: FtdiSpiChannel::checkDO()
 * @param:   void
 * @description: check if DO pin is on
 * @return: ( bool )  success = true
 ****************************************************************************/
bool FtdiSpiChannel::checkDO(void)
{
    quint32 result =  false;
    if(isOpened()){
        readGPIO(FT2232_DO, &result);
    }
    return result;
}


/****************************************************************************
 * @function name: FtdiSpiChannel::readData()
 * @param:
 *   QByteArray &data - data to read  (in binary format)
 *   qint32 sizeToRead  - size of data to expect
 * @description: reads data from SPI port
 * @return: ( qint32 ) data size read
 ****************************************************************************/
qint32 FtdiSpiChannel::readData(QByteArray &data, qint32 sizeToRead, bool send_immediately)
{
    qint32 result = 0;
    qint32 bytesCount;
    qint32 bitsCount ;
    quint8 cmdByte;
    quint8 cmdBit;
    /* CS on */
    if(isOpened())
    {
        switch(configuration.mode)
        {
        case (FT2232_MSB_RISING_EDGE):
            cmdByte = FT2232_CMD_MSB_RE_CLOCK_BYTE_IN;
            cmdBit = FT2232_CMD_MSB_RE_CLOCK_BIT_IN;
            break;
        case (FT2232_MSB_FALLING_EDGE):
            cmdByte = FT2232_CMD_MSB_FE_CLOCK_BYTE_IN;
            cmdBit = FT2232_CMD_MSB_FE_CLOCK_BIT_IN;
            break;
        case (FT2232_LSB_RISING_EDGE):
            cmdByte = FT2232_CMD_LSB_RE_CLOCK_BYTE_IN;
            cmdBit = FT2232_CMD_LSB_RE_CLOCK_BIT_IN;
            break;
        case (FT2232_LSB_FALLING_EDGE):
            cmdByte = FT2232_CMD_LSB_FE_CLOCK_BYTE_IN;
            cmdBit = FT2232_CMD_LSB_FE_CLOCK_BIT_IN;
            break;
        default:
            break;
        }
        if(configuration.lengthInBits)
        {
            bitsCount = (sizeToRead % 8);
            bytesCount = sizeToRead / 8 ;
        }
        else
        {
            bytesCount = sizeToRead;
            bitsCount = 0;
        }
        setCS(true,false);
        // adjust count value
        addToBuffer(cmdByte);
        addToBuffer(bytesCount - 1);
        addToBuffer(0x00);
        if (bitsCount > 0)
        {
            addToBuffer(cmdBit);
            addToBuffer(bitsCount-1);
        }
        if(send_immediately)
        {
            addToBuffer(FT2232_CMD_SEND_NOW);
            sendToFtdi();
            /* CS off */
            result = readFromFtdi();
            data = QByteArray((const char*)m_ibuffer,result);
            setCS(false);
        }
        else result = true;
    }
    return result;
}




/****************************************************************************
 * @function name: FtdiSpiChannel::sendData()
 * @param:
 *    const QByteArray &data - data to send and result returned here
 *                          (in binary format)
 *    [qint32 dataToSend=0] - size of data to send
 *                             if dataToSend =0 bytearray will be sent
 *                                                              all as bytes
 * @description: SPI module sends data by SPI port
 * @return: ( qint32 ) number of bytes
 ****************************************************************************/
qint32 FtdiSpiChannel::sendData(const QByteArray &data, qint32 dataToSend, bool send_immediately)
{
    qint32 result = 0;
    const char *pbuf = data.constData();
    qint32 bytesCount;
    qint32 bitsCount;
    qint32 j=0;
    quint8 cmdByte;
    quint8 cmdBit;
    /* CS on */
    if( isOpened() )
    {
        switch(configuration.mode){
        case (FT2232_MSB_RISING_EDGE):
            cmdByte = FT2232_CMD_MSB_RE_CLOCK_BYTE_OUT;
            cmdBit = FT2232_CMD_MSB_RE_CLOCK_BIT_OUT;
            break;
        case (FT2232_MSB_FALLING_EDGE):
            cmdByte = FT2232_CMD_MSB_FE_CLOCK_BYTE_OUT;
            cmdBit = FT2232_CMD_MSB_FE_CLOCK_BIT_OUT;
            break;
        case (FT2232_LSB_RISING_EDGE):
            cmdByte = FT2232_CMD_LSB_RE_CLOCK_BYTE_OUT;
            cmdBit = FT2232_CMD_LSB_RE_CLOCK_BIT_OUT;
            break;
        case (FT2232_LSB_FALLING_EDGE):
            cmdByte = FT2232_CMD_LSB_FE_CLOCK_BYTE_OUT;
            cmdBit = FT2232_CMD_LSB_FE_CLOCK_BIT_OUT;
            break;
        default:
            break;
        }

        if(configuration.lengthInBits)
        {
            bitsCount = (dataToSend % 8);
            bytesCount = dataToSend/8 ;
        }
        else
        {
            bitsCount=0;
            bytesCount = dataToSend;
        }
        if(bytesCount + (bitsCount != 0) <= data.size())
        {

            setCS(true,false);
            if( bytesCount>0)
            {
                addToBuffer(cmdByte);
                addToBuffer(bytesCount - 1 ); //Length Lo
                addToBuffer(0x00); // length Hi
                /* byytes*/
                for(j=0; bytesCount;bytesCount-- )
                {
                    addToBuffer(pbuf[j++]);
                }
            }
            /* bits*/
            if(bitsCount>0)
            {
                addToBuffer(cmdBit);
                addToBuffer(bitsCount - 1 );
                addToBuffer(pbuf[j]);
            }
            writeGPIO(FT2232_DO,configuration.normal_DO_state,false);
            if(send_immediately)
            {
                addToBuffer(FT2232_CMD_SEND_NOW);
                result = sendToFtdi();
                /* CS off*/
                setCS(false);
            }
            else result=true;
        }
    }
    return result;
}

/****************************************************************************
 * @function name: FtdiSpiChannel::transferData()
 *
 * @param:
 *
 *      QByteArray &data - data to transfer and result returned here
 *                          (in binary format)
 *      [qint32 sizeToTransfer] - size to transfer bits/bytes
 *                        (option in Config see setConfiguration() function)
 * @description: Transfers data to from SPI port
 * @return: ( bool ) - success =true
 ****************************************************************************/
bool FtdiSpiChannel::transferData(QByteArray &data, qint32 sizeToTransfer)
{
    bool result= false;
    qint32 bytesCount;
    qint32 bitsCount;
    const char *pbuf = data.constData();
    qint32 j=0;
    quint8 cmdByte;
    quint8 cmdBit;
    if(isOpened())
    {
        setCS(true,false);
        switch(configuration.mode)
        {
        case (FT2232_MSB_RISING_EDGE):
        case (FT2232_MSB_FALLING_EDGE):
            cmdByte = FT2232_CMD_MSB_FE_RE_CLOCK_BYTE_IO;
            cmdBit = FT2232_CMD_MSB_FE_RE_CLOCK_BIT_IO;
            break;
        case (FT2232_LSB_RISING_EDGE):
        case (FT2232_LSB_FALLING_EDGE):
            cmdByte = FT2232_CMD_LSB_FE_RE_CLOCK_BYTE_IO;
            cmdBit = FT2232_CMD_LSB_FE_RE_CLOCK_BIT_IO;
            break;
        default:
            break;
        }
        if(configuration.lengthInBits)
        {
            bitsCount = (sizeToTransfer % 8);
            bytesCount = sizeToTransfer / 8 ;
        }
        else
        {
            bitsCount =0;
            bytesCount = sizeToTransfer;
        }
        if(bytesCount + (bitsCount != 0) <=data.size())
        {
            if( bytesCount>0)
            {
                addToBuffer(cmdByte);
                addToBuffer(bytesCount - 1 );
                addToBuffer(0x00);
                // now add the data bytes to go out
                for(j=0; bytesCount;bytesCount-- )
                {
                    addToBuffer(pbuf[j++]);
                }
            }
            if(bitsCount>0)
            {
                addToBuffer(cmdBit);
                addToBuffer(bitsCount - 1 );
                // now add the data bits to go out
                addToBuffer(pbuf[j]);
            }
            addToBuffer(FT2232_CMD_SEND_NOW);
            writeGPIO(FT2232_DO,configuration.normal_DO_state,false);
            /* CS off*/
           setCS(false,false);

            result = sendToFtdi();
            result &= (readFromFtdi() >0);
            data = QByteArray((const char*)m_ibuffer,m_ibuffer_index);
            m_ibuffer_index =0;

        }

    }
    return result;
}

/****************************************************************************
 * @function name: FtdiSpiChannel::writeReadData()
 * @param:
 *      const QByteArray &dataToWrite
 *      qint32 sizeToWrite
 *      QByteArray *pdataRead
 *      qint32 sizeToRead
 * @description: Sends number of bits and reads number of bits
 * @return: ( void )
 ****************************************************************************/
bool FtdiSpiChannel::writeReadData(const QByteArray &dataToWrite, qint32 sizeToWrite
                                   , QByteArray *pdataRead, qint32 sizeToRead)
{
    bool result = false;
    setCS(true);
    sendData(dataToWrite,sizeToWrite,false);
    result = readData(*pdataRead,sizeToRead,true);
    setCS(false);
    return result;
}

/****************************************************************************
 * @function name: FtdiSpiChannel::run()
 * @param:
 * @description: controls if port is connected .
 *              In the case the port is disconnected but should be connected
 *              will issue reconnect
 * @return: ( void )
 ****************************************************************************/

void FtdiSpiChannel::run()
{
    bool need_connection;
    while( !m_autoconnector_finish )
    {
        if( !m_autoconnector_suspend )
        {
            need_connection = false;

            if( isOpened() )
            {
                bool responds = checkPort();
                emit signal_StateChanged( responds );
                need_connection |=  !responds && isOpened() ;
                while(need_connection)
                {
                    quint32 bak_dir,bak_pins; //pins state
                    qint32 bak_number;//ftdi channel number
                    if(isOpened())
                    {
                        bak_number = number();
                        bak_dir = directions();
                        bak_pins = pins();
                        close();
                    }
                    //try open
                    if(open(bak_number) == true)
                    {
                        setupAll_GPIO(bak_dir);
                        writeAll_GPIO(bak_pins);
                        need_connection    = false;
                    }
                }
            }
        }
        msleep(200);
    }
    exec();
}

/****************************************************************************
 * @function name: FtdiSpiChannel::log_message()
 * @param:
 *          LogMessage message - message type
 *          const QString message_text - message text
 * @description: sends log message to log window
 * @return: ( void )
 ****************************************************************************/
void FtdiSpiChannel::log_message( LogMessage message, const QString message_text)
{
    static QTime time = QTime::currentTime();
    const char LOG_TIME_FORMAT[]="hh:mm:ss.zzz";
    int color;
    int time_diff=time.msecsTo(QTime::currentTime());
    QString text;
    QString unit = "ms";

    if(logMode())
    {
        if(time_diff>1000) {time_diff /= 1000;unit="s";}
        if(message == LOG_FTDI_TX)
        {
            color =LOG_COLOR(Qt::blue);

            text = QString("TX[%1](%2):%3")
                    .arg(this->m_number)
                    .arg(QTime::currentTime().toString(LOG_TIME_FORMAT))
                    .arg(message_text);
        }
        else if(message == LOG_FTDI_RX)
        {
            color =LOG_COLOR(Qt::green);
            text = QString("RX[%1](+%2%3):%4")
                    .arg(this->m_number)
                    .arg(time_diff)
                    .arg(unit)
                    .arg(message_text);
        }
        else if(message == LOG_FTDI_INFO)
        {
            color = LOG_COLOR(Qt::darkGray);
            text = QString("INF[%1](%2):%3")
                    .arg(this->m_number)
                    .arg(QTime::currentTime().toString(LOG_TIME_FORMAT))
                    .arg(message_text);
        }
        else if(message == LOG_FTDI_ERROR)
        {
            color = LOG_COLOR(Qt::red);
            text = QString("RX_ERR[%1](+%2%3):%4")
                    .arg(this->m_number)
                    .arg(time_diff)
                    .arg(unit)
                    .arg(message_text);
        }


        gp_Logs->slot_addLog(color,text);
        time = QTime::currentTime();
    }
}


bool FtdiSpiChannel::checkPort()
{
    DWORD tmp;
    return FT_GetQueueStatus(m_handle, &tmp) == FT_OK;
}

void FtdiSpiChannel::enableAutoConnector(bool on)
{
    m_autoconnector_enabled =  on;
    if(m_autoconnector_enabled)  m_autoconnector_suspend =  false;
}

bool FtdiSpiChannel::isAutoConnectorEnabled() const
{
    return m_autoconnector_enabled;
}

quint32 FtdiSpiChannel::directions() const
{
    return m_GPIO_Direction;
}

quint32 FtdiSpiChannel::pins() const
{
    return m_GPIO_Pins;
}

qint32 FtdiSpiChannel::number() const
{
    return m_number;
}

void FtdiSpiChannel::setLogMode(bool on)
{
    m_log_mode = on;
}

bool FtdiSpiChannel::logMode() const
{
    return m_log_mode;
}

void FtdiSpiChannel::setCrcType(FtdiSpiChannel::LinCrcType crc_type)
{
    m_crcType = crc_type;
}




// LIN

/****************************************************************************
 * @function name: FtdiSpiChannel::writeLin()
 * @param:
 *          qint32 break_len
 *          qint32 sync
 *          qint32 pid
 *          const QByteArray &data_out -data to write to port
 *          QByteArray *pdata_in - where to put result
 * @description: this function scales byte array by bit
 *              Example: input bytes hex : f0 01  , factor = 2
 *                      output bytes: ff 00 00 03
 *                      (in this example function will double each bit and return
 *                          it as byte array. SizeOut = SizeIn*factor )
 * @return: ( bool ) success = true
 ****************************************************************************/
bool FtdiSpiChannel::writeLin(qint32 break_len, qint32 sync, qint32 pid, const QByteArray &data_out)
{
#define LIN_H QByteArray("\xff",1)
#define LIN_L QByteArray("\x00",1)
#define CONV(ARR,BYTE) for(int __i=0;__i<8;__i++) {if(BYTE&(1<<__i)) ARR.append(LIN_H);else ARR.append(LIN_L);}

    bool result = false;
    QByteArray raw;
    quint32 checksum;


    while(break_len--) raw.append(LIN_L);
    raw.append(LIN_H);

    raw.append(LIN_L);
    CONV(raw,sync);
    raw.append(LIN_H);

    raw.append(LIN_L);
    CONV(raw,pid);
    raw.append(LIN_H);

    checksum = calcLinChecksum(crcType(),QByteArray(data_out).insert(0,pid));

    for(int i=0;i<data_out.size();i++)
    {
        raw.append(LIN_L);
        CONV(raw,(quint8)data_out.at(i));
        raw.append(LIN_H);
    }
    raw.append(LIN_L);
    CONV(raw,checksum);
    raw.append(LIN_H);
    setFrequency(frequency()*8);
    result = sendData(raw, raw.size()*8);
    setFrequency(frequency()/8);

    return result;

}

/****************************************************************************
 * @function name: FtdiSpiChannel::readLin()
 * @param:
 *          qint32 break_len
 *          qint32 sync
 *          qint32 pid
 *          QByteArray *pdata_in
 *          qint bytes_to_read
 * @description:
 * @return: ( bool ) success = true
 ****************************************************************************/

bool FtdiSpiChannel::readLin(qint32 break_len, qint32 sync, qint32 pid
                             , QByteArray *pdata_in, quint32 *pdata_crc, qint32 bytes_to_read)
{
#define LIN_H QByteArray("\xff",1)
#define LIN_L QByteArray("\x00",1)
#define CONV(ARR,BYTE) for(int __i=0;__i<8;__i++) {if(BYTE&(1<<__i)) ARR.append(LIN_H);else ARR.append(LIN_L);}

    setFrequency(frequency()*8);
    bool result = false;
    QByteArray raw;
    int header_len;
    quint32 checksum;

    while(break_len--) raw.append(LIN_L);
    raw.append(LIN_H);

    raw.append(LIN_L);
    CONV(raw,sync);
    raw.append(LIN_H);

    raw.append(LIN_L);
    CONV(raw,pid);
    raw.append(LIN_H);

    header_len = raw.size();


    for(int i=0;i<bytes_to_read;i++)
    {
        raw.append(LIN_H);
        CONV(raw,0xff);
        raw.append(LIN_H);
    }
    raw.append(LIN_H); //start
    CONV(raw,0xff);//crc
    raw.append(LIN_H);     //stop
    raw.append(LIN_H);  //dummy
    raw.append(LIN_H);  //dummy
    raw.append(LIN_H);  //dummy

    //transfer send and read
    transferData(raw, raw.size()*8);
    //chop header
    raw.remove(0, header_len);

    enum {LIN_UNDEF,LIN_START_BYTE,LIN_DATA_BYTE,LIN_STOP_BYTE};
    int step = LIN_UNDEF;

    // processing bits
    quint8 data_byte=0;
    int ctr=0;
    bool stop=false;
    for(int i=4;i<raw.size()*8;i++)
    {
        bool bit_orig = ((quint8)raw.at(i/8)&(1<<(7-i%8)))!=0;
        switch(step)
        {
        // lin undefined state
        case LIN_UNDEF:
            if (ctr>0 && bit_orig==0)
            {step = LIN_START_BYTE;ctr=0;}
            break;
            //lin start flag
        case LIN_START_BYTE:
            {
                data_byte=0;
                if(ctr<5 && bit_orig == 1 )
                {
                    step = LIN_UNDEF;
                    stop = 1;
                }
                else  if(ctr>8 || (bit_orig == 1))
                {
                    step = LIN_DATA_BYTE;
                    ctr=0;
                }
            }
            break;
            //lin data byte
        case LIN_DATA_BYTE:
            {
                static int kind=0;
                bool bit_done = (ctr%8 ==0 && ctr>0);
                if(bit_done)
                {
                    bool bit = kind>3;
                    data_byte |= (bit<<(ctr/8-1));
                    kind=0;
                }
                else kind += bit_orig;
                if(ctr >= 8*8)
                {
                    pdata_in->append(data_byte);
                    if(pdata_in->size() == bytes_to_read+1) {
                        result=true;
                        stop = 1;
                    }
                    step = LIN_STOP_BYTE;
                    ctr=0;kind=0;
                }
            }
            break;
            // stop flag
            case LIN_STOP_BYTE:
            {
                if(ctr>8 || bit_orig==0) {step = LIN_START_BYTE;ctr=0;}
            }
            break;
        }
        if(step== LIN_UNDEF && stop) break;
        ctr++;
    }
    if( result )
    {        
        int i = pdata_in->size()-1;
        if(i > 0 )
            *pdata_crc = (quint8)pdata_in->at(i);
        pdata_in->chop(1);
        checksum = calcLinChecksum(crcType(), QByteArray(*pdata_in).insert(0,pid));     
        result = (checksum == *pdata_crc);
    }
    setFrequency(frequency()/8);
    return result;
}

/****************************************************************************
 * @function name: FtdiSpiChannel::calcLinChecksum()
 * @param:
 *          const QByteArray &data - data woth PID. According to crc type
 *                                      function uses PID or not
 * @description: calculates checksum. function gets data including pid (first byte)
 * @return: ( quint32 ) checksum
 ****************************************************************************/
quint32 FtdiSpiChannel::calcLinChecksum(LinCrcType crc_type, const QByteArray &data)
{
    quint32 checksum=0;
    int i=0;
    if(crc_type == LIN_CRC_1_3)
        i=1;
    for(; i<data.size(); i++)
    {
        checksum += (quint32)((quint8)data.at(i)) & 0x000000ff;
        if(checksum >= 0x100)
        {
            checksum &= 0x000000ff;
            checksum +=1;
        }

    }
    return (checksum ^ 0x000000ff);
}


/****************************************************************************
 * @function name: FtdiSpiChannel::writeLinBreak()
 * @param:
 *          qint32 break_len
 * @description:
 * @return: ( bool ) checksum
 ****************************************************************************/
bool FtdiSpiChannel::writeLinBreak(qint32 break_len)
{
    QByteArray tmp;
    tmp.resize(break_len/8+1);
    tmp.fill('\x0');
    return transferData(tmp, break_len);
}


FtdiSpiChannel::LinCrcType FtdiSpiChannel::crcType() const
{
    return m_crcType;
}

quint8 FtdiSpiChannel::makePid(quint8 id)
{
#define CHECKBIT(B,N) ((B &(1<<N))!=0)
    bool p0 = ((CHECKBIT(id,0)+CHECKBIT(id,1)+CHECKBIT(id,2)+CHECKBIT(id,4))&1)!=0;
    bool p1 = ((CHECKBIT(id,1)+CHECKBIT(id,3)+CHECKBIT(id,4)+CHECKBIT(id,5))&1)!=0;
#undef CHECKBIT
    return (id &0x3F) | (!p1<<7) |(p0<<6);
}
