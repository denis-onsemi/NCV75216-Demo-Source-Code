/**
 * @file:register.cpp   -
 * @description: this is class to maintain bits with names
 * @project: BENCH OnSemiconductor
 * @date: 2014\11\27 13-43-41
 *
 */


#include "register.h"
#include <QDebug>
#include "bitfieldparser.h"
#include <QStringList>

const char Register::TAG_PURENAME[]     = "@purename";
const char Register::TAG_NAME[]     = "@name";
const char Register::TAG_VALUE[]    = "@value";
const char Register::TAG_VALUE_HEX[] = "@value_hex";
const char Register::TAG_GROUP[]    = "@group";
const char Register::TAG_BITN[]     = "@bitn";
const char Register::TAG_DESCR[]    = "@descr";

const char *Register::TAG_LIST[] = {TAG_NAME
                                    ,TAG_VALUE
                                    ,TAG_VALUE_HEX
                                    ,TAG_GROUP
                                    ,TAG_BITN
                                    ,TAG_DESCR};

// register used sub function



/****************************************************************************
 * copy constructor
 *
 *     const Register &reg - register
 *
 * @description: creates register with bits without names
 ****************************************************************************/
Register::Register( const Register &reg)
{
    m_isSub = false;
    m_ReadOnly = false;
    mp_temporary= NULL;
    m_update_policy = UpdateOnChange;
    copy(this,const_cast<Register*> (&reg));
}


/****************************************************************************
 * @function name: constructor
 *
 * @param:
 *
 *  const QByteArray &bit_list
 * @description: this constructor cretaes bit scan chain with names and values
 *                name can be passed as normal c names without []
 *                value can be passed with name as "bit8=1"
 ****************************************************************************/
Register::Register(const QString &init_script, const QString  &name )
{
    m_isSub = false;
    m_ReadOnly = false;
    m_name = name;
    mp_temporary= NULL;
    m_update_policy = UpdateOnChange;
    makeSetup(init_script);
}


/****************************************************************************
 * @function name: constructor
 *
 * @param:
 *
 *  void
 * @description:
 ****************************************************************************/
Register::Register()
{
    m_isSub = false;
    m_ReadOnly = false;
    mp_temporary= NULL;
    m_update_policy = UpdateOnChange;
}

/****************************************************************************
 * @function name: constructor
 *
 * @param:
 *
 *  qint32 bit_count
 * @description: creates register with bits without names
 ****************************************************************************/
Register::Register(qint32 bit_count,const QString  &name )
{
    m_isSub = false;
    m_ReadOnly = false;
    mp_temporary= NULL;
    m_update_policy = UpdateOnChange;
    resize(bit_count);
    m_name = name;
}


/****************************************************************************
 * @function name: Register::addBit()
 *
 * @param:
 *
 *          Bit *pbit
 * @description: Adds bit by pointer
 * @return: ( void )
 ****************************************************************************/
void Register::addBit(Bit *pbit, qint32 put_to)
{
    if(put_to <0)
    {
        m_chain.append(pbit);
    }
    else
    {
        m_chain.insert(put_to,pbit);
    }
}

void Register::removeBit(Bit *pbit)
{
    if(pbit)
    {
    if(m_chain.indexOf(pbit)>=0)
	    delete pbit;
    }
}

/****************************************************************************
 * @function name: Register::findMaxGroupId()
 *
 * @param:
 *
 * @description: Finds maximum group id number in chain
 * @return: ( qint32 )
 ****************************************************************************/
qint32 Register::findMaxGroupId()
{
    qint32 result =0;
    for(qint32 i=0;i<m_chain.size();i++)
    {
        if(m_chain.at(i)->group_id()>result)
        {
            result = m_chain.at(i)->group_id();
        }
    }
    return result;
}

/****************************************************************************
 * @function name: Register::addBit()
 *
 * @param:
 *   const QByteArray &bitname
 * @description: Adds bit with name value automatically set to zero
 *              m_chain is a list container of single bits. each bit is represented
 *              by pointer to object Bit. So m_chain index equals to bit position in list.
 *              Bit are stored from LSB to msb.
 *              Example: Register("A[8];B[8]")
 *                       in m_chain is A(0,,,7),B(0,,,7),...
 *              name is the same.
 *              There are two types of parser used
 *              I) has format "@register=NAME;@bit=A[8] @value=123 @descr=DESCRIPTION;..."
 *              II) second format "A[8]=0;B[8]=123#DESCRIPTION;,,,"
 * @return: ( int ) number of bits added
 ****************************************************************************/
int Register::addBit(const QString &bitname, qint32 put_to)
{    
    int result=0;
    if(!bitname.isEmpty())
    {
        if(bitname.trimmed().startsWith('@'))
        {
            /*
            Bits are separated with ';'
            To set bit name use @name, @field, @bit tags.
            FORMAT:
               @name=BIT @value=10 @descr=test @{extra_name}={extra data};
             */
            BitFieldParser parser;
            QHash <QString, QString> extras_list;
            quint32  value=0;
            bool is_field =false;
            QString name,description;
            QStringList l1 = bitname.trimmed().split('@',QString::SkipEmptyParts);
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
                    continue;
                }
                QString par = rx.cap(1).trimmed();
                QString val = rx.cap(2).trimmed();

                    if(par == "name" || par == "field"|| par == "bit")
                    {
                        if(!parser.load(val.toLatin1().constData())) return 0;
                        name = parser.name();
                        if(!m_items.contains(name))  {
                            m_items.append(name);
                            
                        }
                        else {
                            qWarning()<<""   ;
                        }
			is_field =true;
                    }
                    else if( par == "value")
                    {
                        if(val.startsWith("0x"))
                        {
                            value = val.mid(2).toUInt(0,16);
                        }
                        else
                            value = val.toUInt();
                    }
                    else if(par == "descr")
                    {
                        description = val;
                    }
                    else if( par == "register")
                    {
                        /* set registername */
                        this->setName(val);
                    }
                    else if( par == "readonly")
                    {
                        if(val.isEmpty())
                            m_ReadOnly = true;
                        else m_ReadOnly = val.toUInt();
                    }
                    else {                        
                        if(!is_field)
                        {
                            setExtra(par,val);
                        }
                        else {
                            extras_list[par] =val;
                        }
                    } 
            }
            if(is_field)
            {
                qint32 count  = (parser.lsb() == parser.msb() && parser.msb() > 0) ?
                            parser.msb() :
                            (parser.msb() - parser.lsb()) + 1;

                for (qint32 i = 0; i < count; i++)
                {
                    Bit *pbit = new Bit;
                    pbit->setDescription(description);
                    pbit->setName(name);
                    pbit->setValue((value & (1 << i)) != 0);                    
                    foreach(const QString &extra_name,extras_list.keys()){
                        pbit->setExtra(extra_name,extras_list[extra_name]);
                    }                    
                    if(put_to<0)
                        addBit(pbit,-1);
                    else
                        addBit(pbit,put_to+i);
                }
                extras_list.clear();
                result=count;
            }
        }
        // simple parser
        else
        {
            BitFieldParser  parser(bitname.toLatin1().constData());
            if (parser.isOk() )
            {
                qint32 count  = (parser.lsb() == parser.msb() && parser.msb() > 0) ?
                            parser.msb() :
                            (parser.msb() - parser.lsb()) + 1;
                //qint32 count  = (msb-lsb) +1;

                for (qint32 i = 0; i < count; i++)
                {
                    Bit *pbit = new Bit;
                    pbit->setDescription(parser.description());
                    pbit->setName(parser.name());
                    pbit->setValue((parser.value() & (1 << i)) != 0);
                    if(put_to<0)
                        addBit(pbit);
                    else
                        addBit(pbit,put_to+i);
                }
                result=count;
            }
        }
        regroup();
    }
    return result;
}


/****************************************************************************
 * @function name: Constructor
 *
 * @param:
 *             void
 * @description:
 * @return: (  )
 ****************************************************************************/
Register::~Register()
{
    clear();
}


/****************************************************************************
 * @function name: Register::setValue()
 *
 * @param:
 *
 *      QByteArray field -field name.
 *                      Format is: (I)"string[nn]" -where nn = number of bit
 *                                  'string' is not requred.this parameter
 *                                  will ignored
 *                                 (II) "string[nn:yy]" - nn:yy is a field
 *      quint32 value  -value is value to pass to a field or bit
 * @description: This function sets value to scanchain
 * @return: ( void )
 ****************************************************************************/
bool Register::setValue(const QString &field, quint32 value)
{    
    bool result = false;
    BitFieldParser parser(field.toLatin1().constData());
    if (parser.isOk())
    {
        if(parser.rangeIsName())
        {
            Register *preg = sub(parser.range_name());
            if(!preg->isEmpty())
            {               
                preg->setUInt(value);
                result = true;
            }
            //virtual register
            else if(m_vregs.contains(parser.range_name()))
            {                
                m_vregs[parser.range_name()]->set(value);
                m_vregs[parser.range_name()]->val = value;
                result = true;                
            }
        }
        else
        {
            result = setValue(parser.lsb(), parser.msb(),value);
        }
    }
    return result;
}

/****************************************************************************
 * @function name: Register::setValue()
 *
 * @param:
 *      quint32 from - index from
 *      quint32 to - index to
 *      quint32 value - value is value to pass to a field or bit
 * @description: This function sets value to scanchain
 * @return: ( void )
 ****************************************************************************/
bool Register::setValue(quint32 from, quint32 to,quint32 value)
{
    bool upd=false;
    bool result = false;
    quint32 i;
    quint32 j = 0;

    if(from >to )
        qSwap(from,to);

    if(to >= (quint32)size())
        to = size()-1;

    for (i = from; i <= to; i++)
    {
        bool cur_bit = (bool)((value >> j) & 1);
        upd |= (bit(i) != cur_bit);
        setBit(i,cur_bit);
        j++;
        result = true;
    }

    switch(m_update_policy)
    {
    case UpdateOnChange:
        if(!upd) break;        
    case UpdateAlways:
        __SET(name());
        break;
    }
    return result;
}

void Register::regroup()
{
    int group_id=-1;
    QString previous;
    for(int i=0;i<m_chain.count();i++)
    {
        if (m_chain.at(i)->name() != previous) group_id ++;
        m_chain.at(i)->setGroup_id(group_id);
        previous = m_chain.at(i)->name();
    }
}


/****************************************************************************
 * @function name: Register::value()
 *
 * @param:
 *
 *  QByteArray field - see format above
 * @description: thisfunction returns value of field or single bit
 * @return: ( quint32 ) returns value
 ****************************************************************************/
quint32 Register::value(const QString &field)
{
    // emit signal_updateGet(name());
    BitFieldParser parser(field.toLatin1().constData());
    qint32 value = 0;
    if (parser.isOk())
    {
        if(parser.rangeIsName())
        {
            Register *preg = sub(parser.range_name());
            if(!preg->isEmpty())
            {
                value = preg->toUInt();
            }
            else if(m_vregs.contains(parser.range_name()))
            {
                value = m_vregs[parser.range_name()]->get();
            }
        }
        else
        {
            qint32 i, j = 0;
            for (i = parser.lsb(); i <= parser.msb(); i++)
            {
                if ( i > m_chain.size() - 1 ) break;
                value |= (static_cast<bool>(m_chain.at(i)->value()) << j);
                j++;
            }
        }
    }
    return value;
}

/****************************************************************************
 * @function name: Register::value()
 *
 * @param:
 *  qint32 from
 *  qint32 to
 *
 * @description: thisfunction returns value of field or single bit
 * @return: ( quint32 ) returns value
 ****************************************************************************/
quint32 Register::value(qint32 from, qint32 to)
{
   emit signal_updateGet(name());
    quint32 value = 0;
    qint32 i, j = 0;
    if(from >to )
        qSwap(from,to);
    for (i = from; i <= to; i++)
    {
        if ( i > m_chain.size() - 1 ) break;
        value |= (static_cast<bool>(m_chain.at(i)->value()) << j);
        j++;
    }
    return value;
}

/****************************************************************************
 * @function name: Register::toUInt()
 *
 * @param:
 *      BitOrder bitorder
 * @description: converts register to unsigned 32bit unsigned int
 * @return: ( quint32 ) returns value
 ****************************************************************************/
quint32 Register::toUInt(BitOrder bitorder)
{
    emit signal_updateGet(name());
    quint32 value =0;
    qint32 i;
    for (i = 0; i <m_chain.size(); i++)
    {
        qint32 index;
        if(bitorder == MSB)
        {
            index = m_chain.size()-1-i;
        }
        else index= i;
        value |= (static_cast<bool>(m_chain.at(index)->value()) << i);
    }
    return value;
}

/****************************************************************************
 * @function name: Register::setUInt()
 *
 * @param:
 *      quint32 value
 *      BitOrder bitorder
 * @description: sets value to register as unsigned int 32 bit
 * @return: ( void )
 ****************************************************************************/
void Register::setUInt(quint32 value, BitOrder bitorder)
{
    bool upd = false;
    qint32 i;
    qint32 j = 0;
    for (i = 0; i < m_chain.size(); i++)
    {
        qint32 index;
        if(bitorder == MSB)
        {
            index = m_chain.size()-1-i;
        }
        else index= i;
        bool cur_bit = (bool)((value >> j) & 1);
        upd |= bit(index)!=cur_bit;     
        setBit(index, cur_bit);
        j++;
    }    

    switch(updatePolicy())
    {
    case UpdateOnChange:
        if(!upd)   break;
    case UpdateAlways:
        __SET(name());
        break;
    }
}


/****************************************************************************
 * @function name: Register::value()
 *
 * @param:
 *      qint32 field_number
 * @description: this function returns value of single bit
 * @return: ( quint32 ) returns value
 ****************************************************************************/
quint32 Register::value(qint32 bit_number)
{
    return bit(bit_number);
}


/****************************************************************************
 * @function name: Register::setBit()
 *
 * @param:
 *      qint32 bitn -bit number to set
 *      bool value  - bit value
 * @description: This function sets bit by its number
 * @return: ( void )
 ****************************************************************************/
void Register::setBit(qint32 bitn, bool value)
{
    if ( (bitn >= 0) && (bitn < m_chain.size()) )
    {
        m_chain.at(bitn)->setValue(value);
    }
}


/****************************************************************************
 * @function name: Register::setBit()
 *
 * @param:
 *      const QByteArray &bitname -bit name to set
 *      bool value  - bit value
 * @description: This function sets bit by its number
 * @return: ( void )
 ****************************************************************************/
void Register::setBit(const QString &bitname, bool value)
{
    if ( !bitname.isEmpty())
    {
        qint32 index = findBitByName(bitname);
        if (index >= 0 && index < m_chain.size())
        {
            Bit *pbit = m_chain.at(index);
            pbit->setValue(value);
        }
    }
}

/****************************************************************************
 * @function name: Register::bit()
 *
 * @param:
 *      const QByteArray &bit_name
 * @description: this function gets bit
 * @return: ( bool ) - bit value
 ****************************************************************************/

Bit *Register::bit(const char *bit_name)
{    
    int bitn= this->findBitByName(bit_name);
    if(bitn>=0)
    {
        return this->m_chain[bitn];
    }
    else return NULL;
}
Bit *Register::bit(const QString &bit_name)
{
    int bitn= this->findBitByName(bit_name);
    if(bitn>=0)
    {
        return this->m_chain[bitn];
    }
    else return NULL;
}
/****************************************************************************
 * @function name: Register::bit()
 *
 * @param:
 *      qint32 bitn
 * @description: this function gets bit
 * @return: ( bool ) - bit value
 ****************************************************************************/
bool Register::bit(qint32 bitn)
{    
    return (bool)m_chain.at(bitn)->value();
}


/****************************************************************************
 * @function name: Register::bitAt()
 *
 * @param:
 *      qint32 bitn - bit number
 * @description: tis function returns pointer to a bit
 * @return: (Bit *) - bit pointer
 ****************************************************************************/
Bit *Register::bitAt(qint32 bitn)
{
    return m_chain.at(bitn);
}

/****************************************************************************
 * @function name: Register::contains()
 *
 * @param:
 *      const QString &name
 * @description: if register contains bit name
 * @return: (Bit *) - bit pointer
 ****************************************************************************/
bool Register::contains(const QString &name)
{
    int bitn = findBitByName(name);
    return bitn>=0;
}

/****************************************************************************
 * @function name: Register::join()
 *
 * @param:
 *      const Register &reg
 * @description: this joins (appends ) to the end
 *          if register is subregister - function will append just pointer(s) of bit
 * @return: (Bit *) - bit pointer
 ****************************************************************************/
void Register::join(Register &reg)
{
    int i;
    for (i=0;i< reg.size();i++)
    {
        if(this->isSub())
        {
            addBit(reg.bitAt(i));
        }
        else {
            addBit(new Bit(*reg.bitAt(i)));
        }
    }
}


/****************************************************************************
 * @function name: Register::toByteArray()
 *
 * @param:
 *        BitOrder bitorder
 *
 * @description: This function converts current scanchain to bytearray
 * @return: (QByteArray ) - returns number of bits in bytearray
 ****************************************************************************/
QByteArray Register::toByteArray(BitOrder bitorder,bool contiguously)
{    
    emit signal_updateGet(name());
    QByteArray bytearray;
    quint8 byte = 0;
    for (qint32 byte_i = 0; byte_i < size(); )
    {
        quint8 bit;
        qint32 index = byte_i;
        if(bitorder == MSB)
        {
            index = size() - 1 - byte_i;
        }
        bit = (m_chain.at(index)->value() != 0);
        if(contiguously)
        {
            byte |= (bit<< (7-(byte_i % 8)));
        }
        else{
            byte |= (bit<<    (byte_i % 8));
        }
        byte_i++;
        if ( (byte_i % 8) == 0 || byte_i >= size() )
        {
            bytearray.append(byte);
            byte = 0;
        }
    }
    return bytearray;
}

QString Register::toHex(Register::BitOrder bitorder, bool contiguously)
{
    emit signal_updateGet(name());
    return QString(toByteArray(bitorder,contiguously).toHex());
}


/****************************************************************************
 * @function name: Register::fromByteArray()
 *
 * @param:
 *       const QByteArray &bytearray - data array
 *       qint32 scanchain_length - length in bits
 *       BitOrder bitorder (default = LSB)
 *       bool contiguously  bits go like this 7.6.5..1.0.7.
 *
 * @description: This function populates scan chain by values from bytearray
 *
 *              !!!if register is Empty (size ==0)
 *                      will be resized and then populated
 * @return: ( bool ) -if success
 ****************************************************************************/
bool Register::fromByteArray(const QByteArray &bytearray
                             , qint32 scanchain_length
                             , BitOrder bitorder
                             ,bool contiguously
                             )
{
    bool result=false;
    qint32 bit_i = 0;
    qint32 byte_i;
    quint8 byte = bytearray[0];
    if(scanchain_length == -1) scanchain_length=bytearray.size() * 8;
    if( size() == 0 )
    {
        resize(scanchain_length);
    }
    if (( bytearray.size() * 8) >= scanchain_length)
    {
        for (qint32 i = 0; i < scanchain_length; i++)
        {
            byte_i = i / 8;
            bit_i = i % 8;
            byte = bytearray[byte_i];
            quint8 bit;
            if(contiguously)
            {
                bit = (byte >> (7-bit_i)) & 1;
            }
            else {
                bit = (byte >> bit_i) & 1;
            }
            if(bitorder == LSB)
                setBit(i , bit);
            else setBit(size()-1-i , bit);
        }
        result=true;
    }    
    // ???? __SET(name());
    return result;
}

bool Register::fromHex(const QString &hex, qint32 scanchain_length, Register::BitOrder bitorder, bool contiguously)
{
    return fromByteArray(QByteArray::fromHex(hex.toLatin1()), scanchain_length, bitorder, contiguously);
}


/****************************************************************************
 * @function name: Register::toBitString()
 *
 * @param:
 *
 *        BitOrder bitorder
 * @description: This function converts current scanchain to bit string
 *                "101111" (MSB...LSB)
 * @return: ( QByteArray ) - returns number of bits in bytearray
 ****************************************************************************/
QByteArray Register::toBitString(BitOrder bitorder)
{
    emit signal_updateGet(name());
    QByteArray bytearray;
    quint8 byte = 0;
    qint32 i = 0;
    for (i=0;i<m_chain.size();i++)
    {
        if(bitorder ==MSB)
        {
            byte = (quint8)m_chain.at(m_chain.size()-1-i)->value();
        }
        else
        {
            byte = (quint8)m_chain.at(i)->value();
        }
        bytearray.append(QByteArray::number(byte, 10));
    }
    return bytearray;
}



/****************************************************************************
 * @function name: Register::fromBitStringToUint()
 *
 * @param:
 *
 *       onst QByteArray & ba  - data array
 * @description: This function converts current bit string to uint
 *               ByteArray("x7....x0")
 *               spaces or '.' as separators are available
 * @return: ( qint32 ) - returns value
 ****************************************************************************/
quint32 Register::fromBitStringToUint(const QByteArray & ba)
{
    quint32 result = 0;
    quint32 i = ba.size() - 1;
    qint32 bitn = 0;
    while(bitn<ba.size())
    {
        char c = ba.at(i);
        if(c == '1')
        {
            result |= 1 << bitn;
        }
        i--;
        if(c ==' ' || c == '.')
        {
            continue;
        }
        bitn++;
    }
    return result;
}

/****************************************************************************
 * @function name: Register::fromBitString()
 *
 * @param:
 *       QByteArray &bytearray  - datat to load
 *       BitOrder bitorder
 *
 * @description: This function loads "101111" (MSB...LSB)  text
 *               to current scanchain
 * @return: ( void )
 ****************************************************************************/
void Register::fromBitString(const QByteArray &bytearray, BitOrder bitorder)
{
    for (qint32 i = 0; i < bytearray.size(); i++)
    {
        qint32 index;
        if(bitorder == MSB)
        {
            index = bytearray.size() - i - 1;
        }
        else index = i;
        setBit( index, bytearray[i] != '0');
    }
    // ???? emit signal_updateSet(name());
}


/****************************************************************************
 * @function name: Register::fieldDescription()
 *
 * @param:
 *
 *        const QByteArray &fieldname - name of field
 * @description: This function gets field description
 * @return: (bool)
 ****************************************************************************/
bool Register::field(QString *pbit_name,QString *pbit_description, const QByteArray &fieldname)
{
    BitFieldParser parser(fieldname.constData());
    if (parser.isOk())
    {
        if(parser.rangeIsName())
        {
            int i = findBitByName(QByteArray(parser.range_name()));
            if(i>=0){
                if(pbit_name ) *pbit_name = parser.range_name();
                if(pbit_description) *pbit_description = m_chain.at(i)->description();
                return true;
            }
            //vregs
            else if( m_vregs.contains(parser.range_name()))
            {
                if(pbit_name ) *pbit_name = parser.range_name();
                if(pbit_description ) *pbit_description = m_vregs[parser.range_name()]->descr;

            }
        }
        else if ( parser.lsb() >= 0 && parser.lsb() < m_chain.size())
        {
            if(pbit_name ) *pbit_name = m_chain.at(parser.lsb())->name();
            if(pbit_description) *pbit_description = m_chain.at(parser.lsb())->description();
            return true;
        }
    }
    return false;
}




/****************************************************************************
 * @function name: Register::clear()
 *
 * @param:
 *             void
 * @description: clear bits from chain. Deletes bits
 * @return: ( void )
 ****************************************************************************/
void Register::clear()
{
    qint32 bits_count = m_chain.size();
    while ( bits_count--)
    {
        Bit *pbit = m_chain.first();
        m_chain.removeFirst();
        if(! isSub())
            delete pbit;
    }
    foreach(const QString &name, m_vregs.keys())
    {
        delete m_vregs[name];
        m_vregs.remove(name);
    }
}


/****************************************************************************
 * @function name: Register::readOnly()
 *
 * @param:
 *            void
 * @description:
 * @return: ( bool )  -returns true if readonly
 ****************************************************************************/
bool Register::readOnly() const
{
    return m_ReadOnly;
}


/****************************************************************************
 * @function name: Register::setReadOnly()
 *
 * @param:
 *             bool
 * @description: -set true if readonly
 * @return: ( void)
 ****************************************************************************/
void Register::setReadOnly(bool on)
{
    m_ReadOnly = on;
}

Register &Register::operator [](const char *field)
{
    return *sub(field);
}

Register &Register::operator [](const QString &field)
{
    return *sub(field);
}

/****************************************************************************
 * @function name: Register::copy()
 *
 * @param:
 *             bool
 * @description: -copies register. Copies same register or if
 *                register is zero size
 * @return: ( bool ) true -success
 ****************************************************************************/
bool Register::copy(Register *pdst, Register *psrc)
{
    bool result =false;
    qint32 src_size =psrc->size();

    if( pdst->isSame(psrc) )
    {
        for (qint32 i = 0; i <src_size; i++)
        {
            pdst->setBit(i,psrc->bit(i));
        }
        result = true;
    }
    else if(pdst->m_chain.size() == 0 )
    {
        pdst->resize(src_size);
        for (qint32 i = 0; i < src_size; i++)
        {
            *(pdst->m_chain.at(i)) = *(psrc->bitAt(i));
        }
        result = true;
    }
    return result;
}

/****************************************************************************
 * @function name: Register::appendVirtual()
 * @param:
 *             const QString &name,
 *             const QString &descr
 *              Virtual *pvreg
 * @description: Appends virtual register
 *      Virtual register is register is a part of register (bit/field).
 *      Such register can be accessed using setValue() value() functions.
 * @return: ( bool ) true -success
 ****************************************************************************/
bool Register::appendVirtual(const QString &name, const QString &descr, Virtual *pvreg)
{
    if(pvreg && !name.isEmpty() && !contains(name) )
    {
        pvreg->preg = this;
        //if description set in constructor
        if(pvreg->descr.isEmpty())  pvreg->descr= descr;
        m_vregs[name]= pvreg;
        return true;
    }
    return false;
}

/****************************************************************************
 * @function name: Register::encodeMeToHamming()
 * @param:
 *             bool secded
 * @description: encodes to hamming
 * @return: ( bool ) true -success
 ****************************************************************************/
bool Register::encodeMeToHamming(bool secded)
{
    if(size()==16)
    {
        quint32 value  = ham_encode(toUInt(), 21);
        if(secded)
        {
            value = ham_add_secded(value,21);
            resize(22);
        }
        else resize(21);
        setUInt( value );
        return true;
    }
    return false;
}

/****************************************************************************
 * @function name: Register::decodeMeFromHamming()
 *
 * @param:
 *             bool secded
 * @description:
 * @return: ( bool ) true -success
 ****************************************************************************/
bool Register::decodeMeFromHamming(bool secded)
{
    quint32 value;
    if(secded )
    {
        if(!ham_check_secded(toUInt(),21)) {
            return false;
        }
    }
    if(ham_decode(toUInt(), &value, 21))
    {
        clear();
        resize(16);
        setUInt(value);
        return true;
    }
    return false;
}




/****************************************************************************
 * @function name: Register::operator =()
 *
 * @param: Register
 *
 * @description: copies register. IF register is similar (contains the same bits),
 *              it will just copy values. if register is not similar and is zero size
 *              function will make a copy of given register
 ****************************************************************************/
//[1]

Register &Register::operator =(const Register &reg)
{
    copy(this,const_cast<Register*>(&reg));    
    return *this;
}

/****************************************************************************
 * @function name: Register::operator !()
 *
 * @param: Register
 *
 * @description: operator not
 * @return: ( Register )  - returns not value
 ****************************************************************************/

Register Register::operator !()
{
    Register r= *this;
    for (qint32 i = 0; i < size(); i++)
    {
        r.bitAt(i)->setValue(!r.bitAt(i)->value());
    }
    __SET(name());
    return r;
}

//[2]
/****************************************************************************
 * @function name: Register::operator =()
 *
 * @param: quint32
 *
 * @description: fills register with value. doesnot change register structure
 * @return: ( qint32 )  -returns number of bits in scanchain
 ****************************************************************************/
Register &Register::operator = (quint32 val)
{
    bool upd=false;
    if(32 >= this->size())
    {
        for (qint32 i = 0; i < size(); i++)
        {            
            upd |= (bit(i) != ((val>>i)&1));
            setBit(i,(val>>i)&1);
        }
    }

    switch(m_update_policy)
    {
    case UpdateOnChange:
        if(!upd) break;
    case UpdateAlways:
        __SET(name());
        break;
    }
    return *this;
}




/****************************************************************************
 * @function name: Register::operator &=
 *
 * @param:
 *              Register &reg)
 * @description:
 * @return: ( Bit * )  - returns pointer to bit
 ****************************************************************************/
//[1]
Register &Register::operator &= (Register &reg)
{
    if (this->size() == reg.size())
    {
        for (qint32 i = 0; i < size(); i++)
        {
            bool result = m_chain.at(i)->value() & reg.bitAt(i)->value();
            m_chain.at(i)->setValue(result);
        }
    }
    __SET(name());
    return *this;
}
//[2]
Register &Register::operator &=(quint32 val)
{
    if(32 == this->size())
    {
        for (qint32 i = 0; i < size(); i++)
        {
            bool result = (bool)m_chain.at(i)->value() & (bool)((val>>i)&1);
            m_chain.at(i)->setValue(result);
        }
    }
    __SET(name());
    return *this;
}


/****************************************************************************
 * @function name: Register::operator ^=
 *
 * @param:
 *
 * @description:
 * @return: ( Bit * )  - returns pointer to bit
 ****************************************************************************/
//[1]
Register &Register::operator ^= (Register &reg)
{
    if (this->size() == reg.size())
    {
        for (qint32 i = 0; i < size(); i++)
        {
            bool result = m_chain.at(i)->value() ^ reg.bitAt(i)->value();
            m_chain.at(i)->setValue(result);
        }
    }
    __SET(name());
    return *this;
}
//[2]
Register &Register::operator ^= (quint32 val)
{
    if(32 == this->size())
    {
        for (qint32 i = 0; i < size(); i++)
        {
            bool result = m_chain.at(i)->value() ^ ((val>>i)&1);
            m_chain.at(i)->setValue(result);
        }
    }
    __SET(name());
    return *this;
}


/****************************************************************************
 * @function name: Register::operator |=
 *
 * @param:
 *
 * @description:
 * @return: ( Bit * )  - returns pointer to bit
 ****************************************************************************/
//[1]
Register &Register::operator |= (Register &reg)
{
    if (this->size() == reg.size())
    {
        for (qint32 i = 0; i < size(); i++)
        {
            bool result = m_chain.at(i)->value() | reg.bitAt(i)->value();
            m_chain.at(i)->setValue(result);
        }
    }
    __SET(name());
    return *this;
}
//[2]
Register &Register::operator |= (quint32 val)
{
    if(32 >= this->size())
    {
        for (qint32 i = 0; i < size(); i++)
        {
            bool result = m_chain.at(i)->value() | (bool)((val>>i)&1);
            m_chain.at(i)->setValue(result);
        }
    }
    __SET(name());
    return *this;
}


/****************************************************************************
 * @function name: Register::operator ==
 *
 * @param:
 *          Register &reg)
 * @description:
 * @return: ( bool )
 ****************************************************************************/
bool Register::operator == (Register &reg)
{
    bool result = false;
    if (size() == reg.size())
    {
        result = true;
        for (qint32 i = 0; i < size(); i++)
        {
            if (m_chain.at(i)->value() != reg.bitAt(i)->value())
            {
                result = false;
                break;
            }
        }
    }
    return result;
}


/****************************************************************************
 * @function name: Register::operator ==
 *
 * @param:
 *              quint32 val
 * @description:
 * @return: ( bool )
 ****************************************************************************/
bool Register::operator == (quint32 val)
{
    bool result ;
    result = true;
    for (qint32 i = 0; i < size(); i++)
    {
        if (m_chain.at(i)->value() != (bool)((val>>i)&1) )
        {
            result = false;
            break;
        }
    }
    return result;
}


/****************************************************************************
 * @function name: Register::operator !=
 *
 * @param:
 *              Register &reg
 * @description: operator not equal
 * @return: ( bool )
 ****************************************************************************/
bool Register::operator != (Register &reg)
{
    return !(operator ==(reg));
}


/****************************************************************************
 * @function name: Register::operator !=
 *
 * @param:
 *              quint32 vl
 * @description: operator not equal
 * @return: ( bool )
 ****************************************************************************/
bool Register::operator != (quint32 val)
{
    return !(operator ==(val));
}



/****************************************************************************
 * @function name: Register::fill()
 *
 * @param:
 *          bool value - value of bit
 *          qint32 count    -count of bit from start
 *          qint32 start - start bit number
 * @description: fills register bits with value from start
 * @return: ( void )
 ****************************************************************************/
void Register::fill(bool value, qint32 count, qint32 start)
{
    if (count == -1)
    {
        count = size();
    }
    for (qint32 i = start; i < count; i++)
    {
        setBit(i, value);
    }
    __SET(name());
}

void Register::rotateRight(int count, bool fill)
{
    while( count-- ){
        bool v = fill;
        for(int i=0; i<m_chain.count(); i++){
            bool tmp = m_chain.at(i)->value();
            m_chain.at(i)->setValue(v);
            v = tmp;
        }
    }
}

void Register::rotateLeft(int count, bool fill)
{
    while( count-- ){
        bool v = fill;
        for(int i=m_chain.count()-1; i>=0; i--){
            bool tmp = m_chain.at(i)->value();
            m_chain.at(i)->setValue(v);
            v = tmp;
        }
    }
}

/****************************************************************************
 * @function name: Register::invert()
 *
 * @description: inverts register
 * @return: ( void )
 ****************************************************************************/
void Register::invert()
{
    for (qint32 i = 0; i < size(); i++)
    {
        m_chain.at(i)->setValue(!m_chain.at(i)->value());
    }
    __SET(name());
}


/****************************************************************************
 * @function name: Register::toString()
 *
 * @param:
 *             void
 * @description: Converts register to string
 * @return: ( const QByteArray)  -returns string
 ****************************************************************************/
const QString Register::toString(const QString &format, bool grouped, bool include_virtual)
{    
    QByteArray result ;
    if(grouped)
    {
        Register reg;
        reg.setSub(1);
        QString prev_name;
        qint32 range_start=0;
        for(int i=0;i<=size();i++)
        {

            if((i == size() || prev_name != bitAt(i)->name()) )
            {
                QString tmp;
                QString line = format;
                if( reg.size()>1 )
                {
                    tmp = prev_name+"["+QByteArray::number(reg.size())+"]";
                    line.replace(TAG_NAME,tmp)
                            .replace(TAG_PURENAME,prev_name)
                            .replace(TAG_VALUE,QString::number(reg.value("[]")))
                            .replace(TAG_VALUE_HEX,QString::number(reg.value("[]"),16))
                            .replace(TAG_GROUP,QString::number(reg.bitAt(0)->group_id()))
                            .replace(TAG_BITN,QString("%1:%2 ").arg(range_start).arg(i-1))
                            .replace(TAG_DESCR,reg.bitAt(0)->description());
                    // replace bit extra with name
                    foreach(const QString &extraname, reg.bitAt(0)->extras())
                    {
                        line.replace("@"+extraname,reg.bitAt(0)->extra(extraname));
                    }
                }
                else if( reg.size() == 1 )
                {
                    tmp = prev_name;
                    line.replace(TAG_PURENAME,prev_name)
                            .replace(TAG_NAME, tmp)
                            .replace(TAG_VALUE, QString::number(reg.value("[]")))
                            .replace(TAG_VALUE_HEX, QString::number(reg.value("[]"),16))
                            .replace(TAG_GROUP, QString::number(reg.bitAt(0)->group_id()))
                            .replace(TAG_BITN, QString("%1 ").arg(range_start))
                            .replace(TAG_DESCR,reg.bitAt(0)->description());
                    // replace bit extra with name
                    foreach(const QString &extraname, reg.bitAt(0)->extras())
                    {
                        line.replace("@"+extraname,reg.bitAt(0)->extra(extraname));
                    }
                }
                // replace register extra with name
                foreach(const QString &extraname, extras())
                {
                    line.replace("@"+extraname,extra(extraname));
                }

                if(!tmp.isEmpty())
                {
                    result += line;
                    reg.clear();
                    range_start = i;
                }
            }
            if(i<size())
            {
                reg.addBit(bitAt(i));
                prev_name = bitAt(i)->name();
            }
        }
        if(include_virtual)
        {
            foreach(const QString &name, m_vregs.keys())
            {
                QString line = format;
                    line.replace(TAG_NAME,name+"[]")
                        .replace(TAG_PURENAME,name)
                        .replace(TAG_VALUE,QString::number(m_vregs[name]->get()))
                        .replace(TAG_VALUE_HEX,QString::number(m_vregs[name]->get(),16))
                        .replace(TAG_GROUP, "")
                        .replace(TAG_BITN, "virtual")
                        .replace(TAG_DESCR,m_vregs[name]->descr);
                result += line;
            }
        }
    }
    else
    {
        for (qint32 i = 0; i <size();i++)
        {
            QString line = format;
            line.replace(TAG_NAME,QString(bitAt(i)->name()))
                    .replace(TAG_VALUE,QString::number(bitAt(i)->value()))
                    .replace(TAG_VALUE_HEX,QString::number(bitAt(i)->value(),16))
                    .replace(TAG_GROUP,QString::number(bitAt(i)->group_id()))
                    .replace(TAG_BITN,QString::number(i))
                    .replace(TAG_DESCR,bitAt(i)->description());
            foreach(const QString &extraname, bitAt(i)->extras())
            {
                line.replace("@"+extraname,bitAt(i)->extra(extraname));
            }
            // replace register extra with name
            foreach(const QString &extraname, extras())
            {
                line.replace("@"+extraname,extra(extraname));
            }
            result += line;
        }

    }
    return result;
}

bool Register::fromString(const QString &text, const char ln_separator, const char eq_separator)
{
    bool ok=false;
    foreach(const QString &line ,text.split(ln_separator,QString::SkipEmptyParts))
    {
        QStringList l=line.split(eq_separator);
        if(l.size()==2)
        {
             sub(l[0])->setUInt(l[1].toUInt());

        }
        else {ok=false;break;}
    }
    return ok;
}


/****************************************************************************
 * @function name: Register::size()
 *
 * @param:
 *             void
 * @description:
 * @return: ( qint32 )  -returns number of bits in scanchain
 ****************************************************************************/
qint32 Register::size() const
{
    return m_chain.size();
}


/****************************************************************************
 * @function name: Register::findBitByName - ---
 *
 * @param:
 *
 *  const QByteArray & name
 * @description:
 * @return: ( qint32 ) returns index of bit in scanchain
 ****************************************************************************/
qint32 Register::findBitByName(const QString &name)
{
    for (qint32 i = 0; i < m_chain.size(); i++)
    {
        if (m_chain.at(i)->name() == name )
        {
            return i;
        }
    }
    return -1 ;
}


/****************************************************************************
 * @function name: Register::addBits()
 *
 * @param:
 *
 *          const QByteArray &bit_string - bits in separated by space or
 *                                          in format bits[5:0]
 * @description:
 * @return: ( void)
 ****************************************************************************/
void Register::addBits(const QString &bit_string, qint32 put_to)
{
    QStringList l = bit_string.split(BITS_SPLITTER);
    foreach(const QString &item, l)
    {
        addBit(item.trimmed(),put_to);
    }
}


/****************************************************************************
 * @function name: Register::resize()
 * @param:
 *      qint32 size -size of register
 * @description: resizes register if size of current register is higher than
 *                  size it will be cleared
 * @return: ( void )
 ****************************************************************************/
void Register::resize(qint32 size_need)
{
    if(size_need >size())
    {
        size_need -= size();
        for (qint32 i = 0; i < size_need; i++)
        {
            addBit(QString("_"));
        }
    }
}


/****************************************************************************
 * @function name: Register::setName()
 * @param:
 *      const QString &name - register name
 * @description: sets name of register
 * @return: ( void )
 ****************************************************************************/
void Register::setName(const QString &name)
{
    m_name =  name;
}


/****************************************************************************
 * @function name: Register::setName()
 * @param:
 *      const QString &name - register name
 * @description: gives name of current reg
 * @return: ( const QString & )
 ****************************************************************************/
const QString &Register::name() const
{
    return m_name;
}


/****************************************************************************
 * @function name: Register::makeSubRegister()
 * @param:
 *      Register *preg - preg to make it as sub
 *      qint32 group_id - gropup id of new reg
 * @description: makes subregister from register passed by pointer (preg)
 * @return: ( void )
 ****************************************************************************/
void Register::makeSubRegister(Register *preg, qint32 group_id)
{
    if(preg)
    {
        preg->clear();
        preg->setSub(true);
        for (qint32 i = 0; i < m_chain.size(); i++)
        {
            if (m_chain.at(i)->group_id() == group_id)
            {
                preg->addBit( m_chain.at(i) );
            }
        }
    }
}

/****************************************************************************
 * @function name: Register::makeSubRegister()
 * @param:
 *      Register *preg - preg to make it as sub
 *      qint32 group_id - gropup id of new reg
 * @description: makes subregister from register passed by pointer (preg)
 * @return: ( void )
 ****************************************************************************/
void Register::makeSubRegister(Register *preg, qint32 from, qint32 to)
{
    if(preg)
    {
        preg->clear();
        preg->setSub(true);
        if(to >= m_chain.size() || to<0) to = m_chain.size()-1;
        for (qint32 i = from; i <= to; i++)
        {
            preg->addBit( m_chain.at(i) );
        }
    }
}


/****************************************************************************
 * @function name: Register::sub()
 * @param:
 *      qint32 from - index from including itself
 *      qint32 to - index to including itself
 *              if to is higher then size() or -1
 *                   will be assigned to value size()-1
 * @description: sets value to range in register
 *
 * @return: ( void )
 ****************************************************************************/
Register * Register::sub(qint32 from, qint32 to)
{        
    if(mp_temporary == NULL)
    {
        makeTemporary();
    }
    mp_temporary->clear();
    qint32 i;
    if(to<0 || to >= m_chain.size()) to = m_chain.size()-1;
    for (i = from; i <= to; i++)
    {
        mp_temporary->addBit(m_chain.at(i));
    }
    return mp_temporary;
}

/****************************************************************************
 * @function name: Register::sub()
 * @param:
 *      const QString &bitname - bits with name
 * @description: sets value to bits with given name
 *
 * @return: ( void )
 ****************************************************************************/
Register * Register::sub(const QString &bitname)
{
    if(mp_temporary == NULL) {
         makeTemporary();
    }
    mp_temporary->clear();
    if(bitname.contains(';'))
    {
        sub(bitname.split(';'));
    }
    else {
        for(qint32 i = 0; i < m_chain.size(); i++)
        {
            if ( m_chain.at(i)->name() == bitname )
            {
                mp_temporary->addBit(m_chain.at(i));
            }
        }
    }
    return mp_temporary;
}

Register *Register::sub(const QStringList &bits)
{
    if(mp_temporary == NULL) {
         makeTemporary();
    }    
    mp_temporary->clear();
    qint32 lsb=-1,msb=-1;
    foreach(const QString &bitname, bits)
    {
        QRegExp rx("((\\d+):(\\d+))|(\\d+)");
        if(rx.exactMatch(bitname) )
        {
            rx.indexIn(bitname);
            if(rx.cap(1).isEmpty())
            {
                lsb = msb= rx.cap(4).toUInt();
            }
            else {
                lsb = rx.cap(2).toUInt();
                msb = rx.cap(3).toUInt();
            }
            for(qint32 i=lsb;i<=msb;i++)
            {
                if(i>=0 && i <size()) mp_temporary->addBit(m_chain.at(i));
            }
        }
        else
        {
            for(qint32 i = 0; i < m_chain.size(); i++)
            {
                if ( m_chain.at(i)->name() == bitname )
                {
                    mp_temporary->addBit(m_chain.at(i));
                }
            }
        }
    }
    return mp_temporary;
}

Register * Register::sub(const QString &extra_name, const QString &extra_value)
{
    if(mp_temporary == NULL) {
        makeTemporary();
    }
    mp_temporary->clear();
    for(qint32 i = 0; i < m_chain.size(); i++)
    {
        if ( m_chain.at(i)->extra(extra_name) == extra_value )
        {
            mp_temporary->addBit(m_chain.at(i));
        }
    }
    return mp_temporary;
}


/****************************************************************************
 * @function name: Register::setSub()
 * @param:
 *      bool on - on sub mode
 * @description: setup register as sub register.
 *                   !!! sub register doesnot care about  bits on delete
 * @return: ( void )
 ****************************************************************************/
void Register::setSub(bool on)
{
    m_isSub = on;
}


/****************************************************************************
 * @function name: Register::isSub()
 * @description: returns true if register is sub register
 * @return: ( bool )
 ****************************************************************************/
bool Register::isSub() const
{
    return m_isSub;
}



/****************************************************************************
 * @function name: Register::isSame()
 * @param:
 *      Register *preg - pointer to register compares with current
 * @description: returns true if register is same.
 *      Same means when register has same bit names and size
 * @return: ( bool ) true - is similar
 ****************************************************************************/
bool Register::isSame(Register *preg)
{
    bool result = false;
    if(preg && preg->size() == this->size())
    {
        for (qint32 i = 0; i < m_chain.size(); i++)
        {
            Bit *pbit1 = m_chain.at(i);
            Bit *pbit2 = preg->bitAt(i);
            result = (pbit1->name() == pbit2->name() );
            if(result == false) break;
        }
    }
    return result;
}

/****************************************************************************
 * @function name: Register::isEmpty()
 * @param:
 * @description: register is empty . has no bits in it
 * @return: ( bool )
 ****************************************************************************/
bool Register::isEmpty() const
{
    return m_chain.size()==0;
}


/****************************************************************************
 * @function name: Register::scaleBitArray()
 * @param:
 *          const QByteArray &data_in
 *          qint32 factor - scale factor
 * @description: this function scales bit array
 *              Example: input 10011010, factor = 2
 *                      output 1100001111001100
 * @return: ( QByteArray )  output array
 ****************************************************************************/
QByteArray Register::scaleBitArray(const QByteArray &data_in, qint32 factor)
{
    QByteArray result;
    if(factor>0)
    {
        for(int i=0;i<data_in.size();i++)
        {
            char bit = data_in.at(0);
            for (int j=0;j<factor;j++) result.append(bit);
        }
    }
    return result;
}

/****************************************************************************
 * @function name: Register::scaleByteArray()
 * @param:
 *          const QByteArray &data_in
 *          qint32 factor - scale factor
 * @description: this function scales byte array by bit
 *              Example: input bytes hex : f0 01  , factor = 2
 *                      output bytes: ff 00 00 03
 *                      (in this example function will double each bit and return
 *                          it as byte array. SizeOut = SizeIn*factor )
 * @return: ( QByteArray ) result array
 ****************************************************************************/
QByteArray Register::scaleByteArray(const QByteArray &data_in,  qint32 factor)
{
    QByteArray result;
    if(factor>0)
    {
        result.resize(data_in.size()*factor);
        int j=0,i=0;
        quint8 mask1,mask2;
        quint8 byte;
        while(j<(data_in.size()*8*factor))
        {
            i += (j%factor)==0 && j>0;
            mask1 = 1<<(7-i%8);
            mask2 = 1<<(7-j%8);
            byte = result[j/8];
            if((quint8)data_in.at(i/8) &mask1)
                byte|= mask2;
            else
                byte &= ~mask2;
            result[j/8] = byte;
            j++;
        }
    }
    return result;
}

QByteArray Register::convertByteArrayToBitArray(const QByteArray &data_in, qint32 size_in_bits,BitOrder bitorder )
{
    QByteArray result;
    result.resize(size_in_bits);
    qint32 bit_i = 0;
    qint32 byte_i;
    quint8 byte = data_in[0];

    if (data_in.size() * 8 >= size_in_bits)
    {
        for (qint32 i = 0; i < size_in_bits; i++)
        {
            byte_i = i / 8;
            bit_i = i % 8;
            byte = data_in[byte_i];
            quint8 bit = (byte >> bit_i) & 1;
            if(bitorder == LSB)
            {
                result[i] = bit?'1':'0';
            }
            else
            {
                result[size_in_bits-1-i]=bit?'1':'0';
            }
        }
    }
    return result;
}

QByteArray Register::convertBitArrayToByteArray(const QByteArray &data_in,BitOrder bitorder ,bool contiguously)
{
    QByteArray result;
    qint32 size_in_bits = data_in.size();
    quint8 byte = 0;
    for (qint32 byte_i = 0; byte_i < size_in_bits; )
    {
        quint8 bit;
        qint32 index = byte_i;
        if(bitorder == MSB)
        {
            index = size_in_bits - 1 - byte_i;
        }
        bit = (data_in[index]=='1');
        if(contiguously)
        {
            byte |= (bit<< (7-(byte_i % 8)));
        }
        else{
            byte |= (bit<<    (byte_i % 8));
        }
        byte_i++;
        if ( ((byte_i % 8) == 0) || (byte_i >= size_in_bits) )
        {
            result.append(byte);
            byte = 0;
        }
    }
    return result;
}

quint32 Register::crc(Register::Crc crc_method, quint32 init, quint32 poly, qint32 from, qint32 to)
{
    //alter for bits
    switch(crc_method)
    {

    case CRC8_MSB:
        return crc8(init, poly, (quint8*)sub(from,to)->toByteArray().constData()
                    , sub(from,to)->toByteArray().size(),CRC_MSB);
        break;

    case CRC8_LSB:
        return crc8(init, poly, (quint8*)sub(from,to)->toByteArray().constData()
                    , sub(from,to)->toByteArray().size(),CRC_LSB);
        break;

    case CRC16:
        return crc16(poly, (quint16*)sub(from,to)->toByteArray().constData()
                     , sub(from,to)->toByteArray().size());
        break;
    }
    return 0;
}




void Register::makeSetup(const QString &init_script)
{
    m_setup_script = init_script;
    addBits(init_script);
}

const QString Register::getSetup()
{
    return m_setup_script;
}

void Register::setExtra(const QString &name, const QString &value)
{
    m_extra[name] = value;
}

QStringList Register::extras() const
{
    return m_extra.keys();
}

void Register::applyValueFromExtra(const QString &extra_name)
{
    QStringList items = toString("@purename;").split(';');
    foreach(const QString &item, items)
    {
        Register *ptmp = sub(item);
        if(ptmp->size()>0  )
        {
            sub(item)->setUInt(ptmp->bitAt(0)->extra(extra_name).toUInt());
        }
    }
    emit signal_updateSet(name());
}

void Register::bind(QObject *pobj, const char *Set, const char *Get)
{
    connect(this,SIGNAL(signal_updateSet(QString)),pobj,Set);
    connect(this,SIGNAL(signal_updateGet(QString)),pobj,Get);
    if(mp_temporary == NULL){
        makeTemporary();        
    }
}

void Register::setUpdatePolicy(Register::UpdatePolicy upd_pol)
{
    m_update_policy = upd_pol;
}

void Register::setParentRegister(Register *preg)
{
    mp_parent = preg;
}

Register::UpdatePolicy Register::updatePolicy() const
{
    if(isSub()) return mp_parent->updatePolicy();
    else return m_update_policy;
}

QString Register::extra(const QString &name)
{
    if(m_extra.contains(name))
    {
        return  m_extra[name];
    }
    return QString();
}


Register *Register::roll()
{
    QByteArray tmp = toBitString();
    this->fromBitString(tmp,Register::MSB);
    emit signal_updateSet(name());
    return this;
}

Register::operator int()
{
    emit signal_updateGet(name());
    return toUInt();
}

Register::operator unsigned int()
{
    emit signal_updateGet(name());
    return toUInt();
}


void Register::makeTemporary()
{
    mp_temporary =  new Register;
    mp_temporary->setSub(1);    
    mp_temporary->setName(name());
    mp_temporary->setParentRegister(this);
}

void Register::addTemporary(Bit *pbit)
{
    if(mp_temporary)
        mp_temporary->addBit(pbit);
}

void Register::clearTemporary()
{
    mp_temporary->clear();
}


void Register::removeBitByName(const QString &name)
{
    removeBit(bit(name.toLatin1().constData()));
}
