#include "parameterparser.h"

ParameterParser::ParameterParser(const QString &text, const QString &line, const QString &equal)
{
    par_equal = equal;
    line_end = line;
    parse(text);
}
int ParameterParser::parse(const QString &text)
{
    int result=0;
    m_map.clear();    
    m_current=text;
    QStringList lines=text.split(line_end);
    foreach(const QString line, lines){
        QStringList items=line.trimmed().split(par_equal);
        if(items.count()==2){
            result++;
            m_map[items.at(0).trimmed()] = items.at(1).trimmed();
        }
    }
    return result;
}

ParameterParser &ParameterParser::operator [](const QString &key)
{
    if(m_map.contains(key))
        m_current = m_map[key];
    return *this;
}

int ParameterParser::toInt(int base)
{
    if(!m_current.isEmpty())
    {
        if(m_current.startsWith("0x")){
            return m_current.mid(2).toInt(0,16);
        }
        if(m_current.startsWith("0b")){
            return m_current.mid(2).toInt(0,2);
        }
        return m_current.toInt(0,base);
    }
    return 0;
}

bool ParameterParser::toBool()
{
    if(!m_current.isEmpty())
    {
        if(m_current[0].isNumber())
            return static_cast<bool>(m_current.toInt());
        else if(m_current.toUpper()=="TRUE")
            return true;
        else if(m_current.toUpper()=="FALSE")
            return false;
    }
    return false;
}

double ParameterParser::toDouble()
{
    return m_current.toDouble();
}


QByteArray ParameterParser::toByteArray()
{
    return QByteArray::fromHex(m_current.replace(" ","").toLatin1());
}


QString ParameterParser::toString()
{
    return m_current;
}

QStringList ParameterParser::keys() const
{
    return m_map.keys();
}

bool ParameterParser::contains(const QString &name)
{
    return m_map.contains(name);
}

bool ParameterParser::get(const QString &name, bool *p_bool)
{
    if(m_map.contains(name))
    {
        m_current = m_map[name];
        if(p_bool) *p_bool = toBool();
        return true;
    }
    return false;
}

bool ParameterParser::get(const QString &name, int *p_int)
{
    if(m_map.contains(name))
    {
        m_current = m_map[name];
        if(p_int) *p_int = toInt();
        return true;
    }
    return false;
}


bool ParameterParser::get(const QString &name, unsigned int *p_uint)
{
    if(m_map.contains(name))
    {

        m_current=m_map[name];
        if(p_uint) *p_uint = (unsigned int)toInt();
        return true;
    }
    return false;
}

bool ParameterParser::get(const QString &name, QString *p_str)
{
    if(m_map.contains(name))
    {
        m_current=m_map[name];
        if(p_str) *p_str = toString();
        return true;
    }
    return false;
}

bool ParameterParser::get(const QString &name, QByteArray *p_array)
{
    if(m_map.contains(name))
    {
        m_current=m_map[name];
        if(p_array) *p_array = toByteArray();
        return true;
    }
    return false;
}

bool ParameterParser::get(const QString &name, double *p_double)
{
    if(m_map.contains(name))
    {
        m_current=m_map[name];
        if(p_double) *p_double = toDouble();
        return true;
    }
    return false;
}
