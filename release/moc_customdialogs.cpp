/****************************************************************************
** Meta object code from reading C++ file 'customdialogs.h'
**
** Created by: The Qt Meta Object Compiler version 63 (Qt 4.8.6)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../bench/generic/dialogs/customdialogs.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'customdialogs.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.6. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_DialogCheckBoxes[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
       2,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: signature, parameters, type, tag, flags
      18,   17,   17,   17, 0x08,
      34,   17,   17,   17, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_DialogCheckBoxes[] = {
    "DialogCheckBoxes\0\0slot_SetAllOn()\0"
    "slot_SetAllOff()\0"
};

void DialogCheckBoxes::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        DialogCheckBoxes *_t = static_cast<DialogCheckBoxes *>(_o);
        switch (_id) {
        case 0: _t->slot_SetAllOn(); break;
        case 1: _t->slot_SetAllOff(); break;
        default: ;
        }
    }
    Q_UNUSED(_a);
}

const QMetaObjectExtraData DialogCheckBoxes::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject DialogCheckBoxes::staticMetaObject = {
    { &QDialog::staticMetaObject, qt_meta_stringdata_DialogCheckBoxes,
      qt_meta_data_DialogCheckBoxes, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &DialogCheckBoxes::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *DialogCheckBoxes::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *DialogCheckBoxes::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_DialogCheckBoxes))
        return static_cast<void*>(const_cast< DialogCheckBoxes*>(this));
    return QDialog::qt_metacast(_clname);
}

int DialogCheckBoxes::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QDialog::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 2)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 2;
    }
    return _id;
}
static const uint qt_meta_data_DialogProgressBar[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
       0,    0, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

       0        // eod
};

static const char qt_meta_stringdata_DialogProgressBar[] = {
    "DialogProgressBar\0"
};

void DialogProgressBar::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    Q_UNUSED(_o);
    Q_UNUSED(_id);
    Q_UNUSED(_c);
    Q_UNUSED(_a);
}

const QMetaObjectExtraData DialogProgressBar::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject DialogProgressBar::staticMetaObject = {
    { &QDialog::staticMetaObject, qt_meta_stringdata_DialogProgressBar,
      qt_meta_data_DialogProgressBar, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &DialogProgressBar::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *DialogProgressBar::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *DialogProgressBar::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_DialogProgressBar))
        return static_cast<void*>(const_cast< DialogProgressBar*>(this));
    return QDialog::qt_metacast(_clname);
}

int DialogProgressBar::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QDialog::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    return _id;
}
QT_END_MOC_NAMESPACE
