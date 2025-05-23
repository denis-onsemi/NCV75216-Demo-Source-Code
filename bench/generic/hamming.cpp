#include "hamming.h"

enum {PARITY_REMOVE,PARITY_ADD};

static quint32 ham_parity(quint32 value, quint32 size,quint32 operation);
static bool ham_read_parity(quint32 value, quint32 size, quint32 bit_number);
static quint8 parity_height[10];
static quint32 parity_count=0;

quint32 ham_populate_parity(quint32 size)
{
    parity_count = 0;
    for(quint32 i=1; i<size; i *= 2)
    {
        parity_height[parity_count] = i;
        parity_count++;
    }
    return parity_count;
}

quint32 ham_encode(quint32 value,quint32 size)
{
    quint32 result=0;
    result = ham_parity(value,size,PARITY_ADD);
    ham_populate_parity(size);
    for(quint32 i=0;i<parity_count;i++)
    {
        bool parity = ham_read_parity(result, size, i);
        result |= parity<<(parity_height[i]-1);
    }
    return result;
}

bool isPowOf2(quint32 value)
{
    return (value & (~(value-1))) == value;
}

quint32 ham_parity(quint32 value, quint32 size, quint32 operation)
{
    quint32 result =0;
    int j=0;
    for(quint32 i=1;i<=size;i++)
    {
        if( !isPowOf2(i) )
        {
            if(operation == PARITY_ADD)
            {
                quint32 mask  = 1<<j;
                result |= ( (value & mask ) !=0) <<(i-1);
            }
            else
            {
                quint32 mask = 1<<(i-1);
                result |= (( value& mask)!=0) <<j;
            }
            j++;
        }
    }
    return result;
}

bool ham_read_parity(quint32 value,quint32 size,quint32 bit_number )
{
    quint32 result=0;
    quint32 bit=1;
    while(bit_number--) bit *=2;
    bool include = false;
    for(quint32 i=bit;i<=size;i++)
    {
        include ^= (i%bit)==0;
        if(include && i!=bit)
        {
            quint32 mask = 1<<(i-1);
            result += (value & mask)!=0;
        }
    }
    return (result&1)!=0;
}


bool ham_check(quint32 value,quint32 size)
{
    ham_populate_parity(size);
    for(quint32 i=0;i<parity_count;i++)
    {
        bool parity1 = ham_read_parity(value, size, i);
        quint32 mask = 1<<(parity_height[i]-1);
        bool parity2 = (value & mask) !=0;
        if(parity1  != parity2)
            return false;
    }
    return true;
}

bool ham_decode(quint32 value, quint32 *presult, quint32 size)
{
    bool result = ham_check(value,size);
    *presult = ham_parity(value,size,PARITY_REMOVE);
    return result;
}

bool ham_check_secded(quint32 value, quint32 size )
{
    quint32 sum = 0;
    if(size <= 32)
    {
        ham_populate_parity(size);
        for(quint32 i=0;i<parity_count;i++)
        {
            bool parity = (value & (1<<(parity_height[i]-1))) !=0;
            sum += (quint32)parity;
        }
    }
    return ((sum&1) != 0 ) == ((value &(1<<size))!=0);
}
#include <QDebug>
 quint32 ham_add_secded( quint32 value , quint32 size)
 {
     quint32 sum = 0;
     if(size <= 32)
     {         
         for(quint32 i=0;i<size;i++)
         {             
             sum += (quint32)((value&(1<<i))!=0);
         }
     }
     return value | ((sum&1)<<size);
 }

bool ham_fix(quint32 value_in, quint32 *pvalue_out,quint32 size)
{
    quint32 wrong_bit_pos=0;
    ham_populate_parity(size);
    for(quint32 i=0;i<parity_count;i++)
    {
        bool parity1 = ham_read_parity(value_in, size, i);
        bool parity2 = (value_in & (1<<(parity_height[i]-1))) !=0;
        if(parity1  != parity2)
        {
            wrong_bit_pos += parity_height[i];
        }
    }
    if( wrong_bit_pos <= size)
    {
        quint32 mask = 1<<(wrong_bit_pos-1);
        *pvalue_out = value_in ^ mask;
        return true;
    }
    return false;
}

