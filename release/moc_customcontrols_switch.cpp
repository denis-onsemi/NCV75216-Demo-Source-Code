/****************************************************************************
** Meta object code from reading C++ file 'customcontrols_switch.h'
**
** Created by: The Qt Meta Object Compiler version 63 (Qt 4.8.6)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../bench/generic/controls/customcontrols_switch.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'customcontrols_switch.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.6. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_CustomControls__Switch[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
       0,    0, // methods
       5,   14, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // properties: name, type, flags
      29,   23, 0x0009500b,
      39,   35, 0x02095103,
      59,   52, 0x06095103,
      70,   65, 0x01095103,
      80,   65, 0x01095103,

 // enums: name, flags, count, data

 // enum data: key, value

       0        // eod
};

static const char qt_meta_stringdata_CustomControls__Switch[] = {
    "CustomControls::Switch\0Model\0model\0"
    "int\0currentState\0double\0angle\0bool\0"
    "mirroredX\0mirroredY\0"
};

void CustomControls::Switch::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    Q_UNUSED(_o);
    Q_UNUSED(_id);
    Q_UNUSED(_c);
    Q_UNUSED(_a);
}

const QMetaObjectExtraData CustomControls::Switch::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject CustomControls::Switch::staticMetaObject = {
    { &MySwitch::staticMetaObject, qt_meta_stringdata_CustomControls__Switch,
      qt_meta_data_CustomControls__Switch, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &CustomControls::Switch::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *CustomControls::Switch::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *CustomControls::Switch::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_CustomControls__Switch))
        return static_cast<void*>(const_cast< Switch*>(this));
    if (!strcmp(_clname, "UniversalControl"))
        return static_cast< UniversalControl*>(const_cast< Switch*>(this));
    return MySwitch::qt_metacast(_clname);
}

int CustomControls::Switch::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = MySwitch::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    
#ifndef QT_NO_PROPERTIES
     if (_c == QMetaObject::ReadProperty) {
        void *_v = _a[0];
        switch (_id) {
        case 0: *reinterpret_cast< Model*>(_v) = switchModel(); break;
        case 1: *reinterpret_cast< int*>(_v) = state(); break;
        case 2: *reinterpret_cast< double*>(_v) = angle(); break;
        case 3: *reinterpret_cast< bool*>(_v) = mirroredX(); break;
        case 4: *reinterpret_cast< bool*>(_v) = mirroredY(); break;
        }
        _id -= 5;
    } else if (_c == QMetaObject::WriteProperty) {
        void *_v = _a[0];
        switch (_id) {
        case 0: setSwitchModel(*reinterpret_cast< Model*>(_v)); break;
        case 1: setCurrentState(*reinterpret_cast< int*>(_v)); break;
        case 2: setAngle(*reinterpret_cast< double*>(_v)); break;
        case 3: setMirroredX(*reinterpret_cast< bool*>(_v)); break;
        case 4: setMirroredY(*reinterpret_cast< bool*>(_v)); break;
        }
        _id -= 5;
    } else if (_c == QMetaObject::ResetProperty) {
        _id -= 5;
    } else if (_c == QMetaObject::QueryPropertyDesignable) {
        _id -= 5;
    } else if (_c == QMetaObject::QueryPropertyScriptable) {
        _id -= 5;
    } else if (_c == QMetaObject::QueryPropertyStored) {
        _id -= 5;
    } else if (_c == QMetaObject::QueryPropertyEditable) {
        _id -= 5;
    } else if (_c == QMetaObject::QueryPropertyUser) {
        _id -= 5;
    }
#endif // QT_NO_PROPERTIES
    return _id;
}
QT_END_MOC_NAMESPACE
