
#FTDI
LIBS += -L$$PWD

qtAddLibrary(ftd2xx)

INCLUDEPATH += $$PWD

HEADERS += $$PWD/ftdispichannel.h

SOURCES += $$PWD/ftdispichannel.cpp
