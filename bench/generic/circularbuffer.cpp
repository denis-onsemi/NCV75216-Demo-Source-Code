#include "circularbuffer.h"

CircularBuffer::CircularBuffer(qint32 capacity)  
{
    m_begin = 0;
    m_end = 0;
    m_size = 0;
    m_capacity = capacity;
    mp_data = new char[capacity];
}

CircularBuffer::~CircularBuffer()
{
    delete [] mp_data;
}

void CircularBuffer::reset()
{
    m_begin = 0;
    m_end = 0;
    m_size = 0;
}

qint32 CircularBuffer::write(const QByteArray & data)
{
    return write(data.constData(),data.size());
}

QByteArray CircularBuffer::read(qint32 bytes)
{
    QByteArray result;
    char tmp;
    qint32 count = (bytes < m_size)?bytes:m_size;
    while( count-- && read(&tmp,1) )
    {
        result.append(tmp);
    }
    return result;
}

QByteArray CircularBuffer::readLine(bool *ok)
{
    int i=0;
    int j=m_begin;
    while(i++ < m_size)
    {
        if(j >= m_capacity){        j=0;}
        if(mp_data[j] =='\n' || mp_data[j] =='\r')
        {
            if(ok) *ok=true;
            return read(i);
        }
        j++;
    }
    if(ok) *ok=false;
    return QByteArray();
}

QByteArray CircularBuffer::readAll()
{
    return read(m_size);
}

qint32 CircularBuffer::write(const char *data, qint32 bytes)
{
  if (bytes == 0) return 0;

  qint32 capacity = m_capacity;
  qint32 bytes_to_write =bytes<(capacity - m_size)?bytes:(capacity - m_size);

  // Write in a single step
  if (bytes_to_write <= (capacity - m_end) )
  {
    memcpy(mp_data + m_end, data, bytes_to_write);
    m_end += bytes_to_write;
    if (m_end == capacity) m_end = 0;
  }
  // Write in two steps
  else
  {
    qint32 size_1 = capacity - m_end;
    memcpy(mp_data + m_end, data, size_1);
    qint32 size_2 = bytes_to_write - size_1;
    memcpy(mp_data, data + size_1, size_2);
    m_end = size_2;
  }

  m_size += bytes_to_write;
  return bytes_to_write;
}

qint32 CircularBuffer::read(char *data, qint32 bytes)
{
  if (bytes == 0) return 0;

  qint32 capacity = m_capacity;
  qint32 bytes_to_read = (bytes<m_size)?bytes:m_size;

  // Read in a single step
  if (bytes_to_read <= (capacity - m_begin) )
  {
    memcpy(data, mp_data + m_begin, bytes_to_read);
    m_begin += bytes_to_read;
    if (m_begin == capacity) m_begin = 0;
  }
  // Read in two steps
  else
  {
    qint32 size_1 = capacity - m_begin;
    memcpy(data, mp_data + m_begin, size_1);
    qint32 size_2 = bytes_to_read - size_1;
    memcpy(data + size_1, mp_data, size_2);
    m_begin = size_2;
  }

  m_size -= bytes_to_read;
  return bytes_to_read;
}

