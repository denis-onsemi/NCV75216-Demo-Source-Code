#ifndef BOOTLOADER_H
#define BOOTLOADER_H

#include <QWidget>
#include <QGridLayout>
#include <QLabel>
#include <QPlainTextEdit>
#include <QPushButton>
#include <QProgressBar>


class Bootloader : public QWidget
{
    Q_OBJECT
    
public:
    explicit Bootloader(QWidget *parent = 0);
    ~Bootloader();
    
private slots:
    void slot_pbLoadHex_clicked();
    void slot_pbFlash_clicked();
    void slot_pbDefault_clicked();
    void slot_pbRestart_clicked();

private:
    QGridLayout *gridLayout;
    QLabel *label;
    QPlainTextEdit *teInfo;
    QPushButton *pbLoadHex;
    QPushButton *pbDefault;
    QPushButton *pbFlash;
    QPushButton *pbRestart;
    QProgressBar *pbProgress;

    bool loadHexFile(const QString &file_name);
    void setupUi();
    QByteArray m_data_to_send;
    quint8 m_bytes[200000];
    QByteArray m_hexfile;
    int m_bytes_length;
};

#endif // BOOTLOADER_H
