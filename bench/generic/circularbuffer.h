#ifndef CIRCULAR_BUFFER_H
#define CIRCULAR_BUFFER_H

#include <QTypeInfo>
#include <QByteArray>

class CircularBuffer
{
public:
  CircularBuffer(qint32 capacity);
  ~CircularBuffer();
  void reset();
  qint32 size() const { return m_size; }
  bool empty() const { return size()==0;}
  qint32 capacity() const { return m_capacity; }
  qint32 write(const char *data, qint32 bytes);
  qint32 write(const QByteArray & data);  
  qint32 read(char *data, qint32 bytes);
  QByteArray read(qint32 bytes);
  QByteArray readLine(bool *ok=0);
  QByteArray readAll();

private:
  qint32 m_begin;
  qint32 m_end;
  qint32 m_size;
  qint32 m_capacity;
  char *mp_data;
};
#endif 
