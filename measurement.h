#ifndef MEASUREMENT_H
#define MEASUREMENT_H

#include "Applet.h"

namespace Ui 
{
class Measurement;
}
extern double received_data[3][3][2];    //propojeni s triangulaci

class Measurement : public Applet 
{
    Q_OBJECT
private:
    Ui::Measurement *ui;

public:
    explicit Measurement(QWidget *parent = 0 );
    ~Measurement(){}    
    virtual bool initialize();
    virtual QVariant runAppCommand(const QString &name, const QVariant &data= QVariant());

private slots:
    void slot_getdata_delay();
};

extern Measurement *gp_Measurement;

#endif // MEASUREMENT_H


