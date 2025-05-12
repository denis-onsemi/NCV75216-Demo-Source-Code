#ifndef TRIANGULATION_H
#define TRIANGULATION_H

#include <QWidget>
#include "Applet.h"
#include <QGraphicsScene>


namespace Ui {
class Triangulation;
}

class Triangulation : public Applet
{
    Q_OBJECT

public:
    explicit Triangulation(QWidget *parent = 0);
    ~Triangulation();
    virtual bool initialize();
    virtual QVariant runAppCommand(const QString &name, const QVariant &data= QVariant());
    void IsConstructable(double r0, double r1, double r2);
    void setupCustomUI();
    void SaveObstacles();
    void plotObstacles();
    void saveSettings(double S2X, double S2Y, double S3X, double S3Y, double SOS, quint8 proj_sel, bool AdAxRg, int MemTol, int MM_thr, int alg_tol, int prus_tol);


private slots:
    void on_cbAlgChoice_currentIndexChanged(int index);

private:
    Ui::Triangulation *ui;
};

extern Triangulation *gp_Triangulation;

#endif // TRIANGULATION_H



