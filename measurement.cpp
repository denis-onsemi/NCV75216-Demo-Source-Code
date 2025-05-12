/*!
Class Measurement
-------------------
.Measurement
******************************
This class is complementary to Trilangulation class. It allows to start measurement
by multiple P215s simultaneously, configure multiple parameters of the measurement
and after the measurement is done, request results from MCU and send them to the
Triangulation class for analysis.
******************************
*/

#include "measurement.h"
#include "tmccontroller.h"
#include "triangulation.h"
#include "ui_measurement.h"
#include "mainapp.h"
#include "customdialogs.h"
#include <QTimer>
#include <QMessageBox>

Measurement * gp_Measurement;
QByteArray received_data_string;
double received_data[3][3][2];
int DUT1_duration = 60;     //default - worst case - longest measurement duration

/****************************************************************************
 *                              [1] REGISTERS
 ****************************************************************************/
quint32 MEASVALUES[] = {400,580,780,1010};
//define structure of command pulses
#define DELAY_400US     "wwww"
#define DELAY_580US     "wwwwuuuuuuuuu"
#define DELAY_780US     "wwwwwwuuuuuuuu"
#define DELAY_1010US    "wwwwwwwwwuu"

const char *SIGNAL_PATTERN_3TRANSDUCERS [2][3]= {
    {"111X00wwwuu0XX" DELAY_400US "111"             //pattern where DUT1 is in direct mode and DUT2,3 are in indirect mode, THRi1
    ,"1110X0wwwuuX0X" DELAY_400US "111"             //pattern where DUT2 is in direct mode and DUT1,3 are in indirect mode, THRi1
    ,"11100XwwwuuXX0" DELAY_400US "111"},           //pattern where DUT3 is in direct mode and DUT1,2 are in indirect mode, THRi1
    {"111X00wuuuuuu0XX" DELAY_400US "111"           //pattern where DUT1 is in direct mode and DUT2,3 are in indirect mode, THRi2
    ,"1110X0wuuuuuuX0X" DELAY_400US "111"           //pattern where DUT2 is in direct mode and DUT1,3 are in indirect mode, THRi2
    ,"11100XwuuuuuuXX0" DELAY_400US "111"}          //pattern where DUT3 is in direct mode and DUT1,2 are in indirect mode, THRi2
};
//Measurement timer
QTimer *ptimer;
/****************************************************************************
 *                              [2] APPLICATION
 ****************************************************************************/
Measurement::Measurement(QWidget *parent) :
    Applet(parent)
{
    ui =  new Ui::Measurement;
    ui->setupUi(this);
    setWindowTitle("Measurement");
    ptimer = new QTimer();
    ui->gb_advsettings->setChecked(false);
    ui->gb_advsettings->setExpanded(false);
}


bool Measurement::initialize()
{
    addDataNode(&DUT);          //interconnection with TmcController
    addDataNode(&R4);           //interconnection with TmcController
    connect(ptimer,SIGNAL(timeout()),this,SLOT(slot_getdata_delay()));
    return true;
}


/****************************************************************************
 *                              [3] FUNCTIONALITY
*****************************************************************************/
QVariant Measurement::FUNCTION_LIST()
{
    //UNREFERENCED_PARAMETER(arg_data);                   // to avoid warning, parameter is not used

    FUNCTION("Start","Start/Stop measurement")
    {
        bool result = false;
        QByteArray dataToSend;
        //-- Start measurement --
        if(ui->pbMeasurement_Start->isChecked())
        {
            dataToSend = "set DIOSEQ8=1\n";                                 //send command to MCU - begin measurement cycle
            result = BENCH()->board()->transferData(dataToSend);
            ptimer->start(2*(ui->spInterval->value())+DUT1_duration+10);    //wait until the measurement is finished, then request results from MCU
        }
        //-- End measurement --
        else
        {
            ptimer->stop();
            result = true;
        }
        return result;
    }
    /**************************************************/
    FUNCTION("Once","Start only one measurement cycle")
    {
        bool result = 0;
        QByteArray dataToSend;
        dataToSend = "set DIOSEQ8=1\n";
        result = BENCH()->board()->transferData(dataToSend);
        ptimer->start(2*(ui->spInterval->value())+DUT1_duration+10);        //wait until the measurement is finished, then request results from MCU
        return result;
    }
    /**************************************************/
    FUNCTION("getdata","Request measurement results from microcontroller")
    {
        bool result = MCU.transferData("set DIOSEQ8=get,\n", &received_data_string);    //Send 'request data from MCU' command
        //extract data from received_data_string
        QList<QByteArray> separated_data = received_data_string.split(',');
        //qDebug() << "received data, separated: "<< separated_data;
        quint32 list_index = 0;
        for(int i=0; i<3; i++)          //DUT 1-3
        {
            for(int j=0; j<3; j++)      //measurement 1-3
            {
                for(int k=0; k<2; k++)  //TOF 1-2
                {
                    //subtracting -60us from all times - debounce time between actual echo detection and echo reporting on IO line
                    double value = separated_data.at(list_index).toDouble();
                    if(value > 60)
                        received_data[j][i][k] = value-60;
                    else
                        received_data[j][i][k] = value;
                    list_index++;
                }
            }
        }
        gp_Triangulation->runAppCommand("ReadTriData");         //refresh Triangulation window
        runAppCommand("Start");                                 //Check if periodic measurement is requested
        return result;
    }
    /**************************************************/
    FUNCTION("DefaultCfg","Loads default configuration from 'default.bench' file")
    {
        const int MAX_BYTES = 1000;
        QString filename = "default.bench";

        QFile file(filename);
        if (file.open(QFile::ReadOnly) )
        {
            char data[MAX_BYTES] = {0};
            qint32 bytes = 0;
            Applet *papplet = NULL;
            bool in_brackets = false;
            QByteArray line;
            QByteArray buffer;
            while ( (bytes = file.readLine(data, MAX_BYTES)) > 0 )
            {
                line = QByteArray(data, bytes - 1);
                if (line == "{" )
                {
                    in_brackets = true;
                    buffer.clear();
                }
                else if (line == "}" && in_brackets)
                {
                    if (papplet)
                    {
                        papplet->loadAppData(buffer);
                    }
                    in_brackets = false;
                    buffer.clear();
                }
                else if ( !in_brackets )
                {
                    papplet = MainApp::findApplet(QString(line),MainApp::BY_NAME);
                    buffer.clear();
                }
                else if (in_brackets)
                {
                    buffer += QByteArray(data);
                }
            }
            file.close();
        }

        //Write configuration to all P215
        double backupDUT = DUT.value();
        for(int m=1; m<4; m++)      //DUT 1-3
        {
            DUT.setValue(m);
            gp_TmcController->runAppCommand(QString("WriteAll"),QVariant());
        }
        DUT.setValue(backupDUT);
        return true;
    }
    /**************************************************/
    FUNCTION("uCConfig","Load configuration to microcontroller")
    {
        bool result;
        //Get requested period of measurement
        QString str_period = ui->spInterval->text();
        quint32 RC_period = (quint32)((str_period.toDouble()*0.001)/(1/468750.0));
        QByteArray period;
        if(RC_period < 10000)
            period.append("0");
        period.append(QByteArray::number(RC_period, 10));
        period.append(",");

        //Get requested measurement duration
        quint8 backupDUT = DUT.value();
        DUT.setValue(1);
        gp_TmcController->runAppCommand(QString("ReadREG4"),QVariant());
        DUT1_duration = R4.value("[0:3]")*6;    //0-60ms, 6ms steps
        DUT.setValue(backupDUT);
        QByteArray duration;
        if(DUT1_duration < 10)
            duration.append("0");
        duration.append(QByteArray::number(DUT1_duration, 10));
        duration.append(",");

        if(str_period.toInt() < DUT1_duration)  //meas. duration is longer than meas. cycle period
        {
            QMessageBox::information(0,"Warning","Measurement duration set in 'NM Controller' window must be longer than selected measurement period!");
            return true;
        }

        quint8 IndirThr = ui->cbIndirectThr->currentIndex();
        QByteArray dataToSend;
        //Send configuration (structure of command pulses + selection of threshold table)
        dataToSend = "set DIOSEQ8=ooo,31,44,21,"
                    + period
                    + duration
                    + QByteArray(SIGNAL_PATTERN_3TRANSDUCERS[IndirThr][0])
                    + ";"
                    + QByteArray(SIGNAL_PATTERN_3TRANSDUCERS[IndirThr][1])
                    + ";"
                    + QByteArray(SIGNAL_PATTERN_3TRANSDUCERS[IndirThr][2])
                    + "\n";
        result = BENCH()->board()->transferData(dataToSend);
        return result;
    }
    /**************************************************/
    FUNCTION("advsettsave","Save changes in advanced settings")
    {
        gp_Triangulation->saveSettings(ui->le_S2X->text().toDouble(), ui->le_S2Y->text().toDouble(), ui->le_S3X->text().toDouble(), ui->le_S3Y->text().toDouble(),
                                       ui->le_SOS->text().toDouble(), ui->cb_projection->currentIndex(), ui->cb_AdAxRg->isChecked(), ui->spMemTol->value(),
                                       ui->spMMThr->value(), ui->spAlgTol->value(), ui->spIntTol->value());
        return true;
    }


    return false;
}
/**************************************************/
void Measurement::slot_getdata_delay()
{
    ptimer->stop();
    runAppCommand("getdata");
}
