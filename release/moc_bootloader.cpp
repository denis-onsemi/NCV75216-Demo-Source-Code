/****************************************************************************
** Meta object code from reading C++ file 'bootloader.h'
**
** Created by: The Qt Meta Object Compiler version 63 (Qt 4.8.6)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../bench/bootloader.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'bootloader.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.6. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_Bootloader[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
       4,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: signature, parameters, type, tag, flags
      12,   11,   11,   11, 0x08,
      37,   11,   11,   11, 0x08,
      60,   11,   11,   11, 0x08,
      85,   11,   11,   11, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_Bootloader[] = {
    "Bootloader\0\0slot_pbLoadHex_clicked()\0"
    "slot_pbFlash_clicked()\0slot_pbDefault_clicked()\0"
    "slot_pbRestart_clicked()\0"
};

void Bootloader::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        Bootloader *_t = static_cast<Bootloader *>(_o);
        switch (_id) {
        case 0: _t->slot_pbLoadHex_clicked(); break;
        case 1: _t->slot_pbFlash_clicked(); break;
        case 2: _t->slot_pbDefault_clicked(); break;
        case 3: _t->slot_pbRestart_clicked(); break;
        default: ;
        }
    }
    Q_UNUSED(_a);
}

const QMetaObjectExtraData Bootloader::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject Bootloader::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_Bootloader,
      qt_meta_data_Bootloader, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &Bootloader::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *Bootloader::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *Bootloader::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_Bootloader))
        return static_cast<void*>(const_cast< Bootloader*>(this));
    return QWidget::qt_metacast(_clname);
}

int Bootloader::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 4)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 4;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
