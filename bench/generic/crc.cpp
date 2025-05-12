#include "crc.h"


quint8 crc8(quint32 init,quint32 poly, quint8 *data, qint32 len ,CrcDir crc_dir)
{
    int j,i;
    quint8 crc  = init;

    if(crc_dir == CRC_MSB)
    {
        for (i = 0; i < len; i++)
        {
            crc ^= data[i];
            for (j = 0; j < 8; j++)
            {
                if (crc & 0x80)
                    crc = (crc << 1) ^ poly;
                else
                    crc <<= 1;
            }
        }
    }
    else if(crc_dir==CRC_LSB)
    {
        for (i = 0; i < len; i++)
        {
            crc ^= data[i];
            for (j = 0; j < 8; j++)
            {
                if (crc & 0x1)
                    crc = (crc >> 1) ^ poly;
                else
                    crc >>= 1;
            }
        }
    }
    return crc;
}

quint16 crc16(quint32  poly, quint16 *data, qint32 len)
{
    int j,i;
    quint16 crc  = 0x000;
    for (i = 0; i < len; i++)
    {
        crc ^= data[i];
        for (j = 0; j < 16; j++)
        {
            if (crc & 0x80)
                crc = (crc << 1) ^ poly;
            else
                crc <<= 1;
        }
    }
    return crc;
}

//char *MakeCRC(char *BitString)
//   {
//   static char Res[9];                                 // CRC Result
//   char CRC[8];
//   int  i;
//   char DoInvert;

//   for (i=0; i<8; ++i)  CRC[i] = 0;                    // Init before calculation

//   for (i=0; i<strlen(BitString); ++i)
//      {
//      DoInvert = ('1'==BitString[i]) ^ CRC[7];         // XOR required?

//      CRC[7] = CRC[6] ^ DoInvert;
//      CRC[6] = CRC[5];
//      CRC[5] = CRC[4];
//      CRC[4] = CRC[3] ^ DoInvert;
//      CRC[3] = CRC[2] ^ DoInvert;
//      CRC[2] = CRC[1];
//      CRC[1] = CRC[0] ^ DoInvert;
//      CRC[0] = DoInvert;
//      }

//   for (i=0; i<8; ++i)  Res[7-i] = CRC[i] ? '1' : '0'; // Convert binary to ASCII
//   Res[8] = 0;                                         // Set string terminator

//   return(Res);
//   }
quint32 calcEcc(quint32 data, const quint32 *masks,quint32 size)
{
  quint8 ecc=0;
  for(quint32 i=0;i<size;i++)
  {
      ecc &= ~(1<<i) ;//clear
      bool bit =false;
      if(masks[i]==0) break;
      for(quint32 j=0;j<size;j++)
      {
          if(masks[i] & (1<<j))
          {
              bit ^=  (data &(1<<j)) !=0;
          }
      }
      ecc |= (bit<<i);
  }
  return ecc;
}
