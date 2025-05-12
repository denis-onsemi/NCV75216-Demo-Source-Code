/**
 * @file:variable.h   -
 * @description: This is class for variable
 * @project: BENCH OnSemiconductor
 * @date: 2014\02\04 
 *
 */
#ifndef VARIABLE_H
#define VARIABLE_H

#include <QObject>
#include <QHash>
#include <QStringList>
#include <QString>


class Variable;
class Translator;

#include "compiler.h"
#include "translator.h"

class Variable:public QObject
{
    Q_OBJECT
public:    
    Variable();
    Variable(const Variable &var);
    /* variable type when NONE(default) is empty*/
    typedef enum {NONE,INTEGER,REAL,ARRAY} Type;
    /* makes variable from script. */
    Variable(const QString &init_script);
    /* double constructor for variable class*/
    Variable(double value, const QString &name
            ,const QString &description = QString()
            ,double min = -DBL_MAX,double max = DBL_MAX);
    /* integer constructor v- is value */
    Variable(int value, const QString &name
            , const QString &description = QString()
            , double min = INT_MIN, double max = INT_MAX);
    /* byte array constructor*/
//    Variable(const QByteArray &value, const QString &name = QString()
//            ,const QString &description = QString());

    //~Variable() {destructor();}

    void init();
    void wipe();

    void makeSetup(const QString &init_script);
    const QString getSetup();

    Variable &operator = ( const double &var);
    Variable &operator = ( const int &var);
    Variable &operator = ( const Variable &var);
    Variable &operator = ( const QByteArray &var);

    Variable &operator + (const double &var);
    Variable &operator - (const double &var);
    Variable &operator * (const double &var);
    Variable &operator / (const double &var);
    Variable &operator + (const Variable &var);
    Variable &operator - (const Variable &var);
    Variable &operator * (const Variable &var);
    Variable &operator / (const Variable &var);

    bool operator > (const double &var);
    bool operator < (const double &var);
    bool operator == (const Variable &var);
    bool operator == ( const double &val);
    bool operator != ( const Variable &var);
    bool operator != ( const double &val);

    /*sets limit of variable from min to max*/
    void setLimit(double min,double max);
    /* sets name of variable */
    void setName(const QString &name);
    /* name of variable */
    const QString & name() const ;
    /* value setter for variable*/
    bool setValue(qint32 value,quint32 ring=RING_INTERNAL);
    /* value setter for variable*/
    bool setValue(double value,quint32 ring=RING_INTERNAL);
    /* value setter for variable*/
    bool setValue(const QByteArray &value,quint32 ring=RING_INTERNAL);
    /* value getter for variable */
    double value() const;
    /* value getter for array*/
    const QByteArray &array() const;
    /* variable type INTEGER REAL ARRAY*/
    const Type type() const;
    /* set Variable type*/
    void setType(const Type &type);
    /* returns description*/
    const QString &description() const;
    /* set description . used in help*/
    void setDescription(const QString &text);
    /* set readonly */
    void setReadOnly(bool on);
    /* is readonly */
    bool readOnly() const;    
    /* maximum value double*/
    double maximum() const;
    /* minimum value double*/
    double minimum() const;
    /* value is not set yet*/
    bool isEmpty() const;
    /* static converting function to string */
    static const QString toString(const Variable &var);
    /* from string i=integer f-real s-string first character*/
    static const Variable fromString(const QString &str);

    /*[Translator]*/
    /* sets translator */
    void setTranslator(Translator *ptr);
    /* returns translator pointer*/
    const Translator *translator() const;
    /* if translator installed*/
    bool hasTranslator() const;
    /* sets value by key*/
    bool setValueByKey(const Variable &key);
    /* sets value by key in string format(ByteArray)*/
    bool setValueByKeyString(const QByteArray &key_str);
    /* sets value by key in string format*/
    bool setValueByKeyString(const QString &key_str);
    /* returns value key*/
    const Variable valueKey();
    /* returns value key as string */
    const QString valueKeyString();

    /*[extra parameter]*/
    /* returns name of bit */
    QString extra(const QString &name);
    /* sets name of bit */
    void setExtra(const QString &name,const QString &value);
    QStringList extras();
    void bind(QObject *pobj, const char*Set, const char *Get);

    typedef enum {UpdateNever, UpdateAlways,UpdateOnChange} UpdatePolicy;
    void setUpdatePolicy(UpdatePolicy upd_pol);

signals:
    void signal_updateSet(const QString &regname);
    void signal_updateGet(const QString &regname);

private :
    /* */
    UpdatePolicy m_update_policy;
    /* */
    Translator *mp_tr;
    /* */
    QString m_setup_script;
    /**/
    QString m_description;
    /**/
    QString m_name;
    /**/
    QByteArray m_bytearray;
    double m_value;
    double m_min;
    double m_max;
    bool m_read_only;
    Type m_type;
    QHash <QString, QString> m_extra;

};

/* debug setup*/
#include <QDebug>
inline QDebug operator<< (QDebug d, const Variable &var)
{
    d << QString("Variable(name=%1 %2)").arg(var.name()).arg(Variable::toString(var));
    return d;
}

Q_DECLARE_METATYPE(Variable)



#endif // VARIABLE_H
