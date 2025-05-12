/*!

Class TmcController
-------------------
.TmcController
******************************
This class ......
******************************

*/

#include "tmccontroller.h"
#include "ui_tmccontroller.h"
#include "mainapp.h"
#include <QMessageBox>
#include <QtDebug>
#include "customdialogs.h"

#include "board_def.h"

TmcController * gp_TmcController;


/****************************************************************************
 *                              [0] OPTIONS
 ****************************************************************************/

/*applet window title */
#define APPLET_TITLE        ("NM Controller")


/****************************************************************************
 *                              [1] REGISTERS
 ****************************************************************************/
/*!
*_register_ DIO*::
* DIO board pins
*/
Register DIO("@register=DIO;"
             "@bit=LIN1_EN      @pin=DIO50      @value=1        @dir=OUT;"
             "@bit=LIN2_EN      @pin=DIO32      @value=1        @dir=OUT;"
             "@bit=LIN3_EN      @pin=DIO33      @value=1        @dir=OUT;"
             "@bit=LIN4_EN      @pin=DIO55      @value=1        @dir=OUT;"
             "@bit=LIN1_EN      @pin=DIO50      @value=1        @dir=OUT;"
             );

/*!
*_register_ R0*::
* Measured Junction Temperature
*/
Register R0(
        "TEMP[7:0]=1#Measured Junction Temperature"
        ,"R0"
        );

/*!
*_register_ R1*::
* Sensor Status
* Measured Reverbration period
*/
Register R1(
        "STAT0=0#Acoustic Noise Flag;"
        "STAT1=0#VSUP Under-voltage or Over-voltage during TX;"
        "STAT2=0#TX Period Update Required;"
        "STAT3=0#TX Period Update Direction;"
        "STAT4=0#Unexpected Decay Time (decay time too short);"
        "STAT5=0#End of Reverberation Time-out;"
        "STAT6=0#THS_ERROR Flag (Thermal Shutdown Error);"
        "STAT7=0#EEPROM Two-Bit Error or EEPROM CRC Error or POR flag;"
        "MESREVERB[10:0]=0#Measured Reverbration Period"
        ,"R1"
        );

/*!
*_register_ R2A*::
* Carrier TX/RX period
*/
Register R2A(
        "CARRIER_PER[10:0]=0#Carrier TX/RX period"
        ,"R2A"
        );

/*!
*_register_ R2B*::
* Delta TX/RX period
*/
Register R2B(
        "DTX_PER[7:0]=0#Delta TX Period;"
        "DRX_PER[7:0]=0#Delta RX Period"
        ,"R2B"
        );

/*!
*_register_ R3*::
* Carrier TX/RX period
*/
Register R3(
        "BURST_PULSE_CNT[4:0]=16#TX burst pulse count"
        ,"R3"
        );

/*!
*_register_ R4*::
* Measurement duration
*/
Register R4(
        "MEAS_DUR[3:0]=0#Measurement duration"
        ,"R4"
        );


/*!
*_register_ R5*::
* Thresholds 1
*/
Register R5(
        "LVL0[5:0]=32#Measurement duration;"
        "LVL1[5:0]=32#Measurement duration;"
        "LVL2[5:0]=32#Measurement duration;"
        "LVL3[5:0]=32#Measurement duration;"
        "LVL4[5:0]=32#Measurement duration;"
        "LVL5[5:0]=32#Measurement duration;"
        "LVL6[5:0]=32#Measurement duration;"
        "LVL7[5:0]=32#Measurement duration;"
        "LVL8[5:0]=32#Measurement duration;"
        "LVL9[5:0]=32#Measurement duration;"
        "LVL10[5:0]=32#Measurement duration;"
        "LVL11[5:0]=32#Measurement duration;"
        "DT0[3:0]=0#Threshold;"
        "DT1[3:0]=0#Threshold;"
        "DT2[3:0]=0#Threshold;"
        "DT3[3:0]=0#Threshold;"
        "DT4[3:0]=0#Threshold;"
        "DT5[3:0]=0#Threshold;"
        "DT6[3:0]=0#Threshold;"
        "DT7[3:0]=0#Threshold;"
        "DT8[3:0]=0#Threshold;"
        "DT9[3:0]=0#Threshold;"
        "DT10[3:0]=0#Threshold;"
        "DT11[3:0]=0#Threshold"
        ,"R5"
        );

/*!
*_register_ R6*::
* Thresholds 2
*/
Register R6(
        "LVL0[5:0]=32#Measurement duration;"
        "LVL1[5:0]=32#Measurement duration;"
        "LVL2[5:0]=32#Measurement duration;"
        "LVL3[5:0]=32#Measurement duration;"
        "LVL4[5:0]=32#Measurement duration;"
        "LVL5[5:0]=32#Measurement duration;"
        "LVL6[5:0]=32#Measurement duration;"
        "LVL7[5:0]=32#Measurement duration;"
        "LVL8[5:0]=32#Measurement duration;"
        "LVL9[5:0]=32#Measurement duration;"
        "LVL10[5:0]=32#Measurement duration;"
        "LVL11[5:0]=32#Measurement duration;"
        "DT0[3:0]=0#Threshold;"
        "DT1[3:0]=0#Threshold;"
        "DT2[3:0]=0#Threshold;"
        "DT3[3:0]=0#Threshold;"
        "DT4[3:0]=0#Threshold;"
        "DT5[3:0]=0#Threshold;"
        "DT6[3:0]=0#Threshold;"
        "DT7[3:0]=0#Threshold;"
        "DT8[3:0]=0#Threshold;"
        "DT9[3:0]=0#Threshold;"
        "DT10[3:0]=0#Threshold;"
        "DT11[3:0]=0#Threshold"
        ,"R6"
        );

/*!
*_register_ R7*::
*
*/
Register R7(
        "RX_GAIN_CODE[6:0]=0#Static RX gain code;"
        "DYN_GAIN_ENA=0#Static RX gain code;"
        "NOISE_THR[5:0]=32#Noise Threshold;"
        "NOISE_FLOOR[5:0]=4#Noise Floor"
        ,"R7"
        );

/*!
*_register_ R8*::
*
*/
Register R8(
        "DELTA_GAIN0[6:0]=0#Delta Gain 0;"
        "DELTA_GAIN0_SIGN=0#Delta Gain Sign 0;"
        "DELTA_GAIN1[6:0]=0#Delta Gain 1;"
        "DELTA_GAIN1_SIGN=0#Delta Gain Sign 1;"
        "DELTA_GAIN2[6:0]=0#Delta Gain 2;"
        "DELTA_GAIN2_SIGN=0#Delta Gain Sign 2;"
        "DELTA_GAIN3[6:0]=0#Delta Gain 3;"
        "DELTA_GAIN3_SIGN=0#Delta Gain Sign 3;"
        "DELTA_GAIN4[6:0]=0#Delta Gain 4;"
        "DELTA_GAIN4_SIGN=0#Delta Gain Sign 4;"
        "DELTA_GAIN_DT0[3:0]=0#Delta Time Code;"
        "DELTA_GAIN_DT1[3:0]=0#Delta Time Code;"
        "DELTA_GAIN_DT2[3:0]=0#Delta Time Code;"
        "DELTA_GAIN_DT3[3:0]=0#Delta Time Code;"
        "DELTA_GAIN_DT4[3:0]=0#Delta Time Code;"
        "DELTA_GAIN_START[3:0]=0#Control Start;"
        "DELTA_GAIN_BW[1:0]=0#Filter BW"
        ,"R8"
        );

/*!
*_register_ R9*::
*
*/
Register R9(
        "USER_DATA[119:0]=0#User Data"
        ,"R9"
        );


/*!
*_register_ R10*::
*
*/
Register R10(
        "REVERB_MON_DUR[8]=0#Reverbration Decay Monitoring window duration;"
        "TXCURR[6]=0#Current Adjustment;"
        "REVERB_PER_VAR_LIMIT[2]=0#Reverbration period Variation Limit;"
        "MON_WIN_START[12]=0#Monitoring window start;"
        "MON_WIN_STEP[2]=1#Monitoring window step;"
        "CARRIER_PER_AUTO_ENA=0#Automatic carrier period;"
        "NOISE_SUPP_ENA=0#Noise suppression enable;"
        "TOF_CALIB[6]=0#ToF calibration;"
        "END_OF_REVERB[2]=1#Reverbration debounce time;"
        "QF_SEL[2]=1#DSP filter Q factor selection;"
        "AUTO_QF_CTRL_ENA[1]=0#DSP filter auto Q factor control enable;"
        "AUTO_ECHO_DEB_CTRL_ENA[1]=0#Automatic echo debounce time control enable;"
        "IO_PUP_ENA[1]=1#Internal IO line pullup enable;"
        "IO_SLP_FAST[1]=0#IO Line Slope control;"
        "ADV_IO_ENA[1]=0#Advance IO line protocol enable;"
        "TREC1_THR_CTRL_ENA[1]=0#Threshold control enable;"
        "END_OF_REVERB_TOUT[6]=39#End of reverbration time-out;"
        "ADV_IO_IND_SFE[1]=1#Advanced IO indirect measurement skip first echo;"
        "IO_TRANS_DIAG_ENA[1]=1#IO Line Transducer diagnostic enable;"
        "END_OF_REVERB_THR[1]=0#End of reverbration Threshold;"
        "IO_ECHO_PULSE_ENA[1]=0#IO Line 99.2 us Echo duration;"
        "PARASITIC_PEAK_MAG[2]=0#peak of parasitic echo to suppress at the end of reverbrations;"
        "TX_RX_PER_ENA[1]=0#Index 2 format selection;"
        "WIDTH_PEAK_ENA[1]=0#Index 14 format selection"
        ,"R10"
        );


/*!
*_register_ R11*::
*
*/
Register R11(
        ""
        ,"R11"
        );

/*!
*_register_ R12*::
*
*/
Register R12(
        "MEAS_DATA[359:0]=0#Magnitude data"
        ,"R12"
        );


/*!
*_register_ R13*::
*
*/
Register R13(
        "MEAS_RES_SHR[17:0]=0#Measurement results"
        ,"R13"
        );


/*!
*_register_ R14A*::
*
*/
Register R14A(
        "MEAS_RES_LNG[27:0]=0#Measurement results"
        ,"R14A"
        );

/*!
*_register_ R14B*::
*
*/
Register R14B(
        "MEAS_RES_LNG[29:0]=0#Measurement results"
        ,"R14B"
        );

/*!
*_register_ R15*::
*
*/
Register R15(
        "CMD[7:0]=0#Command byte"
        ,"R15"
        );

Variable BAUDRATE(100,"BAUDRATE");
Variable DUT(1,"DUT","Chip under test",1,4);
Variable DTX(0,"DTX","Delta TX period",-128,127);
Variable DRX(0,"DRX","Delta RX period",-128,127);

/****************************************************************************
 *                              [2] APPLICATION
 ****************************************************************************/
TmcController::TmcController(QWidget *parent) :
    Applet(parent)
{
    ui =  new Ui::TmcController;
    ui->setupUi(this);
    setObjectName("NM");
    setWindowTitle(APPLET_TITLE);
}

void TmcController::setupCustomUi()
{
    //ui->sbREG9->addBits("R9[0:119]");
    ui->sbREG9->addRegister("R9",&R9);
    ui->sbREG9->reconstruct();

    // REG8 Graph
    ui->ptREG8->addGraph();
    ui->ptREG8->yAxis->setLabel("Gain");
    ui->ptREG8->xAxis->setLabel("Time [ms]");
    ui->ptREG8->yAxis->setRange(50, 110);           //Programmable receiver gain range

    // REG5 Graph
    ui->ptREG5->addGraph();
    ui->ptREG5->yAxis->setLabel("Threshold");
    ui->ptREG5->xAxis->setLabel("Time [ms]");
    ui->ptREG5->yAxis->setRange(0, 64);

    // REG6 Graph
    ui->ptREG6->addGraph();
    ui->ptREG6->yAxis->setLabel("Threshold");
    ui->ptREG6->xAxis->setLabel("Time [ms]");
    ui->ptREG6->yAxis->setRange(0, 64);

    // REG12 Graph
    ui->ptREG12->addGraph();
    //ui->ptREG12->yAxis->setLabel("y");
    ui->ptREG12->xAxis->setLabel(" Time [ms]");

    ui->leDTX->setPrecision(0);     //0 desetinnych mist
    ui->leDRX->setPrecision(0);     //0 desetinnych mist

    connect(this,SIGNAL(signal_RegisterChanged()),this,SLOT(slot_REG5_update()));
    connect(this,SIGNAL(signal_RegisterChanged()),this,SLOT(slot_REG6_update()));
    connect(this,SIGNAL(signal_RegisterChanged()),this,SLOT(slot_REG8_update()));
    connect(this,SIGNAL(signal_RegisterChanged()),this,SLOT(slot_REG12_update()));
}

bool TmcController::initialize()
{
    setupCustomUi();
    addDataNode(&DIO);
    addDataNode(&R0);
    addDataNode(&R1);
    addDataNode(&R2A);
    addDataNode(&R2B);
    addDataNode(&R3);
    addDataNode(&R4);
    addDataNode(&R5);
    addDataNode(&R6);
    addDataNode(&R7);
    addDataNode(&R8);
    addDataNode(&R9);
    addDataNode(&R10);
    addDataNode(&R11);
    addDataNode(&R12);
    addDataNode(&R13);
    addDataNode(&R14A);
    addDataNode(&R14B);
    addDataNode(&R15);
    addDataNode(&BAUDRATE);
    addDataNode(&DUT);
    addDataNode(&DTX);
    addDataNode(&DRX);
    /*  use autobind controls to registers*/
    slot_updateAllControls();
    return true;
}

void TmcController::resume()
{
    setBoardDefault();
}


/****************************************************************************
 *                              [3] FUNCTIONALITY
*****************************************************************************/
QVariant TmcController::FUNCTION_LIST()
{
    quint32 dut = DUT.value();

    FUNCTION("ReadREG0","read register R0")
    {
        bool result = readTmcRegister(dut,0,BAUDRATE.value(), &R0);
        slot_updateControls("R0");
        double temp_value = ((double)R0.value(QString("[0:7]")))*0.9915-75.225;
        int temp_int_value = temp_value;
        ui->lbR0_0_7_info->setText(QString("%1(deg. C)").arg(temp_int_value));
        return result;
    }


    FUNCTION("ReadREG1","read register R1")
    {
        bool result =  readTmcRegister(dut,1,BAUDRATE.value(), &R1);
        slot_updateControls("R1");
        return result;
    }


    FUNCTION("ReadREG2","read register R2")
    {

        int select = R10.value("[60]");
        if(select == 0) //2A
        {
            bool result = readTmcRegister(dut,2,BAUDRATE.value(), &R2A);
            slot_updateControls("R2A");
            return result;
        }

        else           //2B
        {
            bool result = readTmcRegister(dut,2,BAUDRATE.value(), &R2B);

            qint32 value1 = (qint32)R2B.value(QString("[0:6]"));   //DTX
            qint32 value2 = (qint32)R2B.value(QString("[8:14]"));  //DRX
            quint32 sign1 = R2B.value("[7]");
            quint32 sign2 = R2B.value("[15]");
            if(sign1 == 0)                                                  //+ DTX
            {
                ui->leDTX->setText(QString::number(value1));
            }
            else                                                            //- DTX
            {
                value1 = value1-128;                                        //2's complement conversion
                ui->leDTX->setText(QString::number(value1));
            }
            if(sign2 == 0)                                                  //+ DRX
            {
                ui->leDRX->setText(QString::number(value2));
            }
            else                                                            //- DRX
            {
                value2 = value2-128;                                        //2's complement conversion
                ui->leDRX->setText(QString::number(value2));
            }
            slot_updateControls("R2B");
            return result;
        }
    }

    FUNCTION("WriteREG2","Write register R2")
    {
        int select = R10.value("[60]");
        if(select == 0) //2A
        {
            return  writeTmcRegister(dut,2,BAUDRATE.value(),R2A);
        }

        else           //2B
        {
            return  writeTmcRegister(dut,2,BAUDRATE.value(),R2B);
        }
    }

    FUNCTION("ReadREG3","Reads R3")
    {
        bool result = readTmcRegister(dut,3,BAUDRATE.value(), &R3);
        slot_updateControls("R3");
        return result;
    }

    FUNCTION("WriteREG3","Writes R3")
    {
       return  writeTmcRegister(dut,3,BAUDRATE.value(),R3);
    }

    FUNCTION("ReadREG4","Reads R4")
    {
        bool result = readTmcRegister(dut,4,BAUDRATE.value(), &R4);
        slot_updateControls("R4");
        return result;
    }

    FUNCTION("WriteREG4","Writes R4")
    {
        return writeTmcRegister(dut,4,BAUDRATE.value(),R4);
    }


    FUNCTION("ReadREG5","Reads R5")
    {
        Register r(R5);
        bool result = readTmcRegister(dut,5,BAUDRATE.value(), &r);
        populateFromCustomReg(&R5,r);
        slot_updateControls("R5");
        emit signal_RegisterChanged();
        return result;
    }

    FUNCTION("WriteREG5","Writes R5")
    {
        Register r(R5);
        populateToCustomReg(&r,R5);
        return  writeTmcRegister(dut,5,BAUDRATE.value(),r);
    }

    FUNCTION("ReadREG6","Reads R6")
    {
        Register r(R6);
        bool result = readTmcRegister(dut,6,BAUDRATE.value(), &r);
        populateFromCustomReg(&R6,r);
        slot_updateControls("R6");
        emit signal_RegisterChanged();
        return result;
    }
    FUNCTION("WriteREG6","Writes R6")
    {
        Register r(R6);
        populateToCustomReg(&r,R6);
        return  writeTmcRegister(dut,6,BAUDRATE.value(),r);
    }

    FUNCTION("ReadREG7","Reads R7")
    {
        bool result = readTmcRegister(dut,7,BAUDRATE.value(), &R7);
        slot_updateControls("R7");
        return result;
    }

    FUNCTION("WriteREG7","Writes R7")
    {
        return writeTmcRegister(dut,7,BAUDRATE.value(),R7);
    }

    FUNCTION("ReadREG8","Reads R8")
    {
        bool result = readTmcRegister(dut,8,BAUDRATE.value(), &R8);
        slot_updateControls("R8");
        emit signal_RegisterChanged();
        return result;
    }

    FUNCTION("WriteREG8","Writes R8")
    {
       return writeTmcRegister(dut,8,BAUDRATE.value(),R8);
    }

    FUNCTION("ReadREG9","Reads R9")
    {
        bool result = readTmcRegister(dut,9,BAUDRATE.value(), &R9);
        slot_updateControls("R9");
        return result;
    }

    FUNCTION("WriteREG9","Writes R9")
    {
        return writeTmcRegister(dut,9,BAUDRATE.value(),R9);
    }

    FUNCTION("ReadREG10","Reads R10")
    {
        bool result = readTmcRegister(dut,10,BAUDRATE.value(), &R10);
        slot_updateControls("R10");
        return result;
    }

    FUNCTION("WriteREG10","Writes R10")
    {
        return writeTmcRegister(dut,10,BAUDRATE.value(),R10);
    }

    FUNCTION("ReadREG12","Reads R12")
    {
        bool result = readTmcRegister(dut,12,BAUDRATE.value(), &R12);
        slot_updateControls("R12");
        emit signal_RegisterChanged();
        return result;
    }

    FUNCTION("ReadREG13","Reads R13")
    {
        bool result = readTmcRegister(dut,13,BAUDRATE.value(), &R13);
        slot_updateControls("R13");
        return result;
    }

    FUNCTION("ReadREG14","Reads R14")
    {
        int select = R10.value("[61]");
        if(select == 0) //14A
        {
            bool result = readTmcRegister(dut,14,BAUDRATE.value(), &R14A);
            slot_updateControls("R14A");
            return result;
        }

        else           //14B
        {
            bool result = readTmcRegister(dut,14,BAUDRATE.value(), &R14B);
            slot_updateControls("R14B");
            return result;
        }
    }

    FUNCTION("ReadREG15","Reads R15")
    {
        bool result = readTmcRegister(dut,15,BAUDRATE.value(), &R15);
        slot_updateControls("R15");
        return result;
    }

    FUNCTION("WriteREG15","Writes R15")
    {
        return writeTmcRegister(dut,15,BAUDRATE.value(),R15);
    }

    FUNCTION("WriteAll","Write ALL registers")
    {
        DialogProgressBar dialog;
        dialog.show();
        runAppCommand(QString("WriteREG10"),QVariant());
        for(int i=0;i<15;i++)
        {
            // ignoring read-only and REG10 since it was written before
            if(i==0 || i==1 || i== 10 || i==11 || i==13 || i==14 ) continue;
            runAppCommand(QString("WriteREG%1").arg(i),QVariant());
            dialog.setProgress(i,15);
        }
        return true;
    }

    FUNCTION("ReadAll","reads all registers")
    {
        DialogProgressBar dialog;
        dialog.show();
        for(int i=0;i<15;i++)
        {
            if(i==11) continue;                     //changed - now possible to read all when ADV_IO_ENA = 1
            runAppCommand(QString("ReadREG%1").arg(i),QVariant());
            dialog.setProgress(i,15);
        }
        return true;
    }

    FUNCTION("WriteToAllDUTs","Write ALL registers to all 4 DUTs")
    {
        DialogProgressBar dialog;
        dialog.show();
        int k=0;
        double backupDUT = DUT.value();
        for(int m=0; m<4; m++)
        {
            DUT.setValue(m+1);
            runAppCommand(QString("WriteREG10"),QVariant());
            for(int i=0;i<15;i++)
            {
                k++;
                // ignoring read-only and REG10 since it was written before
                if(i==0 || i==1 || i== 10 || i==11 || i==13 || i==14 ) continue;
                runAppCommand(QString("WriteREG%1").arg(i),QVariant());
                dialog.setProgress(k,60);
            }
        }
        DUT.setValue(backupDUT);
        return true;
    }

    FUNCTION("StoreEEPROM","Stores to EEPROM")
    {
        R15 = 0x29;
        writeTmcRegister(dut,15,BAUDRATE.value(), R15) ;
        R15 = 0xD6;
        writeTmcRegister(dut,15,BAUDRATE.value(), R15);
        return true;
    }

    FUNCTION("RefreshEEPROM","Refresh EEPROM")
    {
        R15 = 0x29;
        writeTmcRegister(dut,15,BAUDRATE.value(), R15) ;
        R15 = 0x73;
        writeTmcRegister(dut,15,BAUDRATE.value(), R15);
        return true;
    }

    FUNCTION("ApplyTPENA","Applies TPENA")
    {
        if(arg_data.isNull())
        {
            R15 = 0xA0 | ui->cmTPENA->currentIndex();
        }
        else
        {
            R15 = 0xA0 | arg_data.toUInt();
        }
        writeTmcRegister(dut,15,BAUDRATE.value(), R15);
        return true;
    }

    return false;
}

void TmcController::populateFromCustomReg(Register *to, Register &from)     //reading
{
    to->setValue("[95:72]", from.value("[95:72]"));
    to->setValue("[119:96]", from.value("[119:96]"));

    to->setValue("[5:0]", from.value("[5:0]"));             //LVL0-3
    to->setValue("[11:6]", from.value("[13:8]"));
    to->setValue("[17:12]", from.value("[21:16]"));
    to->setValue("[23:18]", (from.value("[7:6]"))*1 + (from.value("[15:14]"))*4 + (from.value("[23:22]"))*16);

    to->setValue("[29:24]", from.value("[29:24]"));             //LVL4-7
    to->setValue("[35:30]", from.value("[37:32]"));
    to->setValue("[41:36]", from.value("[45:40]"));
    to->setValue("[47:42]", (from.value("[31:30]"))*1 + (from.value("[39:38]"))*4 + (from.value("[47:46]"))*16);

    to->setValue("[53:48]", from.value("[53:48]"));             //LVL4-7
    to->setValue("[59:54]", from.value("[61:56]"));
    to->setValue("[65:60]", from.value("[69:64]"));
    to->setValue("[71:66]", (from.value("[55:54]"))*1 + (from.value("[63:62]"))*4 + (from.value("[71:70]"))*16);
}


void TmcController::populateToCustomReg(Register *to, Register &from)     //writing
{
    to->setValue("[95:72]", from.value("[95:72]"));         //DT0-5
    to->setValue("[119:96]", from.value("[119:96]"));       //DT6-11

    to->setValue("[5:0]", from.value("[5:0]"));             //LVL0-3
    to->setValue("[13:8]", from.value("[11:6]"));
    to->setValue("[21:16]", from.value("[17:12]"));
    to->setValue("[7:6]", from.value("[19:18]"));
    to->setValue("[15:14]", from.value("[21:20]"));
    to->setValue("[23:22]", from.value("[23:22]"));

    to->setValue("[29:24]", from.value("[29:24]"));             //LVL4-7
    to->setValue("[37:32]", from.value("[35:30]"));
    to->setValue("[45:40]", from.value("[41:36]"));
    to->setValue("[31:30]", from.value("[43:42]"));
    to->setValue("[39:38]", from.value("[45:44]"));
    to->setValue("[47:46]", from.value("[47:46]"));

    to->setValue("[53:48]", from.value("[53:48]"));             //LVL4-7
    to->setValue("[61:56]", from.value("[59:54]"));
    to->setValue("[69:64]", from.value("[65:60]"));
    to->setValue("[55:54]", from.value("[67:66]"));
    to->setValue("[63:62]", from.value("[69:68]"));
    to->setValue("[71:70]", from.value("[71:70]"));

}


const double THR_DT[] = { 100   ,200  ,300  ,400  ,600 ,800 ,1000   ,1200
                          ,1600  ,2000 ,2400 ,3200 ,4000,5200,6400    ,8000};

void TmcController::slot_REG6_update()
{
    ui->leTHR2_DT0->setText(QString::number(THR_DT[R6.value("[DT0]")]/1000,'f',3)%"ms");
    ui->leTHR2_DT1->setText(QString::number(THR_DT[R6.value("[DT1]")]/1000,'f',3)%"ms");
    ui->leTHR2_DT2->setText(QString::number(THR_DT[R6.value("[DT2]")]/1000,'f',3)%"ms");
    ui->leTHR2_DT3->setText(QString::number(THR_DT[R6.value("[DT3]")]/1000,'f',3)%"ms");
    ui->leTHR2_DT4->setText(QString::number(THR_DT[R6.value("[DT4]")]/1000,'f',3)%"ms");
    ui->leTHR2_DT5->setText(QString::number(THR_DT[R6.value("[DT5]")]/1000,'f',3)%"ms");
    ui->leTHR2_DT6->setText(QString::number(THR_DT[R6.value("[DT6]")]/1000,'f',3)%"ms");
    ui->leTHR2_DT7->setText(QString::number(THR_DT[R6.value("[DT7]")]/1000,'f',3)%"ms");
    ui->leTHR2_DT8->setText(QString::number(THR_DT[R6.value("[DT8]")]/1000,'f',3)%"ms");
    ui->leTHR2_DT9->setText(QString::number(THR_DT[R6.value("[DT9]")]/1000,'f',3)%"ms");
    ui->leTHR2_DT10->setText(QString::number(THR_DT[R6.value("[DT10]")]/1000,'f',3)%"ms");
    ui->leTHR2_DT11->setText(QString::number(THR_DT[R6.value("[DT11]")]/1000,'f',3)%"ms");

    QVector<double> x,y;
    double delta =0.0f;
    for(qint32 i=0;i<12;i++)
    {
        y.append((double)R6.value(QString("[LVL%1]").arg(i)));
        x.append(delta);
        qint32 value = R6.value(QString("[DT%1]").arg(i));
        delta += (double)THR_DT[value]/1000.0f;
    }
    //Interpolate to Max. meas. time [reg 4]
    double end_time = (double)R4.value(QString("[0:3]"))*6;
    x.append(end_time);
    y.append((double)R6.value(QString("[LVL11]")));

    ui->ptREG6->graph(0)->setData(x,y);
    ui->ptREG6->xAxis->setRange(0,end_time);
    ui->ptREG6->replot();
}

void TmcController::slot_REG5_update()
{
    ui->leTHR1_DT0->setText(QString::number(THR_DT[R5.value("[DT0]")]/1000,'f',3)%"ms");
    ui->leTHR1_DT1->setText(QString::number(THR_DT[R5.value("[DT1]")]/1000,'f',3)%"ms");
    ui->leTHR1_DT2->setText(QString::number(THR_DT[R5.value("[DT2]")]/1000,'f',3)%"ms");
    ui->leTHR1_DT3->setText(QString::number(THR_DT[R5.value("[DT3]")]/1000,'f',3)%"ms");
    ui->leTHR1_DT4->setText(QString::number(THR_DT[R5.value("[DT4]")]/1000,'f',3)%"ms");
    ui->leTHR1_DT5->setText(QString::number(THR_DT[R5.value("[DT5]")]/1000,'f',3)%"ms");
    ui->leTHR1_DT6->setText(QString::number(THR_DT[R5.value("[DT6]")]/1000,'f',3)%"ms");
    ui->leTHR1_DT7->setText(QString::number(THR_DT[R5.value("[DT7]")]/1000,'f',3)%"ms");
    ui->leTHR1_DT8->setText(QString::number(THR_DT[R5.value("[DT8]")]/1000,'f',3)%"ms");
    ui->leTHR1_DT9->setText(QString::number(THR_DT[R5.value("[DT9]")]/1000,'f',3)%"ms");
    ui->leTHR1_DT10->setText(QString::number(THR_DT[R5.value("[DT10]")]/1000,'f',3)%"ms");
    ui->leTHR1_DT11->setText(QString::number(THR_DT[R5.value("[DT11]")]/1000,'f',3)%"ms");

    QVector<double> x,y;
    double delta =0.0f;
    for(qint32 i=0;i<12;i++)
    {
        y.append((double)R5.value(QString("[LVL%1]").arg(i)));
        x.append(delta);
        qint32 value = R5.value(QString("[DT%1]").arg(i));
        delta += (double)THR_DT[value]/1000.0f;
    }
    //Interpolate to Max. meas. time [reg 4]
    double end_time = (double)R4.value(QString("[0:3]"))*6;
    x.append(end_time);
    y.append((double)R5.value(QString("[LVL11]")));

    ui->ptREG5->graph(0)->setData(x,y);
    ui->ptREG5->xAxis->setRange(0,end_time);
    ui->ptREG5->replot();


}



const double DELTA_GAIN_DT[] = { 102.4   ,204.8  ,409.6  ,819.2  ,1228.8 ,1638.4 ,2048   ,2457.6
                                 ,3276.8  ,4505.6 ,5939.2 ,7987.2 ,10035.2,12697.6,15974.4    ,20070.4};


void TmcController::slot_REG8_update()
{
    ui->lbR8_40_43_info->setText(QString::number(DELTA_GAIN_DT[R8.value("[DELTA_GAIN_DT0]")]/1000,'f',3)%"ms");
    ui->lbR8_44_47_info->setText(QString::number(DELTA_GAIN_DT[R8.value("[DELTA_GAIN_DT1]")]/1000,'f',3)%"ms");
    ui->lbR8_48_51_info->setText(QString::number(DELTA_GAIN_DT[R8.value("[DELTA_GAIN_DT2]")]/1000,'f',3)%"ms");
    ui->lbR8_52_55_info->setText(QString::number(DELTA_GAIN_DT[R8.value("[DELTA_GAIN_DT3]")]/1000,'f',3)%"ms");
    ui->lbR8_56_59_info->setText(QString::number(DELTA_GAIN_DT[R8.value("[DELTA_GAIN_DT4]")]/1000,'f',3)%"ms");

    QVector<double> x,y;
    double delta =0.0f;
    int data_points = 0;                                                            //Number of data points

    //Starting point
    double start_gain = 50+((double)R7.value(QString("[0:6]")))*0.476;
    x.append(0);
    y.append(start_gain);                                                           //50dB min. gain + static gain
    data_points++;

    //Check if dyn. gain. start is set
    double dyn_gain_start_time = ((double)R8.value(QString("[60:63]"))*0.2048);
    if(dyn_gain_start_time != 0)
    {
        x.append(dyn_gain_start_time);
        y.append(start_gain);
        delta += dyn_gain_start_time;
        data_points++;
    }

    //Append rest of the data
    for(qint32 i=0;i<5;i++)
    {
        double last_gain = y.at(data_points-1);
        if(((double)R8.value(QString("[%1]").arg(((i+1)*7)+i)))==0)      //sign +
        {
            //Predchozi gain + soucasna delta
            y.append(last_gain+((double)R8.value(QString("[%1:%2]").arg(i*8).arg((i*8)+6)))*0.476);
        }

        else                                                                        //sign -
        {
            //Predchozi gain - soucasna delta
            y.append(last_gain-((double)R8.value(QString("[%1:%2]").arg(i*8).arg((i*8)+6)))*0.476);
        }
        //Predchozi time + soucasna delta
        qint32 value = R8.value(QString("[DELTA_GAIN_DT%1]").arg(i));
        delta += ((double)DELTA_GAIN_DT[value]/1000.0f);
        x.append(delta);
        data_points++;
    }

    //Interpolate to Max. meas. time [reg 4]
    double end_time = (double)R4.value(QString("[0:3]"))*6;
    x.append(end_time);
    y.append(y.at(data_points-1));

    ui->ptREG8->graph(0)->setData(x,y);
    ui->ptREG8->xAxis->setRange(0,end_time);
    ui->ptREG8->replot();

}



const double  MON_WIN_STEP[]  = {25.6,51.2,102.4,204.8};
void TmcController::slot_REG12_update()
{
    int j,i;
    QVector<double> x,y;
    double start = R10.value("[16:27]")*25.6f;                                      //MON_WIN_START*25.6
    double step = MON_WIN_STEP[R10.value("[MON_WIN_STEP]")]/1000.0f;                //us -> ms
    for(j=0,i=0; i<360;  i+=6,j++)
    {
        x.append((double)start+j*step);                                             //time step
        y.append((double)R12.value(QString("[%1:%2]").arg(i).arg(i+5))); //echo^
    }
    ui->ptREG12->graph(0)->setData(x,y);
    ui->ptREG12->xAxis->setRange(start,(start+step*60.0f));
    ui->ptREG12->yAxis->setRange(0, 64);
    ui->ptREG12->replot();
}

void TmcController::on_leR1_8_18_textChanged(const QString &arg1)
{
    double value = arg1.toDouble();
    ui->lbR1_8_18_info->setText(QString("%1(ns)   %2(kHz)")
                                .arg(((double)value*25))
                                .arg((double)1000000.0f/(value*25)));
}

void TmcController::on_leR2A_0_10_textChanged(const QString &arg1)
{
    double value = arg1.toDouble();
    ui->lbR2A_0_10_info->setText(QString("%1(ns)   %2(kHz)")
                           .arg((quint32)value*25)
                           .arg(1000000.0f/(value*25)));
    //refresh R2B
    on_leDTX_textChanged(ui->leDTX->text());
    on_leDRX_textChanged(ui->leDRX->text());
}

void TmcController::on_leDTX_textChanged(const QString &arg1)
{
   //int value = (arg1.toInt());                                      //-128,127
   int value = (ui->leDTX->text().toInt());
   QString write;
   quint32 regwrite;
   QString str_period = ui->leR2A_0_10->text();
   double base_period = str_period.toDouble();
   double TX_period;
   //int -> 2's complement
   if(value >= 0)   //MSB=0
   {
       write = QString::number(value);
       regwrite = value;
       R2B.setValue("[0:6]", regwrite);
       R2B.setValue("[7]", 0);
   }
   else             //MSB=1 -> conversion
   {
       write = QString::number(value+128);
       regwrite = value+128;
       R2B.setValue("[0:6]", regwrite);
       R2B.setValue("[7]", 1);
   }
   ui->lbR2B_0_7_info->setText(QString("%1(ns)").arg(value*50));
   TX_period = (base_period + 2* value)*25;
   ui->lb_R2TXPer->setText(QString("TX Period: %1(ns)  %2(kHz)")
                           .arg(TX_period)
                           .arg(1000000.0f/TX_period));
}

void TmcController::on_leDRX_textChanged(const QString &arg1)
{
   int value = (arg1.toInt());                                  //-128,127
   QString write;
   quint32 regwrite;
   QString str_period = ui->leR2A_0_10->text();
   double base_period = str_period.toDouble();
   double RX_period;
   //int -> 2's complement
   if(value >= 0)   //MSB=0
   {
       write = QString::number(value);
       regwrite = value;
       R2B.setValue("[8:14]", regwrite);
       R2B.setValue("[15]", 0);
   }
   else             //MSB=1 - conversion
   {
       write = QString::number(value+128);
       regwrite = value+128;
       R2B.setValue("[8:14]", regwrite);
       R2B.setValue("[15]", 1);
   }
   ui->lbR2B_8_15_info->setText(QString("%1(ns)").arg(value*50));
   RX_period = (base_period + 2* value)*25;
   ui->lb_R2RXPer->setText(QString("RX Period: %1(ns)  %2(kHz)")
                           .arg(RX_period)
                           .arg(1000000.0f/RX_period));
}


void TmcController::on_leR7_0_6_textChanged(const QString &arg1)
{
    double value = arg1.toDouble();
    ui->lbR7_0_6_info->setText(QString("%1(dB)")
                               .arg(value*0.476f));
}

void TmcController::on_leR8_60_63_textChanged(const QString &arg1)
{    double value = arg1.toDouble();
     ui->lbR8_60_63_info->setText(QString("%1(us)")
                                .arg(value*204.8f));
}

void TmcController::on_leR10_0_7_textChanged(const QString &arg1)
{
    double value = arg1.toDouble();
    ui->lbR10_0_7_info->setText(QString("%1(us)")
                                .arg(value*25.6f));
}

void TmcController::on_leR10_8_13_textChanged(const QString &arg1)
{
    double value = arg1.toDouble();
    value = value*4.762f+50;
    QString str_value = QString::number(value, 'f', 1);     //decimal point reduction
    ui->lbR10_8_13_info->setText(QString("%1(mA)")
                                .arg(str_value));
}

void TmcController::on_leR10_16_27_textChanged(const QString &arg1)
{
    double value = arg1.toDouble();
    ui->lbR10_16_27_info->setText(QString("%1(us)")
                                  .arg(value*25.6f));
}

void TmcController::on_leR10_32_37_textChanged(const QString &arg1)
{
    double value = arg1.toDouble();
    ui->lbR10_32_37_info->setText(QString("%1(us)")
                                  .arg(value*25.6f));
}

void TmcController::on_leR10_48_53_textChanged(const QString &arg1)
{
    double value = arg1.toDouble();
    ui->lbR10_48_53_info->setText(QString("%1(us)")
                                  .arg(value*51.2f));
}

void TmcController::on_leR13_8_17_textChanged(const QString &arg1)
{
    double value = arg1.toDouble()*51.2f
            - ui->leR10_32_37->text().toDouble()*25.6;
    ui->lbR13_8_17_info->setText(QString("%1(us)")
                                 .arg(value));
}

void TmcController::on_leR14A_8_17_textChanged(const QString &arg1)
{
    double value = arg1.toDouble()*51.2f
            - ui->leR10_32_37->text().toDouble()*25.6;
    ui->lbR14A_8_17_info->setText(QString("%1(us)")
                                 .arg(value));
}

void TmcController::on_leR14A_18_27_textChanged(const QString &arg1)
{
    double value = arg1.toDouble()*51.2f
            - ui->leR10_32_37->text().toDouble()*25.6;
    ui->lbR14A_18_27_info->setText(QString("%1(us)")
                                  .arg(value));
}

void TmcController::on_leR14B_8_17_textChanged(const QString &arg1)
{
    double value = arg1.toDouble()*51.2f
            - ui->leR10_32_37->text().toDouble()*25.6;
    ui->lbR14B_8_17_info->setText(QString("%1(us)")
                                   .arg(value));
}

void TmcController::on_leR14B_24_29_textChanged(const QString &arg1)
{
    double value = arg1.toDouble()*12.8f;
    ui->lbR14B_24_29_info->setText(QString("%1(us)")
                                  .arg(value));
}



void TmcController::on_pbExpandAll_clicked()
{

    ui->grREG0->setExpanded(true);
    ui->grREG1->setExpanded(true);
    ui->grREG2->setExpanded(true);
    ui->grREG3->setExpanded(true);
    ui->grREG4->setExpanded(true);
    ui->grREG5->setExpanded(true);
    ui->grREG6->setExpanded(true);
    ui->grREG7->setExpanded(true);
    ui->grREG8->setExpanded(true);
    ui->grREG9->setExpanded(true);
    ui->grREG10->setExpanded(true);
    ui->grREG12->setExpanded(true);
    ui->grREG13->setExpanded(true);
    ui->grREG14->setExpanded(true);
    ui->grREG15->setExpanded(true);
}

void TmcController::on_pbHideAll_clicked()
{
    ui->grREG0->setExpanded(false);
    ui->grREG1->setExpanded(false);
    ui->grREG2->setExpanded(false);
    ui->grREG3->setExpanded(false);
    ui->grREG4->setExpanded(false);
    ui->grREG5->setExpanded(false);
    ui->grREG6->setExpanded(false);
    ui->grREG7->setExpanded(false);
    ui->grREG8->setExpanded(false);
    ui->grREG9->setExpanded(false);
    ui->grREG10->setExpanded(false);
    ui->grREG12->setExpanded(false);
    ui->grREG13->setExpanded(false);
    ui->grREG14->setExpanded(false);
    ui->grREG15->setExpanded(false);
}

bool TmcController::writeTmcRegister(quint32 dut,quint32 index, quint32 baudrate, Register &reg)
{
    bool result;
    char tmp[50];
    QByteArray value;
    Register r(QString("operation=0;index[4]=%1;%2;checksum[8]")
               .arg(index).arg(reg.toString()));
    (*r.sub("payload")) = reg;
    quint32 checksum = calcTmcCheckSum(r);
    (*r.sub("checksum")) = checksum;

        switch(dut)
        {
        case 4:
            SPRINTF(tmp,"%s,%s,%04u,%04u,%04u,", PIN_IO_OUT_DUT4, PIN_IO_IN_DUT4, r.size(), 1, baudrate);
            break;
        case 3:
            SPRINTF(tmp,"%s,%s,%04u,%04u,%04u,", PIN_IO_OUT_DUT3, PIN_IO_IN_DUT3, r.size(), 1, baudrate);
            break;
        case 2:
            SPRINTF(tmp,"%s,%s,%04u,%04u,%04u,", PIN_IO_OUT_DUT2, PIN_IO_IN_DUT2, r.size(), 1, baudrate);
            break;
        case 1:
            SPRINTF(tmp,"%s,%s,%04u,%04u,%04u,", PIN_IO_OUT, PIN_IO_IN, r.size(), 1, baudrate);
            break;
        default:
            result = false;
            break;
        }

    result = BENCH()->board()->setData("DIOSEQ5", QByteArray(tmp)+r.toByteArray().toHex());
    if(result)
    {
        result = BENCH()->board()->getData("DIOSEQ5",&value);
        ///
    }

    result =value.toInt()==1;
    if(!result)
    {
        LOG(LOG_ERROR,QString("Register WRITE fail %1").arg(reg.name()));
    }
    return result;
}


bool TmcController::readTmcRegister(quint32 dut,quint32 index, quint32 baudrate, Register *preg)
{
    bool result;
    char tmp[50];
    Register r(QString("operation=1;index[4]=%1;%2;checksum[8]")
               .arg(index).arg(preg->toString()));
    QByteArray value;

    switch(dut)
    {
    case 4:
        SPRINTF(tmp,"%s,%s,%04u,%04u,%04u,", PIN_IO_OUT_DUT4, PIN_IO_IN_DUT4, 5, r.size()-5, baudrate);
        break;
    case 3:
        SPRINTF(tmp,"%s,%s,%04u,%04u,%04u,", PIN_IO_OUT_DUT3, PIN_IO_IN_DUT3, 5, r.size()-5, baudrate);
        break;
    case 2:
        SPRINTF(tmp,"%s,%s,%04u,%04u,%04u,", PIN_IO_OUT_DUT2, PIN_IO_IN_DUT2, 5, r.size()-5, baudrate);
        break;
    case 1:
        SPRINTF(tmp,"%s,%s,%04u,%04u,%04u,", PIN_IO_OUT, PIN_IO_IN, 5, r.size()-5, baudrate);
        break;
    default:
        result = false;
        break;
    }

    result = BENCH()->board()->setData("DIOSEQ5"
                                       ,QByteArray(tmp)+ r.sub(0,4)->toByteArray().toHex());
    if(result)
    {
         result = BENCH()->board()->getData("DIOSEQ5",&value);
         r.sub(5,r.size())->fromByteArray( QByteArray::fromHex(value),preg->size()+8 );
         quint32 checksum = 0xff &calcTmcCheckSum( r);

         if(checksum == r.value("[checksum]"))
         {
             *preg = *r.sub(5,r.size()-9);
             return true;
         }
         else
         {
             LOG(LOG_ERROR,QString("Register READ fail %1").arg(preg->name()));
         }
    }
    else
    {
         LOG(LOG_ERROR,QString("Register READ fail %1").arg(preg->name()));
    }



    return false;
}



quint32 TmcController::calcTmcCheckSum(Register &reg)
{

    quint32 result =
            (quint32)reg.value(0)<<7
                                   |(quint32)reg.value(4)<<6
                                   |(quint32)reg.value(3)<<5
                                   |(quint32)reg.value(2)<<4
                                   |(quint32)reg.value(1)<<3;
    int lsb =5;

    int msb =5;
    do{
        if( (lsb+8) <= (reg.size() - 9) )
        {
            msb =  lsb+7;
        }
        else
        {
            msb = (reg.size() - 9);
        }
        result += reg.value(lsb, msb);
        if(result >0xff)
        {
            result += 1;
            result &= 0xff;
        }
        lsb+=8;
    }while(lsb<(reg.size() - 9));
    result = (0xff) & (0xff^result);
    return result;
}


//{I}  0x98   0x80   =118=>19=>E6
//{II} 0x98
//     11100  00001  11101010

void TmcController::setBoardDefault()
{
    updateHardwareAll();
}

void TmcController::updateHardwareAll()
{
    updateHardware(HARDWARE_SET,"*");
    slot_updateAllControls();
}

void TmcController::setupBoard()
{
    updateHardwareAll();
}

bool TmcController::updateHardware(Direction direction, const QString &item)
{
    bool result = false;
    if(direction== HARDWARE_SET)
    {
        if(item=="*")
        {
            static quint32 prev_val;
            //if(prev_val != DIO.sub("dir","OUT")->toUInt())
                result = MCU.transferData("set " + DIO.sub("dir","OUT")->toString("@pin=@value ").toLatin1());
            prev_val = DIO.sub("dir","OUT")->toUInt();
        }
    }
    return result;
}
