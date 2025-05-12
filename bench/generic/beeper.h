#ifndef BEEPER_H
#define BEEPER_H

#include <QThread>
#include <QList>
#include <QPair>

class Beeper : public QThread
{
    Q_OBJECT
public:        
    explicit Beeper(const QString &melody);
    ~Beeper();

protected:
    void run();

private:
    struct Sound{
     int freq;
     int time;
    };
    QList<Sound> m_melody;
    
};

#endif // BEEPER_H
