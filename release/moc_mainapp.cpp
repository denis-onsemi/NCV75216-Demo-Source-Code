/****************************************************************************
** Meta object code from reading C++ file 'mainapp.h'
**
** Created by: The Qt Meta Object Compiler version 63 (Qt 4.8.6)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../bench/mainapp.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'mainapp.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.6. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_MainApp[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
       6,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: signature, parameters, type, tag, flags
      11,    9,    8,    8, 0x05,

 // slots: signature, parameters, type, tag, flags
      51,   40,   35,    8, 0x0a,
      92,   74,   35,    8, 0x0a,
     139,  115,   35,    8, 0x0a,
     172,    8,    8,    8, 0x0a,
     192,    8,    8,    8, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_MainApp[] = {
    "MainApp\0\0,\0signal_log(int,QString)\0"
    "bool\0name,value\0set(QString,QVariant&)\0"
    "name,pdata_result\0get(QString,QVariant*)\0"
    "name,value,pdata_result\0"
    "run(QString,QVariant&,QVariant*)\0"
    "slot_disconnected()\0slot_connected()\0"
};

void MainApp::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        MainApp *_t = static_cast<MainApp *>(_o);
        switch (_id) {
        case 0: _t->signal_log((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2]))); break;
        case 1: { bool _r = _t->set((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< QVariant(*)>(_a[2])));
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = _r; }  break;
        case 2: { bool _r = _t->get((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< QVariant*(*)>(_a[2])));
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = _r; }  break;
        case 3: { bool _r = _t->run((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< QVariant(*)>(_a[2])),(*reinterpret_cast< QVariant*(*)>(_a[3])));
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = _r; }  break;
        case 4: _t->slot_disconnected(); break;
        case 5: _t->slot_connected(); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData MainApp::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject MainApp::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_MainApp,
      qt_meta_data_MainApp, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &MainApp::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *MainApp::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *MainApp::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_MainApp))
        return static_cast<void*>(const_cast< MainApp*>(this));
    return QObject::qt_metacast(_clname);
}

int MainApp::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 6)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 6;
    }
    return _id;
}

// SIGNAL 0
void MainApp::signal_log(int _t1, QString _t2)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}
QT_END_MOC_NAMESPACE
