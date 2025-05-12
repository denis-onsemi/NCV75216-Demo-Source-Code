
INCLUDEPATH += $$PWD \
               $$PWD/controls \
               $$PWD/dialogs



SOURCES += \
    $$PWD/translator.cpp \
    $$PWD/main.cpp \	
    $$PWD/register.cpp \
    $$PWD/exceldata.cpp \
    $$PWD/serialportchannel.cpp \
    $$PWD/logs.cpp \
    $$PWD/dialogs/customdialogs.cpp \
    $$PWD/bitfieldparser.cpp \
    $$PWD/applet.cpp \    
    $$PWD/localserver.cpp \
    $$PWD/bit.cpp \
    $$PWD/variable.cpp \
    $$PWD/beeper.cpp \
    $$PWD/hamming.cpp \
    $$PWD/parameterparser.cpp \
    $$PWD/crc.cpp \
    $$PWD/applethelper.cpp\
    $$PWD/circularbuffer.cpp


    

HEADERS += \
    $$PWD/translator.h \
    $$PWD/register.h \
    $$PWD/exceldata.h \
    $$PWD/serialportchannel.h \
    $$PWD/logs.h \
    $$PWD/dialogs/customdialogs.h \      
    $$PWD/bitfieldparser.h \
    $$PWD/applet.h \
    $$PWD/localserver.h \
    $$PWD/bit.h \
    $$PWD/variable.h \ 
    $$PWD/beeper.h \
    $$PWD/hamming.h \
    $$PWD/compiler.h \
    $$PWD/parameterparser.h \
    $$PWD/tools.hpp \
    $$PWD/crc.h \
    $$PWD/applethelper.h\
    $$PWD/circularbuffer.h


    




RESOURCES += \
    $$PWD/generic.qrc

OTHER_FILES += \
    $$PWD/css/stylesheet.css

include(controls/customcontrols.pri)

FORMS +=


