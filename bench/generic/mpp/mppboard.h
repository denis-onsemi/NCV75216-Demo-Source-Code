/**
 * @file:mcuprotocol.h   -
 * @description:
 * @project: BENCH OnSemiconductor
 * @date: 2014\11\27 13-50-31
 *  This class intended for communication with MCU board using simple text protocol.
 *  For detailed information about protocol see: protocol_mcu_bench.docx
 */


#ifndef MPPBOARD_H
#define MPPBOARD_H

#include <QThread>
#include <QList>

#include <QVariant>
#include <QTcpSocket>
#include <QBuffer>

#include <QDateTime>
#include <QTimer>
#include "register.h"
#include "applet.h"
#include <QAbstractSocket>

#include <QPair>


class MppBoard:public QObject
{
  Q_OBJECT
public:  
  MppBoard();
  ~MppBoard();

  enum {INPUT = 1, OUTPUT   =  0};


  class Item :public QPair<QString,QVariant>
  {
  public:
      Item(const QString &name, const QVariant &value= QVariant()){ first = name;second = value;}
      QString name() const {return first;}
      QVariant value() const {return second;}
      QString valueToString() const;
      void valueFromString(const QString &txt);
      void setName(const QString &name) {first = name;}
      void setValue(const QVariant &value) {second = value;}
  };

  typedef QList<Item> ItemsList;

  /* connects to Mcu board*/
  bool connectDevice(const QString & ip_and_socket, bool reset);  
  /* disconnect mcu board*/  
  void disconnectDevice();
  /* available ports in system*/
  void availablePorts(QStringList *plist) ;  
  /* sets timeout*/
  void setTimeout(const quint32 &timeout){ if(timeout>m_timeout) m_timeout = timeout;}
  int timeout() const {return m_timeout;}
  /*is connected*/
  bool isConnected(void) ;
  bool isSocketConnected() const {return mp_socket && (mp_socket->state() == (QAbstractSocket::ConnectedState));}
  /* resets mcu board*/
  void resetDevice();
  /* sets log mode on/off*/
  void setLogMode(bool on){m_logMode = on;}
  /* true if log mod is on*/
  bool logMode() const {return m_logMode;}
  /*read session from MCU and compare to my_session*/
  bool checkSession();

  /* this check version */
  bool checkVersion();
  /* clear buffers*/
  void clearBuffers();  
  /* transfers data without each item control*/
  bool transferData(const QByteArray &data_write, QByteArray *pdata_read=0, int to=-1, int *time_elapsed=0);
  /* sets single data to mcu*/
  bool setData(const ItemsList &data, int timeout=-1, int *time_elapsed=0);
  /* gets single data from mcu */
  bool getData(ItemsList *pdata, int timeout=-1, int *time_elapsed=0);

  /* set register */
  bool setData(Register &reg, int timeout=-1, int *time_elapsed=0);
  /* gets single data from mcu */
  bool getData(Register *preg, int timeout=-1, int *time_elapsed=0);

  /* sets single data to mcu*/
  bool setDataItem(const Item &item, int to=-1, int *time_elapsed=0);
  /* gets single data from mcu */
  bool getDataItem(Item *pitem, int to=-1, int *time_elapsed=0);

  /* rns function single data from mcu */
  bool runFunction(const QString &funcname, const QVariant  &params, QVariant *res=0, int to=-1, int *time_elapsed=0);
  /*restart beagleboard*/
  bool restart();
  /* reset */
  bool reset();
  /* shutdown */
  bool shutdown();


  QString version() const {return m_version;}

  QString project() const {return m_project;}

  typedef enum {LOG_MPP_RX,LOG_MPP_TX,LOG_MPP_INFO,LOG_MPP_ERROR} LogMessage;
  void log_message(LogMessage message , const QString message_text);
  
  /* this grup of items used to provide access to DIO pins with MCU["DIOx"] form*/
  MppBoard& operator [](const char *dio);
  void operator =(const int val) ;
  void operator =(const QByteArray &seq) ;  
  operator int();    

private:
  bool m_connected;
  QTcpSocket *mp_socket;
  // temporary variable
  QString m_cur_dio;

  bool reconnectDevice();

  /* check if mcu present on connected channel*/
  bool checkConnected();
    
  /* thread runner */
  QString m_version;
  QString m_project;
  /* in logging mode */
  bool m_logMode;
  /* timeout variable */
  quint32 m_timeout;  
  /* current port name */
  QString m_socket;
  QString m_ip;
      
  /* flag when time to check */
  bool m_is_time_to_check;
  /* my session number*/
  int m_my_session;
  /* finish flag to terminate thread*/
  bool m_finish;
  bool m_suspend;  

private slots:
  void slot_stateChanged(QAbstractSocket::SocketState);

signals:
  /* signal for logging . Sends signal to LOG window*/
  void signal_log(int ,QString);
  void signal_tx( );
  void signal_rx();  
  void signal_disconnected();
  void signal_connected();

  void signal_stream(bool);
  void signal_captureData(QByteArray);

};

#endif // PROTOCOL_H
