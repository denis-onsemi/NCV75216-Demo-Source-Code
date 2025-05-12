#ifndef CRC_H
#define CRC_H
#include "QTypeInfo"
enum CrcDir{CRC_LSB,CRC_MSB};
quint8 crc8(quint32 init, quint32  poly, quint8 *data, qint32 len,CrcDir crc_dir);
quint16 crc16(quint32  poly, quint16 *data, qint32 len);
quint32 calcEcc(quint32 data, const quint32 *masks,quint32 size);

#endif // CRC_H
