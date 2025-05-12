#ifndef TMCCONTROLLER_H
#define TMCCONTROLLER_H

#include "Applet.h"
#include "board_def.h"

namespace Ui
{
class TmcController;
}
    extern Variable DUT;    //propojeni s Measurement
    extern Register R4;     //propojeni s Measurement

class TmcController : public Applet
{
    Q_OBJECT
private:
    Ui::TmcController *ui;
    void setupCustomUi();
    void setupBoard();
    quint32 calcTmcCheckSum(Register &data);

public:
    explicit TmcController(QWidget *parent = 0 );
    ~TmcController(){}
    virtual bool initialize();
    virtual QVariant runAppCommand(const QString &name, const QVariant &data);

    bool writeTmcRegister(quint32 dut, quint32 index, quint32 baudrate, Register &reg);
    bool readTmcRegister(quint32 dut, quint32 index,quint32 baudrate, Register *preg);

    void populateFromCustomReg(Register *to, Register &from);
    void populateToCustomReg(Register *to,Register &from);

    void setBoardDefault();
    void updateHardwareAll();

protected:
    virtual void resume();
    virtual bool updateHardware(Direction direction,const QString &item) ;

private slots:
    void slot_REG6_update();
    void slot_REG5_update();
    void slot_REG8_update();
    void slot_REG12_update();
    void on_leR1_8_18_textChanged(const QString &arg1);
    void on_leR2A_0_10_textChanged(const QString &arg1);
    //void on_leR2B_0_6_textChanged(const QString &arg1);
    //void on_leR2B_8_14_textChanged(const QString &arg1);
    void on_leR7_0_6_textChanged(const QString &arg1);
    void on_leR8_60_63_textChanged(const QString &arg1);
    void on_leR10_0_7_textChanged(const QString &arg1);
    void on_leR10_8_13_textChanged(const QString &arg1);
    void on_leR10_48_53_textChanged(const QString &arg1);
    void on_leR10_32_37_textChanged(const QString &arg1);
    void on_leR10_16_27_textChanged(const QString &arg1);
    void on_pbExpandAll_clicked();
    void on_pbHideAll_clicked();
    void on_leR14A_18_27_textChanged(const QString &arg1);
    void on_leR14A_8_17_textChanged(const QString &arg1);
    void on_leR14B_8_17_textChanged(const QString &arg1);
    void on_leR14B_24_29_textChanged(const QString &arg1);
    void on_leR13_8_17_textChanged(const QString &arg1);
    void on_leDTX_textChanged(const QString &arg1);
    void on_leDRX_textChanged(const QString &arg1);
};
extern TmcController *gp_TmcController;
#endif // TMCCONTROLLER_H


