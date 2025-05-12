/****************************************************************************
** Meta object code from reading C++ file 'customcontrols_buttonbox.h'
**
** Created by: The Qt Meta Object Compiler version 63 (Qt 4.8.6)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../bench/generic/controls/customcontrols_buttonbox.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'customcontrols_buttonbox.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.6. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_CustomControls__ButtonBox[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
       2,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: signature, parameters, type, tag, flags
      27,   26,   26,   26, 0x05,

 // slots: signature, parameters, type, tag, flags
      44,   26,   26,   26, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_CustomControls__ButtonBox[] = {
    "CustomControls::ButtonBox\0\0signal_clicked()\0"
    "slot_on_click()\0"
};

void CustomControls::ButtonBox::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        ButtonBox *_t = static_cast<ButtonBox *>(_o);
        switch (_id) {
        case 0: _t->signal_clicked(); break;
        case 1: _t->slot_on_click(); break;
        default: ;
        }
    }
    Q_UNUSED(_a);
}

const QMetaObjectExtraData CustomControls::ButtonBox::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject CustomControls::ButtonBox::staticMetaObject = {
    { &QGroupBox::staticMetaObject, qt_meta_stringdata_CustomControls__ButtonBox,
      qt_meta_data_CustomControls__ButtonBox, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &CustomControls::ButtonBox::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *CustomControls::ButtonBox::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *CustomControls::ButtonBox::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_CustomControls__ButtonBox))
        return static_cast<void*>(const_cast< ButtonBox*>(this));
    if (!strcmp(_clname, "UniversalControl"))
        return static_cast< UniversalControl*>(const_cast< ButtonBox*>(this));
    return QGroupBox::qt_metacast(_clname);
}

int CustomControls::ButtonBox::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QGroupBox::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 2)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 2;
    }
    return _id;
}

// SIGNAL 0
void CustomControls::ButtonBox::signal_clicked()
{
    QMetaObject::activate(this, &staticMetaObject, 0, 0);
}
QT_END_MOC_NAMESPACE
