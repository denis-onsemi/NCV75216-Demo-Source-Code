#ifndef DSCANCHAIN_H
#define DSCANCHAIN_H

#include "applet.h"


namespace Ui {
class Scanchain;
}

class Scanchain : public Applet
{
    Q_OBJECT
public:
    explicit Scanchain(QWidget *parent = 0 );
    ~Scanchain(){}
    virtual bool initialize();    
    virtual QVariant runAppCommand(const QString &name, const QVariant &data= QVariant());
    virtual void resume();
    virtual const QByteArray saveAppData();
    virtual bool loadAppData(const QByteArray &data);

    void rebuild();
    void setScanchain(Register *preg);

    bool setConfig(const QString &name);
    bool addConfig(const QString &name, Register *preg);
    bool containsConfig(const QString &name);
    QString currentConfig() const;

    Register *scanchain(int num=-1) const;
    qint32 size() const;
    int chainsCount() const {return m_scanchains;}
    void setModesList(const QStringList &modes_list_before, const QStringList &modes_list_after);

    bool SetModeBefore(const QString &mode_name);
    bool SetModeAfter(const QString &mode_name);

private slots:    
    void on_pbMs_clicked();
    void on_pbMr_clicked();
    void slot_LayoutMode_toggled();    
    void on_cmFilter_currentIndexChanged(int);
    void slot_highlight(const QString &text);

    void on_pbFill0_clicked();
    void on_pbXor_clicked();
    void on_pbXor_toggled(bool checked);
    void on_cmCONFIG_activated(const QString &arg1);

    void on_pbSaveBack_clicked();

private:
    int m_scanchains;
    Ui::Scanchain *ui;
    void setupCustomUi();
    void updateStatistics();
    void inflateWithBits(const int num);
    CustomControls::SuperBox *box(int num);
};
extern Scanchain *gp_Scanchain;
#define SCANCHAIN gp_Scanchain

#endif // DIGITALSCANCHAIN_H


