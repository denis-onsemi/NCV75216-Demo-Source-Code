/****************************************************************************
** Meta object code from reading C++ file 'customcontrols_lineedit.h'
**
** Created by: The Qt Meta Object Compiler version 63 (Qt 4.8.6)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../bench/generic/controls/customcontrols_lineedit.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'customcontrols_lineedit.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.6. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_CustomControls__LineEdit[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
       4,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: signature, parameters, type, tag, flags
      26,   25,   25,   25, 0x05,

 // slots: signature, parameters, type, tag, flags
      45,   25,   25,   25, 0x08,
      64,   25,   25,   25, 0x08,
      86,   25,   25,   25, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_CustomControls__LineEdit[] = {
    "CustomControls::LineEdit\0\0signal_activated()\0"
    "slot_TextChanged()\0slot_ActionMode(bool)\0"
    "slot_triggered()\0"
};

void CustomControls::LineEdit::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        LineEdit *_t = static_cast<LineEdit *>(_o);
        switch (_id) {
        case 0: _t->signal_activated(); break;
        case 1: _t->slot_TextChanged(); break;
        case 2: _t->slot_ActionMode((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 3: _t->slot_triggered(); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData CustomControls::LineEdit::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject CustomControls::LineEdit::staticMetaObject = {
    { &QLineEdit::staticMetaObject, qt_meta_stringdata_CustomControls__LineEdit,
      qt_meta_data_CustomControls__LineEdit, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &CustomControls::LineEdit::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *CustomControls::LineEdit::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *CustomControls::LineEdit::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_CustomControls__LineEdit))
        return static_cast<void*>(const_cast< LineEdit*>(this));
    if (!strcmp(_clname, "UniversalControl"))
        return static_cast< UniversalControl*>(const_cast< LineEdit*>(this));
    return QLineEdit::qt_metacast(_clname);
}

int CustomControls::LineEdit::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QLineEdit::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 4)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 4;
    }
    return _id;
}

// SIGNAL 0
void CustomControls::LineEdit::signal_activated()
{
    QMetaObject::activate(this, &staticMetaObject, 0, 0);
}
QT_END_MOC_NAMESPACE
