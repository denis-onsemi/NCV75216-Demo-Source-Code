/****************************************************************************
** Meta object code from reading C++ file 'mainwindow.h'
**
** Created by: The Qt Meta Object Compiler version 63 (Qt 4.8.6)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../bench/mainwindow.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'mainwindow.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.6. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_MainWindow[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
      22,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: signature, parameters, type, tag, flags
      12,   11,   11,   11, 0x0a,
      34,   11,   11,   11, 0x0a,
      56,   11,   11,   11, 0x0a,
      82,   11,   11,   11, 0x0a,
     100,   11,   11,   11, 0x0a,
     118,   11,   11,   11, 0x0a,
     136,   11,   11,   11, 0x0a,
     158,   11,   11,   11, 0x0a,
     177,   11,   11,   11, 0x08,
     209,   11,   11,   11, 0x08,
     237,   11,   11,   11, 0x08,
     251,   11,   11,   11, 0x08,
     274,   11,   11,   11, 0x08,
     290,   11,   11,   11, 0x08,
     317,   11,   11,   11, 0x08,
     339,   11,   11,   11, 0x08,
     355,   11,   11,   11, 0x08,
     387,  380,   11,   11, 0x08,
     416,   11,   11,   11, 0x08,
     445,   11,   11,   11, 0x08,
     462,   11,   11,   11, 0x08,
     485,   11,   11,   11, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_MainWindow[] = {
    "MainWindow\0\0slot_ActivityRx_LED()\0"
    "slot_ActivityTx_LED()\0slot_Connection_LED(bool)\0"
    "slot_stream(bool)\0slot_SaveConfig()\0"
    "slot_LoadConfig()\0slot_LoadConfigName()\0"
    "slot_supress_led()\0slot_RestorePreviousWorkspace()\0"
    "slot_pbConnectBoard_click()\0slot_Rescan()\0"
    "slot_LoadProjectData()\0slot_AboutApp()\0"
    "slot_UpdateBenchSoftware()\0"
    "slot_UpdateFirmware()\0slot_CloseAll()\0"
    "slot_CloseAllButActive()\0enable\0"
    "slot_cbLogComm_toggled(bool)\0"
    "slot_ChangeConenctionSpeed()\0"
    "slot_LogOn(bool)\0slot_pbDontSleep(bool)\0"
    "slot_timeout()\0"
};

void MainWindow::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        MainWindow *_t = static_cast<MainWindow *>(_o);
        switch (_id) {
        case 0: _t->slot_ActivityRx_LED(); break;
        case 1: _t->slot_ActivityTx_LED(); break;
        case 2: _t->slot_Connection_LED((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 3: _t->slot_stream((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 4: _t->slot_SaveConfig(); break;
        case 5: _t->slot_LoadConfig(); break;
        case 6: _t->slot_LoadConfigName(); break;
        case 7: _t->slot_supress_led(); break;
        case 8: _t->slot_RestorePreviousWorkspace(); break;
        case 9: _t->slot_pbConnectBoard_click(); break;
        case 10: _t->slot_Rescan(); break;
        case 11: _t->slot_LoadProjectData(); break;
        case 12: _t->slot_AboutApp(); break;
        case 13: _t->slot_UpdateBenchSoftware(); break;
        case 14: _t->slot_UpdateFirmware(); break;
        case 15: _t->slot_CloseAll(); break;
        case 16: _t->slot_CloseAllButActive(); break;
        case 17: _t->slot_cbLogComm_toggled((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 18: _t->slot_ChangeConenctionSpeed(); break;
        case 19: _t->slot_LogOn((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 20: _t->slot_pbDontSleep((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 21: _t->slot_timeout(); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData MainWindow::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject MainWindow::staticMetaObject = {
    { &QMainWindow::staticMetaObject, qt_meta_stringdata_MainWindow,
      qt_meta_data_MainWindow, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &MainWindow::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *MainWindow::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *MainWindow::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_MainWindow))
        return static_cast<void*>(const_cast< MainWindow*>(this));
    return QMainWindow::qt_metacast(_clname);
}

int MainWindow::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QMainWindow::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 22)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 22;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
