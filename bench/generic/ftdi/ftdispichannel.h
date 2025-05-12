/**
 * @file:ftdispichannel.h   - 
 * @description: 
 * @project: BENCH OnSemiconductor
 * @date: 2014\11\27 13-48-31
 * 
 */


#ifndef FTDISPICHANNEL_H
#define FTDISPICHANNEL_H

#include  <QObject>
#include <QThread>
#include <QStringList>
#include "Windows.h"
#include "ftd2xx.h"
#include "ftdispichannel.h"
#include "register.h"

/* PARAMETERS */
#define FTDI_MAX_IN_BUFFER        (65535) /* 1..65535*/
#define FTDI_MAX_OUT_BUFFER       (65535) /* 1..65535*/
#define FTDI_LATENCY_TIMER        (50)  /* 1..255*/
#define FTDI_READ_TIMEOUT         (50000)
#define FTDI_WRITE_TIMEOUT        (50000)
#define FTDI_MAX_CHANNELS           (2)
#define FTDI_SPI_FREQUENCY        (5.00f)


/* MPSSE commands */
#define FT2232_CMD_RESET          (0x00)
#define FT2232_CMD_SET_GPIOLOW    (0x80)
#define FT2232_CMD_SET_GPIOHIGH   (0x82)
#define FT2232_CMD_READ_GPIOLOW   (0x81)
#define FT2232_CMD_READ_GPIOHIGH  (0x83)
#define FT2232_CMD_CON_LOOPBACK   (0x84)
#define FT2232_CMD_DIS_LOOPBACK   (0x85)
#define FT2232_CMD_CLOCK_DIVISOR  (0x86)
#define FT2232_CMD_SEND_NOW       (0x87)
/* out MSB */
#define FT2232_CMD_MSB_RE_CLOCK_BYTE_OUT    (0x10)
#define FT2232_CMD_MSB_FE_CLOCK_BYTE_OUT    (0x11)
#define FT2232_CMD_MSB_RE_CLOCK_BIT_OUT     (0x12)
#define FT2232_CMD_MSB_FE_CLOCK_BIT_OUT     (0x13)
/* in MSB */
#define FT2232_CMD_MSB_RE_CLOCK_BYTE_IN     (0x20)
#define FT2232_CMD_MSB_RE_CLOCK_BIT_IN      (0x22)
#define FT2232_CMD_MSB_FE_CLOCK_BYTE_IN     (0x24)
#define FT2232_CMD_MSB_FE_CLOCK_BIT_IN      (0x26)
/*out LSB*/
#define FT2232_CMD_LSB_RE_CLOCK_BYTE_OUT    (0x18)
#define FT2232_CMD_LSB_FE_CLOCK_BYTE_OUT    (0x19)
#define FT2232_CMD_LSB_RE_CLOCK_BIT_OUT    (0x1A)
#define FT2232_CMD_LSB_FE_CLOCK_BIT_OUT    (0x1B)
/* in LSB */
#define FT2232_CMD_LSB_RE_CLOCK_BYTE_IN    (0x28)
#define FT2232_CMD_LSB_FE_CLOCK_BYTE_IN    (0x2C)
#define FT2232_CMD_LSB_RE_CLOCK_BIT_IN    (0x2A)
#define FT2232_CMD_LSB_FE_CLOCK_BIT_IN    (0x2E)
/* in out MSB */
#define FT2232_CMD_MSB_FE_RE_CLOCK_BYTE_IO  (0x31)
#define FT2232_CMD_MSB_RE_FE_CLOCK_BYTE_IO  (0x34)//not used
#define FT2232_CMD_MSB_FE_RE_CLOCK_BIT_IO   (0x33)
#define FT2232_CMD_MSB_RE_FE_CLOCK_BIT_IO   (0x36)//not used
/* in out LSB */
#define FT2232_CMD_LSB_FE_RE_CLOCK_BYTE_IO  (0x3C)
#define FT2232_CMD_LSB_RE_FE_CLOCK_BYTE_IO  (0x39)//not used
#define FT2232_CMD_LSB_FE_RE_CLOCK_BIT_IO   (0x3E)
#define FT2232_CMD_LSB_RE_FE_CLOCK_BIT_IO   (0x3B)//not used



#define FT2232_CMD_UNKNOWN1 (0xAA)
#define FT2232_CMD_UNKNOWN2 (0xAB)

/* FTDI chip MODES*/
#define FTDI_RESET          (0x00)
#define FTDI_MPSSE_MODE     (0x02)

/* GPIO pins*/
#define FT2232_SK                 (0)
#define FT2232_DO                 (1)
#define FT2232_DI                 (2)
#define FT2232_CS                 (3)
#define FT2232_GPIO0              (4)
#define FT2232_GPIO1              (5)
#define FT2232_GPIO2              (6)
#define FT2232_GPIO3              (7)
#define FT2232_GPIO4              (8)
#define FT2232_GPIO5              (9)
#define FT2232_GPIO6              (10)
#define FT2232_GPIO7              (11)
#define FT2232_GPIO8              (12)
#define FT2232_GPIO9              (13)
#define FT2232_GPIO10              (14)
#define FT2232_GPIO11              (15)

/* GPIO init states*/
#define FT2232_INIT_GPIO_DIR       (1<<FT2232_SK)|(1<<FT2232_DO)\
                                    |(1<<FT2232_CS)
#define FT2232_INIT_GPIO_PIN        (1<<FT2232_CS)

/* unknown command set 0x25 and 0x27
 * used in Pascal example*/

class FtdiSpiChannel : public QThread
{
    Q_OBJECT

signals:
    /* true - is connected false-disconnected */
    void signal_StateChanged(bool);
    void signal_Active();

public:
  FtdiSpiChannel();
  ~FtdiSpiChannel();
  /*directions */
  typedef enum{
    OUTPUT,/* =0*/
    INPUT/* *=1 */
  }Direction;
    typedef enum {
      LIN_CRC_1_3,
      LIN_CRC_2x
      } LinCrcType;
  /*clock modes */
  typedef enum{
    FT2232_MSB_RISING_EDGE,
    FT2232_MSB_FALLING_EDGE,
    FT2232_LSB_RISING_EDGE,
    FT2232_LSB_FALLING_EDGE
  }BitModes;
  /*configuration*/
  typedef struct
  {
    quint32 mode;
    quint32 readTimeout;
    quint32 writeTimeout;
    quint32 latencyTimer;
    bool lengthInBits;
    bool negativeCS;
    double clockPeriod;
    bool normal_DO_state;    
    qint32 communication_clock_scale;
  }Config;
  /* true if is opened*/
  bool isOpened() const;
  /* try to open ftdi */
  bool open(qint32 number);
  // close ftdi port
  void close();
  /* send data to ftdi SPI */
  qint32 sendData(const QByteArray &data, qint32 sizeToSend=0, bool send_immediately=true);
  /* read from ftdi SPI*/
  qint32 readData(QByteArray &data, qint32 sizeToRead, bool send_immediately=true);
  /* bytes available to read from ftdi */
  qint32 bytesAvailable();
  /* enumerate ftdi devices*/
  static qint32 enumDevices(QStringList *pchannels_list);
  /* flush data FTDI*/
  void flush(void);
  /* set timeouts*/
  void setTimeouts(qint32 latency, quint32 timeoutRead, quint32 timeoutWrite);
  /* sets configuration to system*/
  void setConfiguration(Config *pconfig);
  /* gets config to system */
  void getConfiguration(Config *pconfig);
  /* transfer data*/
  bool transferData(QByteArray &data, qint32 sizeToTransfer);
  /* */
  bool writeReadData(const QByteArray &dataToWrite, qint32 sizeToWrite, QByteArray *pdataRead, qint32 sizeToRead);
  /* setup GPIO pin */
  bool setupGPIO(qint32 gpio_number, Direction dir, bool send_immediately=true);
  /* write GPIO pin number with value 1 or 2*/
  bool writeGPIO(qint32 gpio_number, quint32 value, bool send_immediately=true);
  /* write all gpio value (there are 32 pins)*/
  bool writeAll_GPIO(quint32 value);
  /* write all gpio value (there are 32 pins, 1- input 0-output) */
  bool setupAll_GPIO(quint32 value);
  /* reads gpio number */
  bool readGPIO(qint32 gpio_number, quint32 *value);
  /* reads setup of all gpio*/
  bool readSetupAll_GPIO(quint32 *pvalue);
  /* read setup of gpio*/
  bool readSetupGPIO(qint32 gpio_number, FtdiSpiChannel::Direction *pdir);
  /* read all gpios */
  bool readAll_GPIO(quint32 *value);
  /* set CHIP SELECT */
  bool setCS(bool on, bool send_immediately=true);
  /* check DO pin */
  bool checkDO(void);
  /* set SPI frequency */
  bool setFrequency(double freq);
  /**/
  double frequency();
  /* write pin sequence */
  bool pinSequence(qint32 gpio_number, const QByteArray& sequence);
  /* check port if is alive/active */
  bool checkPort();
  /* enables autoreconnect function */
  void enableAutoConnector(bool on);
  /* return state of autoconnector*/
  bool isAutoConnectorEnabled() const;
  /* returns direction */
  quint32 directions() const;
  /* current gpio pins states*/
  quint32 pins() const;
  /* current ftdi port number*/
  qint32 number() const;
  /* sets log mode to on*/
  void setLogMode(bool on);
  /* log mode state*/
  bool logMode() const;    

  //LIN  
  /* */
  void setCrcType(LinCrcType crc_type);
  /* */
  LinCrcType crcType() const ;
  /* writes to LIN via physical SPI*/
  bool writeLin(qint32 break_len,qint32 sync, qint32 pid
                , const QByteArray &data_out);
  /* reads from LIN via physical SPI*/
  bool readLin(qint32 break_len, qint32 sync
               , qint32 pid, QByteArray *pdata_in, quint32 *pdata_crc, qint32 bytes_to_read);
  /* calculates checksum*/
  static quint32 calcLinChecksum(LinCrcType crc_type, const QByteArray &data);
  /* writes LIN break*/
  bool writeLinBreak(qint32 break_len);

  static quint8 makePid(quint8 id);

private:
  /* log mode types*/
  typedef enum {LOG_FTDI_RX, LOG_FTDI_TX, LOG_FTDI_INFO, LOG_FTDI_ERROR} LogMessage;
  /*generates log message to Log applet*/
  void log_message(LogMessage message , const QString message_text);
  /* port is opened*/
  bool m_isOpened;
  /* module is in log mode*/
  bool m_log_mode;
  /* ftdi  number*/
  qint32 m_number;
  /* configuration structure*/
  Config configuration;
  /* port handle*/
  FT_HANDLE m_handle;
  /* ==1(TRUE) - given length to any function will be interpreted as length in bits*/
  bool m_length_in_bits;// 
  /* input buffer*/
  quint8 m_ibuffer[FTDI_MAX_IN_BUFFER];
  /* output buffer*/
  quint8 m_obuffer[FTDI_MAX_OUT_BUFFER];
  /* obuffer last byte index. */
  quint32 m_obuffer_index;
  /* index of input buffer last byte*/
  quint32 m_ibuffer_index;
  /* GPIO direction state*/
  quint32 m_GPIO_Direction;
  /* GPIO pins state*/
  quint32 m_GPIO_Pins;
  /* autconnector to finish thread*/
  bool m_autoconnector_finish ;
  /* autoconnector pause*/
  bool m_autoconnector_suspend;
  /* autoconnector thread is enabled*/
  bool m_autoconnector_enabled;
  /* reads byte from FTDI hardware */
  qint32 readFromFtdi(int max_bytes=-1);
  /* send bytes to Ftdi hardware*/
  bool sendToFtdi(void);
  /*initialize MPSSE functionality of FTDI chip*/
  bool initMPSSE_Controller(bool check_channel);
  /*reset buffer input output*/
  void resetBuffer(void);
  /* adds bytes to buffer */
  void addToBuffer(quint8 byte);
  /* this function chesks if MPSSE present and responding*/
  bool checkMPSSE_Controller(void);



  // LIN
  qint32 m_linRX_pin;
  qint32 m_linTX_pin;
  qint32 m_linSpeed;
  LinCrcType m_crcType;

protected:
  /*thread run function */
  void run();

};





#endif // FTDISPICHANNEL_H

