/**
 * @file:variable.cpp   -
 * @description: This is class for variable
 * @project: BENCH OnSemiconductor
 * @date: 2014\02\04
 *
 */

#include "variable.h"


void Variable::init()
{
    m_value = (double)0;
    m_min = (double)-DBL_MAX;
    m_max = (double)DBL_MAX;
    m_name = "";
    m_type = Variable::NONE;
    m_read_only = false;
    mp_tr= NULL;
    m_update_policy = UpdateOnChange;
}

Variable::Variable()
{
    init();
}

Variable::Variable(const Variable &var)
{
    *this = var;
}

Variable::Variable(const QString &init_script)
{
    init();
    makeSetup(init_script);
}

/****************************************************************************
 * @function name: constructor
 *
 * @param:
 *
 *    double v - value of variable
 *    [const QString &name - name of variable]
 * @description:
 ****************************************************************************/
Variable::Variable(double value, const QString &name , const QString &description,double min,double max)
{    
    init();
    m_value = value;
    m_name = name;
    m_min = min;
    m_max =max;
    m_type = REAL;
    m_description = description;
}

/****************************************************************************
 * @function name: constructor
 * @param:
 *    int value
 *    const QString &name - name of variable]
 *    const QString &description
 *    double min
 *    double max
 * @description:
 ****************************************************************************/

Variable::Variable(int value, const QString &name , const QString &description, double min, double max)
{
    init();
    m_value = (double)value;
    m_min = (double)min;
    m_max = (double)max;
    m_name = name;
    m_type = INTEGER;
    m_description = description;
}

/****************************************************************************
 * @function name: constructor
 * @param:
 *    const QByteArray &value
 *    const QString &name
 *    const QString &description
 * @description:
 ****************************************************************************/
//Variable::Variable(const QByteArray &value, const QString &name, const QString &description)
//{
//    init();
//    m_bytearray = value;
//    m_type = ARRAY;
//    m_name = name;
//    m_value = 0.0f;
//    m_min = 0.0f;
//    m_max = 0.0f;
//    m_description = description;
//}





/****************************************************************************
 * @function name:  Variable::setLimit()
 *
 * @param:
 *          double min
 *          double max
 * @description: Sets name
  ****************************************************************************/
void Variable::setLimit(double min, double max)
{
    m_min = min;
    m_max = max;
}

/****************************************************************************
 * @function name:  Variable::setName()
 *
 * @param:
 *
 *          const QString &name
 * @description: Sets name
  ****************************************************************************/
void  Variable::setName(const QString &name)
{
    m_name = name;
}

/****************************************************************************
 * @function name:  Variable::name()
 *
 * @param:
 *
 * @description: gets name of variable
 ****************************************************************************/
const QString &  Variable::name() const
{    
    return m_name;
}

/****************************************************************************
 * @function name:  Variable::setValue()
 * @param:
 *       qint32 value
 * @description: Sets value by integer value
 * @return: bool success
 ****************************************************************************/
bool Variable::setValue(qint32 value, quint32 ring)
{
    if(m_type==NONE) m_type=INTEGER;
    bool result = setValue((double)value,ring);    
    return result;
}

/****************************************************************************
 * @function name:  Variable::setValue()
 * @param: 
 *       double v
 * @description: Sets value
 * @return: bool success
 ****************************************************************************/

bool Variable::setValue(double v, quint32 ring)
{
    bool upd=false;
    bool result = false;
    if(m_type==NONE) m_type=REAL;
    if(!readOnly() || ring == RING_INTERNAL)
    {
        if(m_min <= v && m_max >= v)
        {
            upd = (m_value != v);            
            m_value = v;
            result= true;
        }
    }
    switch(m_update_policy)
    {
        case UpdateNever:
        break;
        case UpdateOnChange:
            if(upd ) emit signal_updateSet(name());
        break;
        case UpdateAlways:
            emit signal_updateSet(name());
        break;
    }
    return result;
}

/****************************************************************************
 * @function name:  Variable::setValue()
 *
 * @param:
 *       const QByteArray &value
 *       quint32 ring
 * @description: Sets value
 * @return: bool success
 ****************************************************************************/
bool Variable::setValue(const QByteArray &value,quint32 ring)
{
    bool result = false;
     bool upd=false;
    if(m_type==NONE) m_type=ARRAY;
    if(!readOnly() || ring == RING_INTERNAL)
    {
        upd = (m_bytearray != value);
        m_bytearray = value;
        result= true;
    }
    switch(m_update_policy)
    {
        case UpdateNever:
        break;
        case UpdateOnChange:
            if(upd ) emit signal_updateSet(name());
        break;
        case UpdateAlways:
            emit signal_updateSet(name());
        break;
    }
    return result;
}

/****************************************************************************
 * @function name:  Variable::value()
 *
 * @param:
 *
 * @description: gets value as double
  ****************************************************************************/
double Variable::value() const
{    
    return m_value;
}

/****************************************************************************
 * @function name:  Variable::array()
 * @param:
 *       
 * @description: 
 * @return: const QByteArray 
 ****************************************************************************/
const QByteArray &Variable::array() const
{
    return m_bytearray;
}

/****************************************************************************
 * @function name:  Variable::type()
 * @param:
 *       
 * @description: 
 * @return: const Variable
 ****************************************************************************/
const Variable::Type Variable::type() const
{
    return m_type;
}

/****************************************************************************
 * @function name:  Variable::setType()
 * @param:
 *       const Variable::Type &type
 * @description: 
 * @return: void
 ****************************************************************************/
void Variable::setType(const Variable::Type &type)
{
    m_type = type;
}

/****************************************************************************
 * @function name:  Variable::description()
 * @param:
 *       const QString &text
 * @description: 
 * @return: const QString &
 ****************************************************************************/
const QString &Variable::description() const
{
    return m_description;
}

/****************************************************************************
 * @function name:  Variable::setDescription()
 * @param:
 *       const QString &text
 * @description: 
 * @return: void
 ****************************************************************************/
void Variable::setDescription(const QString &text)
{
    m_description = text;
}

/****************************************************************************
 * @function name:  Variable::setReadOnly()
 * @param:
 *       
 * @description: 
 * @return: void
 ****************************************************************************/
void Variable::setReadOnly(bool on)
{
    m_read_only = on;
}

/****************************************************************************
 * @function name:  Variable::readOnly()
 * @param:
 *       
 * @description: 
 * @return: bool
 ****************************************************************************/
bool Variable::readOnly() const
{
    return m_read_only;
}

/****************************************************************************
 * @function name:  Variable::maximum()
 * @param:
 *       
 * @description: 
 * @return: double
 ****************************************************************************/
double Variable::maximum() const
{
    return m_max;
}

/****************************************************************************
 * @function name:  Variable::minimum()
 * @param:
 *       
 * @description: 
 * @return: double
 ****************************************************************************/
double Variable::minimum() const
{
    return m_min;
}

/****************************************************************************
 * @function name:  Variable::isEmpty()
 * @param:
 *       
 * @description: 
 * @return: bool - true is empty
 ****************************************************************************/
bool Variable::isEmpty() const
{
    return type()== NONE;
}

/****************************************************************************
 * @function name:  Variable::toString()
 * @param:
 *       const Variable &var
 * @description: 
 * @return: const QString 
 ****************************************************************************/
const QString Variable::toString(const Variable &var)
{
    switch(var.type())
    {
    case Variable::ARRAY:
        return QString("s%1").arg(QString(var.array()));
        break;
    case Variable::INTEGER:
        return QString("i%1").arg((qint32)var.value());
        break;
    case Variable::REAL:
        return QString("f%1").arg(QString::number(var.value(),'E'));
        break;
    default:
        break;
    }
    return QString();
}

/****************************************************************************
 * @function name:  Variable::fromString()
 * @param:
 *       const QString &str)
 * @description: 
 * @return: const Variable
 ****************************************************************************/
const Variable Variable::fromString(const QString &str)
{
    Variable result;
    if(str.count()>1)
    {
        char type = str.at(0).toAscii();
        QString var_str = str;
        var_str.remove(type);
        switch(type)
        {
        case 'i':
            result.setType(Variable::INTEGER);
            result.setValue(var_str.toInt());
            break;
        case 'f':
            result.setType(Variable::REAL);
            result.setValue(var_str.toDouble());
            break;
        case 's':
            result.setType(Variable::ARRAY);
            result.setValue(var_str.toLatin1());
            break;
        default: break;
        }
    }
    return result;
}

/****************************************************************************
 * @function name:  Variable::setTranslator()
 * @param:
 *       Translator *ptr
 * @description: 
 * @return: void
 ****************************************************************************/
void Variable::setTranslator(Translator *ptr)
{
    if(mp_tr == NULL)
        mp_tr = ptr;
}

/****************************************************************************
 * @function name:  Variable::translator()
 * @param:
 *       
 * @description: 
 * @return: const Translator *
 ****************************************************************************/
const Translator *Variable::translator() const
{
    return mp_tr;
}

bool Variable::hasTranslator() const
{
    return mp_tr!=NULL;
}

/****************************************************************************
 * @function name:  Variable::setValueByKey()
 * @param:
 *       const Variable &key
 * @description: 
 * @return: bool
 ****************************************************************************/
bool Variable::setValueByKey(const Variable &key)
{
    if(mp_tr)
    {
        setValue(mp_tr->get(Translator::VALUE,key).value());
        return true;
    }
    return false;
}

/****************************************************************************
 * @function name:  Variable::setValueByKeyString()
 * @param:
 *       const QByteArray &key_str)
 * @description: 
 * @return: bool
 ****************************************************************************/
bool Variable::setValueByKeyString(const QByteArray &key_str)
{
    Variable v;
    v.setValue(key_str);
    return Variable::setValueByKey(v);
}

/****************************************************************************
 * @function name:  Variable::setValueByKeyString()
 * @param:
 *       const QString &key_str
 * @description: 
 * @return: bool
 ****************************************************************************/
bool Variable::setValueByKeyString(const QString &key_str)
{
    Variable v;
    v.setValue(key_str.toLatin1());
    return Variable::setValueByKey(v);
}

/****************************************************************************
 * @function name:  Variable::valueKey()
 * @param:
 *       
 * @description: 
 * @return: const Variable 
 ****************************************************************************/
const Variable Variable::valueKey()
{
    if(mp_tr)
    {
            return mp_tr->get(Translator::KEY,*this);
    }
    return Variable();
}

/****************************************************************************
 * @function name:  Variable::valueKeyString()
 * @param:
 *       
 * @description: 
 * @return: const QString  
 ****************************************************************************/
const QString Variable::valueKeyString()
{
    return QString(Variable::valueKey().array());
}

/****************************************************************************
 * @function name:  OPERATORS
 * @param:
 *       double, Variable
 * @description: 
 ****************************************************************************/

Variable &Variable::operator =(const int &var)
{
    if(this->type() != Variable::ARRAY) setValue(var);
    return *this;
}

Variable &Variable::operator =(const Variable &var)
{
    this->m_description = var.description();
    this->m_max = var.m_max;
    this->m_min = var.m_min;
    this->m_read_only = var.readOnly();
    this->m_type =var.type();
    this->m_value = var.value();
    this->m_bytearray = var.array();
    emit signal_updateSet(name());
    return *this;
}

Variable &Variable::operator =(const QByteArray &var)
{
    if(this->type() == Variable::ARRAY)
        this->setValue( var);
     return *this;
}


Variable &Variable::operator =(const double &var)
{
    if(this->type() != Variable::ARRAY) setValue(var);
    return *this;
}

Variable &Variable::operator +(const double &var)
{
    this->setValue(value()+var);    
    return *this;
}

Variable &Variable::operator -(const double &var)
{
    this->setValue(value()-var);    
    return *this;
}

Variable &Variable::operator *(const double &var)
{
    this->setValue(value()*var);
    return *this;
}

Variable &Variable::operator /(const double &var)
{
    this->setValue(value()/var);
    return *this;
}

Variable &Variable::operator +(const Variable &var)
{
    this->setValue(value()+var.value());
    return *this;
}

Variable &Variable::operator -(const Variable &var)
{
    this->setValue(value()-var.value());
    return *this;
}

Variable &Variable::operator *(const Variable &var)
{
    this->setValue(value()*var.value());
    return *this;
}

Variable &Variable::operator /(const Variable &var)
{
    this->setValue(value()/var.value());
    return *this;
}

bool Variable::operator ==( const Variable &var)
{
    return this->value() == var.value();
}


bool Variable::operator ==(const double &val)
{
    return this->value() == val;
}


bool Variable::operator != (const Variable &var)
{
    return !(operator ==(var));
}


bool Variable::operator != (const double &val)
{
    return !(operator ==(val));
}


/****************************************************************************
 * @function name:  Variable::setExtra()
 * @param:
 *       const QString &name
 *       const QByteArray &value
 * @description: 
 * @return: void 
 ****************************************************************************/
void Variable::setExtra(const QString &name, const QString &value)
{
    m_extra[name] = value;
}

QStringList Variable::extras()
{
    return m_extra.keys();
}


/****************************************************************************
 * @function name:  Variable::extra()
 * @param:
 *       const QString &name
 * @description: 
 * @return: QString
 ****************************************************************************/
QString Variable::extra(const QString &name)
{
    if(m_extra.contains(name))
    {
        return  m_extra[name];
    }
    return QString();
}

bool Variable::operator >(const double &var)
{
    return this->value()>var;
}

bool Variable::operator <(const double &var)
{
    return this->value()<var;
}


void Variable::makeSetup(const QString &init_script)
{
    // FORMAT:@name=BIT @value=10 @descr=test @someextra=dima
    QStringList l1 = QString(init_script).trimmed().split('@',QString::SkipEmptyParts);
    foreach(const QString &item, l1)
    {     
        bool ok=false;
        QRegExp rx;
        rx.setPattern("(\\w+)=(.*)");
        ok  = rx.indexIn(item)>=0 ;
        if(!ok)
        {
            rx.setPattern("(.*)()");
            ok = rx.indexIn(item)>=0;
        }
        QString par = rx.cap(1).trimmed();
        QString val = rx.cap(2).trimmed();
        if(ok)
        {
            if(par == "value" )
            {
                bool ok;
                double value = val.toDouble(&ok);
                if(ok){
                    m_type = Variable::REAL;
                    m_value = value;
                }
                else{
                    m_type = Variable::ARRAY;
                    this->m_bytearray=val.toLatin1();
                }
            }
            else if(par == "descr")
            {                
                m_description = val;
            }
            else if(par == "type")
            {
                if(val == "bit"){
                    m_type = Variable::INTEGER;
                }
                else if(val == "integer"){
                    m_type = Variable::INTEGER;
                }
                else if(val == "real"){
                    m_type = Variable::REAL;
                }
                else if(val == "string" ||val == "array"){
                    m_type = Variable::ARRAY;
                }
            }
            else if(par == "min")
            {
                m_min = val.toDouble();
            }
            else if(par == "max")
            {
                m_max = val.toDouble();
            }
            else if(par == "tr")
            {
                setTranslator(new Translator(val));
            }
            else if(par == "variable" || par == "name"){
                this->setName(val);
                m_type = Variable::REAL;
            }
            else if(par == "readonly")
            {
                if(val.isEmpty())
                    setReadOnly(1);
                else
                    setReadOnly(val.toInt());
            }
            else{                                
                  setExtra(par,val);
            }
        }
    }
}

const QString Variable::getSetup()
{
    return m_setup_script;
}

void Variable::bind(QObject *pobj, const char *Set, const char *Get)
{
    connect(this,SIGNAL(signal_updateSet(QString)),pobj,Set);
    connect(this,SIGNAL(signal_updateGet(QString)),pobj,Get);
}

void Variable::setUpdatePolicy(Variable::UpdatePolicy upd_pol)
{
    m_update_policy = upd_pol;
}
