/****************************************************************************
** Meta object code from reading C++ file 'applethelper.h'
**
** Created by: The Qt Meta Object Compiler version 63 (Qt 4.8.6)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../bench/generic/applethelper.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'applethelper.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.6. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_AppletHelper[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
      11,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: signature, parameters, type, tag, flags
      14,   13,   13,   13, 0x05,

 // slots: signature, parameters, type, tag, flags
      30,   13,   13,   13, 0x08,
      57,   13,   13,   13, 0x08,
      78,   13,   13,   13, 0x08,
     101,   13,   13,   13, 0x08,
     124,   13,   13,   13, 0x08,
     145,   13,   13,   13, 0x08,
     171,  166,   13,   13, 0x08,
     197,   13,   13,   13, 0x08,
     225,   13,   13,   13, 0x08,
     255,   13,   13,   13, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_AppletHelper[] = {
    "AppletHelper\0\0signal_update()\0"
    "slot_pbUpdateAll_clicked()\0"
    "slot_pbSet_clicked()\0slot_pbFill0_clicked()\0"
    "slot_pbFill1_clicked()\0slot_pbGet_clicked()\0"
    "slot_pbRun_clicked()\0text\0"
    "slot_FindInNodes(QString)\0"
    "slot_exportRegistersTo101()\0"
    "slot_importRegistersFrom101()\0"
    "slot_ApplyFile()\0"
};

void AppletHelper::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        AppletHelper *_t = static_cast<AppletHelper *>(_o);
        switch (_id) {
        case 0: _t->signal_update(); break;
        case 1: _t->slot_pbUpdateAll_clicked(); break;
        case 2: _t->slot_pbSet_clicked(); break;
        case 3: _t->slot_pbFill0_clicked(); break;
        case 4: _t->slot_pbFill1_clicked(); break;
        case 5: _t->slot_pbGet_clicked(); break;
        case 6: _t->slot_pbRun_clicked(); break;
        case 7: _t->slot_FindInNodes((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 8: _t->slot_exportRegistersTo101(); break;
        case 9: _t->slot_importRegistersFrom101(); break;
        case 10: _t->slot_ApplyFile(); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData AppletHelper::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject AppletHelper::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_AppletHelper,
      qt_meta_data_AppletHelper, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &AppletHelper::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *AppletHelper::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *AppletHelper::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_AppletHelper))
        return static_cast<void*>(const_cast< AppletHelper*>(this));
    return QWidget::qt_metacast(_clname);
}

int AppletHelper::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 11)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 11;
    }
    return _id;
}

// SIGNAL 0
void AppletHelper::signal_update()
{
    QMetaObject::activate(this, &staticMetaObject, 0, 0);
}
QT_END_MOC_NAMESPACE
