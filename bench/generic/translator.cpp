#include "translator.h"
#include <QDebug>
#include <QStringList>



Translator::Translator()
{
}


/****************************************************************************
* @function name: CONSTRUCTOR
*
* @param:
*           const QByteArray &data
* @description:
* @return: ( void )
****************************************************************************/
Translator::Translator(const QString &data)
{
    populateTable(data);
}


/****************************************************************************
* @function name: populateTable()
*
* @param:
*           const QByteArray &script
* @description:
*----------------------------------
*  FORMAT1:"tKEY:tVALUE"
*           where tKEY is a key and
*       t -is a type
*           s- string
*           i-integer
*           f-real
*   example: sPI:f3.14;sE:f2.17
*   example: sINDEX1:i0;sINDEX2:i1
*----------------------------------
*  FORMAT2:"=formula(x)"
*
*   example: =2*x
*----------------------------------
* @return: ( void )
****************************************************************************/
void Translator::populateTable(const QString &script)
{     
    if(script.at(0)=='=')
    {
        m_formula = script.mid(1,script.count());
    }
    else
    {
        m_key_value.clear();
        foreach(const QString &item, script.split(';'))
        {
            QStringList l2 = item.split("::");
            if (l2.size() == 2 )
            {
                const int KEY_INDEX=0,VALUE_INDEX=1;
                Variable key = Variable::fromString(l2.at(KEY_INDEX));
                Variable value = Variable::fromString(l2.at(VALUE_INDEX));
                if(!key.isEmpty() && !value.isEmpty())
                {
                    QString str_key = QString("K%1").arg(Variable::toString(key));
                    QString str_value =  QString("V%1").arg(Variable::toString(value));
                    m_key_value[str_key] = str_value;
                    m_key_value[str_value] = str_key;
                }
            }
        }
    }


}
/****************************************************************************
* @function name: getValue()
*
* @param:
*           qint32 index
* @description:
* @return: ( double )
****************************************************************************/
const Variable Translator::get(Translator::Side side, const Variable &item)
{
    Variable result;
        QString item_str = QString("%1%2")
                .arg(side==KEY?'V':'K')
                .arg(Variable::toString(item));
        if ( m_key_value.contains(item_str) )
        {
            result= Variable::fromString(QString(m_key_value[item_str]).remove(0,1));
        }
    return result;
}

const QStringList Translator::items(Side side)
{
    QStringList result;
    foreach(const QString &item,m_key_value)
    {
        if(item.count())
        {
            if(item.at(0)=='K' && side == KEY)
                result.append(QString(item).remove(0,2));
            else if(item.at(0)=='V' && side == VALUE)
                result.append(QString(item).remove(0,2));
        }
    }
    return result;
}

