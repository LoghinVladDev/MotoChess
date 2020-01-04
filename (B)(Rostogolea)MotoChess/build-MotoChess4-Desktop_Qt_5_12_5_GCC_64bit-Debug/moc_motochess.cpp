/****************************************************************************
** Meta object code from reading C++ file 'motochess.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.12.5)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../MotoChess4/motochess.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'motochess.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.12.5. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_MotoChess_t {
    QByteArrayData data[8];
    char stringdata0[90];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_MotoChess_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_MotoChess_t qt_meta_stringdata_MotoChess = {
    {
QT_MOC_LITERAL(0, 0, 9), // "MotoChess"
QT_MOC_LITERAL(1, 10, 16), // "refreshUsersList"
QT_MOC_LITERAL(2, 27, 0), // ""
QT_MOC_LITERAL(3, 28, 19), // "buttonMatch_clicked"
QT_MOC_LITERAL(4, 48, 13), // "refresh_table"
QT_MOC_LITERAL(5, 62, 11), // "click_piesa"
QT_MOC_LITERAL(6, 74, 2), // "id"
QT_MOC_LITERAL(7, 77, 12) // "loginClicked"

    },
    "MotoChess\0refreshUsersList\0\0"
    "buttonMatch_clicked\0refresh_table\0"
    "click_piesa\0id\0loginClicked"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_MotoChess[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
       5,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    0,   39,    2, 0x08 /* Private */,
       3,    1,   40,    2, 0x08 /* Private */,
       4,    0,   43,    2, 0x08 /* Private */,
       5,    1,   44,    2, 0x08 /* Private */,
       7,    0,   47,    2, 0x08 /* Private */,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void, QMetaType::QString,    2,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int,    6,
    QMetaType::Void,

       0        // eod
};

void MotoChess::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<MotoChess *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->refreshUsersList(); break;
        case 1: _t->buttonMatch_clicked((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 2: _t->refresh_table(); break;
        case 3: _t->click_piesa((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 4: _t->loginClicked(); break;
        default: ;
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject MotoChess::staticMetaObject = { {
    &QMainWindow::staticMetaObject,
    qt_meta_stringdata_MotoChess.data,
    qt_meta_data_MotoChess,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *MotoChess::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *MotoChess::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_MotoChess.stringdata0))
        return static_cast<void*>(this);
    return QMainWindow::qt_metacast(_clname);
}

int MotoChess::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QMainWindow::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 5)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 5;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 5)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 5;
    }
    return _id;
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
