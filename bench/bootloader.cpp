#include "bootloader.h"
#include "mainapp.h"
#include <QFile>
#include <qmath.h>

#define SIGN(_val_) ((_val_)<0)?-1:(((_val_)==0)?0:1)


Bootloader::Bootloader(QWidget *parent) :
    QWidget(parent)
{
    setupUi();
    pbDefault->setEnabled(QFile::exists(":/project/firmware.hex"));
}

Bootloader::~Bootloader()
{
    delete gridLayout;
    delete label;
    delete teInfo ;
    delete pbLoadHex;
    delete pbDefault;
    delete pbFlash;
    delete pbRestart;
    delete pbProgress;
}

static int Hex2Byte(quint8 num)
{
    int ret = 0;
    if (num < 0x3A)
        ret = (int)num - '0';
    else if (num < 0x47)
        ret = (int)num - 'A' + 10;
    else
        ret = (int)num - 'a' + 10;
   return ret;
}

void Bootloader::slot_pbLoadHex_clicked()
{
    QFileDialog dialog;
    loadHexFile(dialog.getOpenFileName(0, "Spec path",
                                       "",
                                       "Files (*.hex)"));
}

void Bootloader::slot_pbFlash_clicked()
{

    quint32 address = 0x80050000ul;
    int packet_size = 256;
    quint32 cnt = 0;
    quint32 cnt_crc = 0;
    bool error = false;
    QString str_send;
    //int bytes_to_send = 10000;
    int bytes_to_send = m_bytes_length;
    address -= (quint32)packet_size;
    uint crc = 0;
    uint m;
    uint crcmsb;
    uint acrc;
    uint num;
    uint CRC_POLYNOMIAL = 0x04C11DB7;
    uint CRC_INIT = 0;
    pbProgress->setMaximum( bytes_to_send/packet_size);
    pbProgress->setValue(0);
    try
    {
        teInfo->appendHtml("Flash...");
        for (int i = 0; (i < bytes_to_send/packet_size) && (!error); i++ )
        {
            address = address +(quint32)packet_size;
            cnt_crc = cnt;                          // CRC computation start
            crc = CRC_INIT;
            for (int j = 0; j < packet_size; j++)
            {
                acrc = crc;
                num = m_bytes[cnt_crc];

                m = 8;
                do
                {
                    crcmsb = (acrc >> 24);
                    acrc <<= 1;
                    if ((num & 0x80) == 0x80)
                        acrc |= 1;
                    if ((crcmsb & 0x80) == 0x80)
                        acrc ^= CRC_POLYNOMIAL;
                    num <<= 1; //next bit
                } while ((--m) > 0);

                crc = acrc;
                cnt_crc++;
            }                                       // CRC computation end
            str_send = "set flash=";
            str_send += QString("%1").arg(address,8,16,QChar('0'));
            str_send += QString("%1").arg(crc,8,16,QChar('0'));
            str_send += QString("%1").arg(packet_size,8,16,QChar('0'));
            for (int j = 0; j < packet_size;j++ )
            {
                str_send += QString("%1").arg(m_bytes[cnt],2,16,QChar('0'));
                cnt++;
            }
            str_send += "\r";
            QByteArray received;
            bool transfer_ok = MCU.transferData(str_send.toLatin1(),&received);
            if (!transfer_ok || received.contains("flash:err"))
            {
                teInfo->appendHtml("<font color=red> Error</font>");
                QMessageBox::warning(0,"","Some error during sending appears");
                error = true;
            }
            pbProgress->setValue(i);
            QApplication::processEvents();
        }


        if(((bytes_to_send % packet_size)!=0) && (!error))
        {
            address = address + (uint)packet_size;

            cnt_crc = cnt;                          // CRC computation start
            crc = CRC_INIT;
            for (int j = 0; j < (bytes_to_send % packet_size); j++)
            {
                acrc = crc;
                num = m_bytes[cnt_crc];

                m = 8;
                do
                {
                    crcmsb = (acrc >> 24);
                    acrc <<= 1;
                    if ((num & 0x80) == 0x80)
                        acrc |= 1;
                    if ((crcmsb & 0x80) == 0x80)
                        acrc ^= CRC_POLYNOMIAL;
                    num <<= 1; //next bit
                } while ((--m) > 0);

                crc = acrc;
                cnt_crc++;
            }                                       // CRC computation end


            str_send = "set flash=";
            str_send += QString("%1").arg(address,8,16,QChar('0'));
            str_send += QString("%1").arg(crc,8,16,QChar('0'));
            str_send += QString("%1").arg((bytes_to_send % packet_size),8,16,QChar('0'));
            for (int j = 0; j < (bytes_to_send % packet_size); j++)
            {
                str_send += QString("%1").arg(m_bytes[cnt],2,16,QChar('0'));
                cnt++;
            }
            str_send += "\r";
            QByteArray received;
            bool transfer_ok = MCU.transferData(str_send.toLatin1(), &received);
            while ((!received.contains("flash:ok")) && (!received.contains("flash:err")))
            {

            }
            if (!transfer_ok ||received.contains("flash:err"))
            {
                QMessageBox::warning(0,"","Some error during sending appears");
                error = true;
            }
            pbProgress->setValue(pbProgress->maximum());
        }

        m_data_to_send[0] = (byte)'s'; m_data_to_send[1] = (byte)'e'; m_data_to_send[2] = (byte)'t'; m_data_to_send[3] = (byte)' ';     // set flash=
        m_data_to_send[4] = (byte)'f'; m_data_to_send[5] = (byte)'l'; m_data_to_send[6] = (byte)'a'; m_data_to_send[7] = (byte)'s';
        m_data_to_send[8] = (byte)'h'; m_data_to_send[9] = (byte)'=';
        m_data_to_send[10] = (byte)'8'; m_data_to_send[11] = (byte)'0'; m_data_to_send[12] = (byte)'0'; m_data_to_send[13] = (byte)'7'; // ADDRESS
        m_data_to_send[14] = (byte)'f'; m_data_to_send[15] = (byte)'f'; m_data_to_send[16] = (byte)'f'; m_data_to_send[17] = (byte)'C';
        m_data_to_send[18] = (byte)'1'; m_data_to_send[19] = (byte)'2'; m_data_to_send[20] = (byte)'3'; m_data_to_send[21] = (byte)'4'; // CHECKSUM
        m_data_to_send[22] = (byte)'a'; m_data_to_send[23] = (byte)'b'; m_data_to_send[24] = (byte)'c'; m_data_to_send[25] = (byte)'d';
        m_data_to_send[26] = (byte)'0'; m_data_to_send[27] = (byte)'0'; m_data_to_send[28] = (byte)'0'; m_data_to_send[29] = (byte)'0'; // LENGTH
        m_data_to_send[30] = (byte)'0'; m_data_to_send[31] = (byte)'0'; m_data_to_send[32] = (byte)'0'; m_data_to_send[33] = (byte)'4';
        m_data_to_send[34] = (byte)'1'; m_data_to_send[35] = (byte)'2'; m_data_to_send[36] = (byte)'3'; m_data_to_send[37] = (byte)'4'; // code 1234ABCD
        m_data_to_send[38] = (byte)'a'; m_data_to_send[39] = (byte)'b'; m_data_to_send[40] = (byte)'c'; m_data_to_send[41] = (byte)'d';
        m_data_to_send[42] = 13;

        if (!error)
        {
            QByteArray received;
            MCU.transferData(m_data_to_send, &received);

            while ((!received.contains("flash:ok")) && (!received.contains("flash:err")))
            {

            }
            if (received.contains("flash:err"))
            {
                QMessageBox::warning(0,"","Some error during sending appears");
                error = true;
            }
            teInfo->appendHtml("Finished.\n Please restart MCU! it can take up to 30 seconds ");
        }
    }
    catch(...)
    {
        QMessageBox::warning(0,"","ONMCU board is not connected!");
    }
}

void Bootloader::slot_pbDefault_clicked()
{    
    loadHexFile(":/project/firmware.hex");
}

void Bootloader::slot_pbRestart_clicked()
{
    teInfo->appendHtml("Restarting...");
    QApplication::processEvents();
    MCU.resetDevice();
    teInfo->appendHtml("Done!");
}

bool Bootloader::loadHexFile(const QString &file_name)
{

    int i, j, k;
    int high_addr = 0;
    int low_addr = 0;
    int start_addr = 0;
    int high_addr_first = 1;
    int low_addr_first = 1;
    int length;
    quint8 data;
    int checksum = 0;
    int checksum_or = 0;
    m_bytes_length = 0;

    if (!file_name.isEmpty())
    {
        QFile file(file_name);
        if(file.open(QFile::ReadOnly))
            m_hexfile = file.readAll();
        else return false;
    }
    else return false;
    if ((m_hexfile[0] != ':') || (m_hexfile[1] != '0'))
    {
        pbFlash->setEnabled(false);
        QMessageBox::warning(0,"Error","bad HEX file format!");
    }
    else
    {
        for (i = 0; i < m_hexfile.size(); i++)
        {
            if (m_hexfile[i] == ':')
            {

                checksum = 0;
                length = 0;
                for (j = 0; j < 2; j++)
                {
                    length += (byte)(qPow(16, (1 - j)) * Hex2Byte(m_hexfile[i + 1 + j]));
                }
                checksum = length;

                low_addr = 0;
                for (j = 0; j < 4; j++)
                {
                    low_addr += (int)(qPow(16, (3 - j)) * Hex2Byte(m_hexfile[i + 3 + j]));
                }
                checksum += low_addr & 0xFF;
                checksum += (low_addr >> 8) & 0xFF;
                checksum += Hex2Byte(m_hexfile[i + 8]);
                if ((quint8)m_hexfile[i + 8] == 0x30)             //data
                {
                    if(low_addr_first == 1)
                    {
                        start_addr += low_addr;
                        low_addr_first--;
                    }

                    for (k = 0; k < length; k++)
                    {
                        data = 0;
                        for (j = 0; j < 2; j++)
                        {
                            data += (byte)(qPow(16, (1 - j)) * Hex2Byte(m_hexfile[i + 9 + j + 2 * k]));
                        }
                        checksum += data;
                        m_bytes[low_addr + 65536 * high_addr + k - start_addr] = data;
                        m_bytes_length = low_addr + 65536 * high_addr + k;
                    }
                }
                else if ((quint8)m_hexfile[i + 8] == 0x34)       // high_addr
                {
                    high_addr = 0;
                    for (j = 0; j < 4; j++)
                    {
                        high_addr += ((int)(qPow(16, (3 - j)) * Hex2Byte(m_hexfile[i + 9 + j])));

                    }
                    checksum += high_addr & 0xFF;
                    checksum += (high_addr >> 8) & 0xFF;
                    high_addr = high_addr & 0xFF;

                    if(high_addr_first == 1)
                    {
                        start_addr += ((high_addr & 0xFF) << 16);
                        high_addr_first--;
                    }

                }
                else
                {
                    for (k = 0; k < length; k++)
                    {
                        data = 0;
                        for (j = 0; j < 2; j++)
                        {
                            data += (quint8)(qPow(16, (1 - j)) * Hex2Byte(m_hexfile[i + 9 + j + 2 * k]));
                        }
                        checksum += data;
                    }
                }
                checksum_or = 0;
                for (j = 0; j < 2; j++)
                {
                    checksum_or += ((int)(qPow(16, (1 - j)) * Hex2Byte(m_hexfile[i + 9 + j + 2 * length])) & 0xFF);
                }
                checksum = ((~checksum) + 1) & 0xFF;
                if (checksum != checksum_or)
                    QMessageBox::warning(0,"","Error in HEX file!");
            }
        }
        teInfo->appendHtml("File loaded ::<font color=blue>" +file_name+"</font>");
        pbFlash->setEnabled( true);
        pbProgress->setValue(0);
    }
    return true;
}

void Bootloader::setupUi()
{
    setFixedSize(518, 163);
    gridLayout = new QGridLayout(this);
    label = new QLabel(this);
    label->setPixmap(QPixmap(QString::fromUtf8(":/onmcu.png")));
    label->setScaledContents(true);
    gridLayout->addWidget(label, 0, 0, 4, 1);
    teInfo = new QPlainTextEdit(this);
    gridLayout->addWidget(teInfo, 0, 1, 4, 1);
    pbLoadHex = new QPushButton(this);
    gridLayout->addWidget(pbLoadHex, 0, 2, 1, 1);
    pbDefault = new QPushButton(this);
    pbDefault->setEnabled(false);
    gridLayout->addWidget(pbDefault, 1, 2, 1, 1);
    pbFlash = new QPushButton(this);
    pbFlash->setEnabled(false);
    gridLayout->addWidget(pbFlash, 2, 2, 1, 1);
    pbRestart = new QPushButton(this);
    gridLayout->addWidget(pbRestart, 3, 2, 1, 1);
    pbProgress = new QProgressBar(this);
    pbProgress->setValue(0);
    gridLayout->addWidget(pbProgress, 4, 0, 1, 3);
    setWindowTitle("FIRMWARE Update");
    label->setText(QString());
    pbLoadHex->setText("LoadHex");
    pbDefault->setText("Default");
    pbFlash->setText("Flash");
    pbRestart->setText("Restart");
    setWindowModality(Qt::ApplicationModal);
    setWindowFlags( Qt::CustomizeWindowHint | Qt::WindowCloseButtonHint );
    connect(this->pbDefault,SIGNAL(clicked()),this,SLOT(slot_pbDefault_clicked()));
    connect(this->pbFlash,SIGNAL(clicked()),this,SLOT(slot_pbFlash_clicked()));
    connect(this->pbLoadHex,SIGNAL(clicked()),this,SLOT(slot_pbLoadHex_clicked()));
    connect(this->pbRestart,SIGNAL(clicked()),this,SLOT(slot_pbRestart_clicked()));

}
