/**
 * @file:mcuprotocol.h   -
 * @description:
 * @project: BENCH OnSemiconductor
 * @date: 2014\11\27 13-50-31
 *  This class intended for communication with MCU board using simple text protocol.
 *  For detailed information about protocol see: protocol_mcu_bench.docx
 */


#ifndef MCUPROTOCOL_H
#define MCUPROTOCOL_H
#include <QThread>
#include <QList>

#include <QVariant>
#include "serialportchannel.h"
#include <QBuffer>

#include <variable.h>
#include <QDateTime>
#include <QTimer>
#include "register.h"
#include "applet.h"

#define DEFAULT_TIMEOUT (500)
#define ADC_RESOLUTION (2048)
/* this class gets data from text in text format paramater = value
 * separated with separarotr CR
 * Protocol gets registers id to table and controls whether data are sent correctly
 * and all answerrs where received from channel.
 * prootocol communicates with channnel in thread .
 * also it has timeout for unreceived registers
 */
class ControlBoard: public QThread
{
  Q_OBJECT
public:  
  ControlBoard();
  ~ControlBoard();
  enum {INPUT = 1, OUTPUT   =  0};
  /* connects to Mcu board*/
  bool connectDevice(const QString & port, bool reset);
  bool setBaudrate(quint32 baudrate);
  quint32 baudrate() const;
  /* disconnect mcu board*/
  typedef enum {USER_COMMAND, DUE_ERROR,NEED_RECONNECT}DisconnectReason;
  void disconnectDevice(DisconnectReason reason);
  /* available ports in system*/
  void availablePorts(QStringList *plist) ;
  /* scans ports from list and finds which has mcu board*/
  void scanDevices(const QStringList &list_in , QStringList *plist_out);
  /* sets timeout*/
  void setTimeout(const quint32 &timeout);    
  /*is connected*/
  bool isConnected(void) const;
  /* resets mcu board*/
  void resetDevice();
  /* sets log mode on/off*/
  void setLogMode(bool on);
  /* true if log mod is on*/
  bool logMode() const;
  /*read session from MCU and compare to my_session*/
  bool checkSession();

  typedef struct
  {
    int minor;
    int major;
    int build;
    QDateTime timestamp;
  }Version;

  /* this check version */
  bool checkVersion();
  /* clear buffers*/
  void clearBuffers();  
  /* transfers data without each item control*/
  bool transferData(const QByteArray &data_write,QByteArray *pdata_read=0, int timeout=-1);
  /* sets single data to mcu*/
  bool setData(const QString &variable_name, qint32 variable_value);
  /* sets single data to mcu*/
  bool setData(const QString &variable_name, QByteArray variable_value);
  /* gets single data from mcu */
  bool getData(const QString &variable_name, qint32 *pvariable_value);
  /* gets single data from mcu */
  bool getData(const QString &variable_name, QByteArray *pvariable_value);
  /* sets data list to mcu */
  bool setDataList(const QList<Variable> &variables);
  /* gets data from mcu*/
   bool getDataList(QList<Variable> *pvariables);
  /* take value from last packet and convert it to integer*/
  //qint32 lastAsInteger(const QString &name);
  /* take value from last packet and convert it to bytearray*/
  //QByteArray lastAsArray(const QString &name);
  /* default timeout setting*/
  int versionMajor();
  int versionMinor();
  int versionBuild();
  QString versionTimeDate();
  QString project() const;

  void startStream(const QString &cmd,Applet *papplet);
  void stopStream();

 /* sequencers*/
  /* generates clocks */
  bool genClocks(const char* dio,quint32 count, quint32 inverted=0, quint32 delay=0);
  /* send sw SPI */
  bool sendSwSpi(const char *dio_clk
                 , const char *dio_write
                 , const char *dio_read
                 , quint32 edge
                 , quint32 half_period
                 , quint32 size_in_bits
                 , const QByteArray &data);
  bool readSwSpi(QByteArray *pdata);
  /* clock generator precise */
  bool genClocksP(const char *dio, quint32 count, quint32 time, const QString unit="us");
  /* capture data bits from port*/
  bool captureData(const char *dio, quint32 count
                                 , quint32 delay, quint32 trigger, QByteArray *pdata);
  // not implemented
  bool transferScanChain(const char *dio_clk, const char *dio_write
                         , const char *dio_read,
                         char operation, quint32 size, QByteArray &data, QByteArray *presult);
  //not implemented
  bool transferScanChainList(QList<Register *> chain_list);
  //not implemented
  bool captureTimeData(const char *dio, QByteArray *presult, const QString &unit="us");
  //not implemented
  bool sendByProtocol(const QString &protocol, const QByteArray &data, QByteArray *presult);

  /* set dio pin value */
  bool setDIO(const char *dio, quint32 val, bool send_immediately=true);  
  /* set dio dir  value*/
  bool setDIODIR(const char *dio, quint32 val, bool send_immediately=true);  
  /* get dio pin value*/
  bool getDIO(const char *dio, qint32 *pval);  
  void sendBreak();
  /* provides adc measuring */
  bool adcMeasure(const char *dio_clock,const char *dio_read
                  ,quint32 t1,quint32 t2, quint32 count,QList<int> *pdata);


  bool function(const char *name, const QByteArray &data, quint32 timeout
                , QByteArray *presult1=0
                , QByteArray *presult2=0
                , QByteArray *presult3=0
                , QByteArray *presult4=0
                , QByteArray *presult5=0);

  double ainVoltage(const char *ain);
  /* this grup of items used to provide access to DIO pins with MCU["DIOx"] form*/
  ControlBoard& operator [](const char *dio);
  void operator =(const int val) ;
  void operator =(const QByteArray &seq) ;  
  operator int();  
  void setAinVref(double vcc) {m_ain_vref=vcc;}
  double ainVref() const {return m_ain_vref;}
  enum{VREF_INTERNAL1V=1, VREF_EXTERNAL=2};
  bool configAin(quint32 options);
  /* extracts value from data buffer*/
  static bool extractText(const QByteArray &data, const QString &name, QByteArray *pvalue);
  /* extracts value from data buffer*/
  static bool extractByteArray(const QByteArray &data, const QString &name, QByteArray *pvalue);

private:
  // temporary variable
  QString m_cur_dio;
  // AIN voltage reference
  double m_ain_vref ;
  // capture mode
  bool m_capture_mode;
  // command is trigger for streaming start
  QString  m_capture_command;
  Applet *mp_capture_applet;

  bool reconnectDevice();
  /* */
  typedef enum { MICROCONTROLLER, SCRIPT, MOCKUP} Mode;
  typedef enum {LOG_MCU_RX,LOG_MCU_TX,LOG_MCU_INFO,LOG_MCU_ERROR} LogMessage;
  void log_message(LogMessage message , const QString message_text);
  /* check if mcu present on connected channel*/
  bool checkDevice();
  /* initialize mockup mode. Mockup mode is functionality evaluation mode*/
  void initMockup();
  /* reads data from mockup channel */
  void transferMockup(const QByteArray& data_out, QByteArray *pdata_in);
  /* reads data from mockup channel */
  void transferScript(const QByteArray& data_out, QByteArray *pdata_in);
  /* */
  static qint32 splitBySpace(const QByteArray &data, QList<QByteArray> *plist);
  /* thread runner */
  void run();
  Version m_version;
  QString m_project;
  /* in logging mode */
  bool m_logMode;
  /* timeout variable */
  quint32 m_timeout;
  /* script file name*/
  QString m_script_file;
  /* current port name */
  QString m_port;
  quint32 m_baudrate;
  /* open mode mockup script, physical microcontroller*/
  Mode m_mode;
  /* counter for errors*/
  qint32 m_errors_count;
  /* flag when time to check */
  bool m_is_time_to_check;
  /* my session number*/
  int m_my_session;
  /* finish flag to terminate thread*/
  bool m_finish;
  bool m_connected;
  bool m_suspend;
  /* channel pointer*/
  SerialPortChannel *mp_channel;

signals:
  /* signal for logging . Sends signal to LOG window*/
  void signal_log(int ,QString);
  void signal_tx( );
  void signal_rx();
  void signal_stream(bool);
  void signal_disconnected();
  void signal_connected();
  void signal_captureData(QByteArray);

};

#endif // PROTOCOL_H
