#ifndef PARAMETERPARSER_H
#define PARAMETERPARSER_H

#include <QMap>
#include <QString>
#include <QStringList>



class ParameterParser
{
public:
    explicit ParameterParser(const QString &text,const QString &line=";",const QString &equal = "=");
    int parse(const QString &text);
    ParameterParser& operator [](const QString &key);

    bool contains(const QString &name);

    //int
    bool get(const QString &name, int *p_int);
    bool get(const QString &name, bool *p_bool);
    bool toBool();
    bool get(const QString &name, unsigned int *p_uint);
    int toInt(int base=10);
    //double
    bool get(const QString &name, double *p_double);
    double toDouble();
    //bytearray
    bool get(const QString &name, QByteArray *p_array);
    QByteArray toByteArray();
    //string
    bool get(const QString &name, QString *p_str);
    QString toString();
    QStringList keys()const;
private :
    QString m_current;
    QMap<QString,QString> m_map;
    QString line_end;
    QString par_equal;
};

#endif // PARAMETERPARSER_H

