TEMPLATE = app
QT += gui network

# customer build
contains(DEFINES, CUSTOMER) {
 TARGET= $$TARGET"_EVAL"
}

CONFIG(debug, debug|release){
    UI_DIR += debug    
    LIBS += -lqaxcontainerd #ActiveX
    contains(DEFINES, ACTIVEX){
        message(DEBUG:Using ActiveX)
        CONFIG  += qaxserver   #ActiveX
    }
    TARGET = $$TARGET"d"
    DEFINES+= DEBUG_MODE
}
CONFIG(release, debug|release){    
    UI_DIR += release    
    LIBS += -lqaxcontainer #ActiveX
    contains(DEFINES, ACTIVEX) {
        message(RELEASE: Using ActiveX)
        CONFIG  += qaxserver   #ActiveX
    }
    RC_FILE  = bench.rc #Rc with On semi Icon
}


DEFINES += QT_NO_WHATSTHIS BENCHBOARD_SOFTWARE
#do not generate whatsThis code in ui_* file

win32-g++ {
    message("MINGW G++")
    DESTDIR = $$PROJECTPATH/bin_mingw
}
win32-msvc*{
    message("MSVC C++")
    DESTDIR = $$PROJECTPATH/bin
}



# we use different uic
# because I don't need conenectSlotsByName() function
QMAKE_UIC = $$PWD/tools/uic.exe


# include BENCH and PROJECT paths
INCLUDEPATH += $$BENCHPATH \
               $$PROJECTPATH

# QMAKE_TARGET is a project name 
DEFINES += QMAKE_TARGET=\\\"$$TARGET\\\"
DEFINES += FIRMWARE_FILE_NAME=\\\"$$FIRMWARE\\\"


INSTALLS += target sources
target.path = $$PROJECTPATH/bin
sources.files = $$SOURCES $$HEADERS $$RESOURCES $$FORMS bench.pri
sources.path = $$PROJECTPATH/bin

#ftdi chip module
contains(DEFINES, USE_FTDI) {
   message(Use FTDI)
    include ($$PWD/generic/ftdi/ftdi.pri)
}

# control board
contains(DEFINES,USE_CONTROLLER) {
  message(Use Control board)
    include ($$PWD/generic/controlboard/controlboard.pri)
   SOURCES += $$BENCHPATH/bootloader.cpp
   HEADERS += $$BENCHPATH/bootloader.h
}

# control board
contains(DEFINES,USE_MPP) {
  message(Use MPP board)
    include ($$PWD/generic/mpp/mppboard.pri)
}

# generic functionality
include( generic/generic.pri )

#use json
contains(DEFINES, USE_JSON){
    include(json/json.pri)
}

contains(DEFINES,USE_VEC_INTERPRETER){
message(Use vecInterepreter)
    SOURCES += \
        $$PWD/vec_interpreter.cpp
    HEADERS += \
        $$PWD/vec_interpreter.h
    FORMS += \
        $$PWD/vec_interpreter.ui
}


contains(DEFINES,USE_SCANCHAIN){
message(Use Scanchain)
    SOURCES += \
        $$PWD/scan_chain.cpp
    HEADERS += \
        $$PWD/scan_chain.h
    FORMS += \
        $$PWD/scan_chain.ui
}


contains(DEFINES, ACTIVEX) {
    HEADERS +=     $$BENCHPATH/axconnector.h
    SOURCES +=     $$BENCHPATH/axconnector.cpp
}

#BenchBoard specific
# Input
SOURCES += \    
    $$BENCHPATH/mainapp.cpp   \
    $$BENCHPATH/mainwindow.cpp \
    $$BENCHPATH/settings.cpp

HEADERS += \
    $$BENCHPATH/mainapp.h \
    $$BENCHPATH/mainwindow.h \
    $$BENCHPATH/settings.h


RESOURCES += \
    $$BENCHPATH/bench.qrc

FORMS += \
    bench/settings.ui


