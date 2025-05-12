/****************************************************************************
** Meta object code from reading C++ file 'controlboard.h'
**
** Created by: The Qt Meta Object Compiler version 63 (Qt 4.8.6)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../bench/generic/controlboard/controlboard.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'controlboard.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.6. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_ControlBoard[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
       7,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       7,       // signalCount

 // signals: signature, parameters, type, tag, flags
      16,   14,   13,   13, 0x05,
      40,   13,   13,   13, 0x05,
      52,   13,   13,   13, 0x05,
      64,   13,   13,   13, 0x05,
      84,   13,   13,   13, 0x05,
     106,   13,   13,   13, 0x05,
     125,   13,   13,   13, 0x05,

       0        // eod
};

static const char qt_meta_stringdata_ControlBoard[] = {
    "ControlBoard\0\0,\0signal_log(int,QString)\0"
    "signal_tx()\0signal_rx()\0signal_stream(bool)\0"
    "signal_disconnected()\0signal_connected()\0"
    "signal_captureData(QByteArray)\0"
};

void ControlBoard::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        ControlBoard *_t = static_cast<ControlBoard *>(_o);
        switch (_id) {
        case 0: _t->signal_log((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2]))); break;
        case 1: _t->signal_tx(); break;
        case 2: _t->signal_rx(); break;
        case 3: _t->signal_stream((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 4: _t->signal_disconnected(); break;
        case 5: _t->signal_connected(); break;
        case 6: _t->signal_captureData((*reinterpret_cast< QByteArray(*)>(_a[1]))); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData ControlBoard::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject ControlBoard::staticMetaObject = {
    { &QThread::staticMetaObject, qt_meta_stringdata_ControlBoard,
      qt_meta_data_ControlBoard, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &ControlBoard::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *ControlBoard::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *ControlBoard::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_ControlBoard))
        return static_cast<void*>(const_cast< ControlBoard*>(this));
    return QThread::qt_metacast(_clname);
}

int ControlBoard::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QThread::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 7)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 7;
    }
    return _id;
}

// SIGNAL 0
void ControlBoard::signal_log(int _t1, QString _t2)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void ControlBoard::signal_tx()
{
    QMetaObject::activate(this, &staticMetaObject, 1, 0);
}

// SIGNAL 2
void ControlBoard::signal_rx()
{
    QMetaObject::activate(this, &staticMetaObject, 2, 0);
}

// SIGNAL 3
void ControlBoard::signal_stream(bool _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 3, _a);
}

// SIGNAL 4
void ControlBoard::signal_disconnected()
{
    QMetaObject::activate(this, &staticMetaObject, 4, 0);
}

// SIGNAL 5
void ControlBoard::signal_connected()
{
    QMetaObject::activate(this, &staticMetaObject, 5, 0);
}

// SIGNAL 6
void ControlBoard::signal_captureData(QByteArray _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 6, _a);
}
QT_END_MOC_NAMESPACE
