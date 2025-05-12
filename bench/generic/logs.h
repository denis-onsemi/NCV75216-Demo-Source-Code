/**
 * @file:logs.h
 * @description:
 *    This is Logs window applet
 * @project: BENCH OnSemiconductor
 * @date: 2014\11\27 13-50-31
 *
 */

#ifndef LOGS_H
#define LOGS_H

#include <QTextEdit>
#include <QWidget>
#include <QFile>
#include "applet.h"
#include <QToolButton>
#include "customcontrols_htmllistbox.h"
#include <QMap>
#define LOG_NO_ERROR    (0)
#define LOG_ERROR       (-1)
#define LOG_WARNING     (1000)
#define LOG_COLOR(C)       (C+100)

class Logs : public Applet
{
  Q_OBJECT

public:

  explicit Logs(QWidget *parent = 0);
  void createCustomUi();
  ~Logs();
  virtual QVariant runAppCommand(const QString &name, const QVariant &data);
  virtual bool initialize();
  void assignSignalNames(QStringList tags, QStringList signal_names);
  void showSignalNames(bool on);
  QString retranslateByDict(const QString &text);

public slots:
  void slot_addLog(int type, const QString text);

private slots:
  void slot_pbClear_clicked();
  void slot_cbSaveLog_toggled(bool checked);
  void slot_LogContextMenu(QPoint);
  void slot_ActionCopy();
  void slot_ActionCopyAll();
  void slot_ControllerSend();
  void slot_ResetStatus();

private:
    QMap<QString, QString> m_dict;
    void updateStatus();
    void resetStatus();
    int m_error_count;
    int m_inf_count;
    int m_warn_count;
    QGridLayout *gridLayout;
    QPushButton *pbClear;
    QLineEdit *pleControl;
    QLabel *mp_status;
    QCheckBox *mp_cbTranslater;
    HtmlListBox *mp_list;
    QCheckBox *mp_cbSaveLog;
    QCheckBox *mp_cbMessageBoxes;
    QPushButton *mp_sound_on;    
    QFile *mp_log_file;
};

extern Logs *gp_Logs;
#define LOGS gp_Logs
#endif // LOGS_H
