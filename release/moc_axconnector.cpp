/****************************************************************************
** Meta object code from reading C++ file 'axconnector.h'
**
** Created by: The Qt Meta Object Compiler version 63 (Qt 4.8.6)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../bench/axconnector.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'axconnector.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.6. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_Bench[] = {

 // content:
       6,       // revision
       0,       // classname
       3,   14, // classinfo
       7,   20, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // classinfo: key, value
      45,    6,
      92,   53,
     108,  104,

 // slots: signature, parameters, type, tag, flags
     124,  123,  123,  123, 0x0a,
     149,  136,  131,  123, 0x0a,
     180,  136,  171,  123, 0x0a,
     210,  202,  171,  123, 0x0a,
     232,  223,  131,  123, 0x0a,
     246,  123,  131,  123, 0x0a,
     270,  265,  254,  123, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_Bench[] = {
    "Bench\0{5533dc46-a6f2-45f8-a812-2a06bde4ccc7}\0"
    "ClassID\0{cd1e1629-2331-45cd-a880-325361981b84}\0"
    "InterfaceID\0yes\0RegisterObject\0\0quit()\0"
    "bool\0command,data\0set(QString,QVariant)\0"
    "QVariant\0run(QString,QVariant)\0command\0"
    "get(QString)\0portname\0open(QString)\0"
    "close()\0QByteArray\0data\0write(QByteArray)\0"
};

void Bench::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        Bench *_t = static_cast<Bench *>(_o);
        switch (_id) {
        case 0: _t->quit(); break;
        case 1: { bool _r = _t->set((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< QVariant(*)>(_a[2])));
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = _r; }  break;
        case 2: { QVariant _r = _t->run((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< QVariant(*)>(_a[2])));
            if (_a[0]) *reinterpret_cast< QVariant*>(_a[0]) = _r; }  break;
        case 3: { QVariant _r = _t->get((*reinterpret_cast< QString(*)>(_a[1])));
            if (_a[0]) *reinterpret_cast< QVariant*>(_a[0]) = _r; }  break;
        case 4: { bool _r = _t->open((*reinterpret_cast< QString(*)>(_a[1])));
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = _r; }  break;
        case 5: { bool _r = _t->close();
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = _r; }  break;
        case 6: { QByteArray _r = _t->write((*reinterpret_cast< QByteArray(*)>(_a[1])));
            if (_a[0]) *reinterpret_cast< QByteArray*>(_a[0]) = _r; }  break;
        default: ;
        }
    }
}

const QMetaObjectExtraData Bench::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject Bench::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_Bench,
      qt_meta_data_Bench, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &Bench::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *Bench::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *Bench::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_Bench))
        return static_cast<void*>(const_cast< Bench*>(this));
    return QObject::qt_metacast(_clname);
}

int Bench::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 7)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 7;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
