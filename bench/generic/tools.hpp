#ifndef TOOLS_HPP
#define TOOLS_HPP
#include <QString>
#include <QByteArray>

static QString formatHexString(const QString &text)
{
    int count=text.count()/2+1;
    QString result;
    foreach(const QChar &c, text){
        if(count%3 ==0 && count !=0 ) result+=" ";
        result+=c;
    }
}

#endif // TOOLS_HPP
