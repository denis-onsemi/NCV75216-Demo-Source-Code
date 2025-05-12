/****************************************************************************
** Meta object code from reading C++ file 'myswitch.h'
**
** Created by: The Qt Meta Object Compiler version 63 (Qt 4.8.6)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../bench/generic/controls/myswitch.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'myswitch.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.6. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_CustomControls__MySwitch[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
       3,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       2,       // signalCount

 // signals: signature, parameters, type, tag, flags
      26,   25,   25,   25, 0x05,
      54,   25,   25,   25, 0x05,

 // slots: signature, parameters, type, tag, flags
      77,   71,   25,   25, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_CustomControls__MySwitch[] = {
    "CustomControls::MySwitch\0\0"
    "signal_StateChanged(qint32)\0"
    "signal_clicked()\0state\0setCurrentState(qint32)\0"
};

void CustomControls::MySwitch::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        MySwitch *_t = static_cast<MySwitch *>(_o);
        switch (_id) {
        case 0: _t->signal_StateChanged((*reinterpret_cast< qint32(*)>(_a[1]))); break;
        case 1: _t->signal_clicked(); break;
        case 2: _t->setCurrentState((*reinterpret_cast< qint32(*)>(_a[1]))); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData CustomControls::MySwitch::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject CustomControls::MySwitch::staticMetaObject = {
    { &QLabel::staticMetaObject, qt_meta_stringdata_CustomControls__MySwitch,
      qt_meta_data_CustomControls__MySwitch, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &CustomControls::MySwitch::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *CustomControls::MySwitch::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *CustomControls::MySwitch::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_CustomControls__MySwitch))
        return static_cast<void*>(const_cast< MySwitch*>(this));
    return QLabel::qt_metacast(_clname);
}

int CustomControls::MySwitch::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QLabel::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 3)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 3;
    }
    return _id;
}

// SIGNAL 0
void CustomControls::MySwitch::signal_StateChanged(qint32 _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void CustomControls::MySwitch::signal_clicked()
{
    QMetaObject::activate(this, &staticMetaObject, 1, 0);
}
static const uint qt_meta_data_CustomControls__MySwitchSet[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
       1,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: signature, parameters, type, tag, flags
      29,   28,   28,   28, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_CustomControls__MySwitchSet[] = {
    "CustomControls::MySwitchSet\0\0"
    "slot_SwitchClick()\0"
};

void CustomControls::MySwitchSet::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        MySwitchSet *_t = static_cast<MySwitchSet *>(_o);
        switch (_id) {
        case 0: _t->slot_SwitchClick(); break;
        default: ;
        }
    }
    Q_UNUSED(_a);
}

const QMetaObjectExtraData CustomControls::MySwitchSet::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject CustomControls::MySwitchSet::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_CustomControls__MySwitchSet,
      qt_meta_data_CustomControls__MySwitchSet, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &CustomControls::MySwitchSet::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *CustomControls::MySwitchSet::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *CustomControls::MySwitchSet::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_CustomControls__MySwitchSet))
        return static_cast<void*>(const_cast< MySwitchSet*>(this));
    return QObject::qt_metacast(_clname);
}

int CustomControls::MySwitchSet::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 1)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 1;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
