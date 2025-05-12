/**
 * @file:localconnector.h
 * @description:
 *    This is named pipe connector for controlling app.
 * @project: BENCH OnSemiconductor
 * @date: 2014\11\27 13-50-31
 *
 */

#ifndef LOCALCONNECTOR_H
#define LOCALCONNECTOR_H
#include <QLocalServer>

class LocalServer: public QObject
{
  Q_OBJECT

public:
  LocalServer();
  bool initialize(const QString &pipename);
  const QString & pipeName() const;
  void setLogMode(bool enable);
  bool logMode() const;

private:
  /* if in log mode*/
  bool m_log_mode;
  /*pointer to local server*/
  QLocalServer *mp_server;
  /* pipe name with full path*/
  QString m_file_pipe_name;
  /* pipename without path*/
  QString m_pipename;
  /* input buffer */
  QByteArray input;
  /* output buffer */
  QByteArray output;
  /* process input .reads from input buffer*/
  QByteArray parse(const QByteArray &data_in);
  /*send log message*/
  void log_message(const QString &text);

private slots:
  /* read data and execute set/get/run */
  void processConnection();

};

#endif // LOCALCONNECTOR_H
