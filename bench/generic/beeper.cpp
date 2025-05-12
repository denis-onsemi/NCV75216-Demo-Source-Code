#include "beeper.h"
#include "Windows.h"
#include <QStringList>

Beeper::Beeper(const QString &melody) :
    QThread(0)
{
    bool melody_ok= false;
    QStringList l = melody.split(';');
    QStringList::const_iterator i = l.constBegin();
    while( i != l.constEnd() )
    {
        QStringList l2 = (*i).split(':');
        if(l2.size()==2)
        {
            bool ok1,ok2;
            Sound sound;
            sound.freq = l2.at(0).toInt(&ok1);
            sound.time =  l2.at(1).toInt(&ok2);
            m_melody.append(sound);
            melody_ok = ok1 &ok2;
        }
        else {
            melody_ok = false;
            break;
        }
        ++i;
    }
    if(melody_ok)
    {

        this->start(QThread::LowPriority);
    }
}

Beeper::~Beeper(){
    terminate();
}

void Beeper::run()
{
    QList<Sound>::const_iterator i = m_melody.constBegin() ;
    for(;i != m_melody.constEnd();)
    {
        Beep((*i).freq, (*i).time);
        ++i;
    }
    exec();
    delete (this);
}
