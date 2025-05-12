



# !!!   BUILDING  NEW  VERSION !!!
#************************************************************
# Please set prebuild command in projects setup
# chelper.py version
# => example "Custom Process Step: python.exe chelper.py version"
#************************************************************
# and post build command
# chelper.py copy
# => example "Custom Process Step: python.exe chelper.py copy"
#************************************************************










#############################################################
# PROJECT SPECIFIC DATA
#############################################################

TARGET = P215 #example P215,C516

#DEFINES += ACTIVEX
#DEFINES  += USE_FTDI
DEFINES  += USE_CONTROLLER
#DEFINES  += USE_VEC_INTERPRETER
#DEFINES += USE_SCANCHAIN


PROJECTPATH = $$PWD
BENCHPATH = bench
INCLUDEPATH += $$PROJECTPATH




#########USERS PART###############

# The project file name *.pro  doesn't strictly limited
# project_app.qrc - default project resource
# project_app.cpp - dafault project cpp file
# project_app.h -default project  header file


SOURCES += \    
    tmccontroller.cpp \
    measurement.cpp \
    triangulation.cpp


HEADERS += \        
    $$PWD/project_app.h \ 
    tmccontroller.h \
    board_def.h \
    measurement.h \
    triangulation.h


FORMS += \
    tmccontroller.ui \
    measurement.ui \
    triangulation.ui

RESOURCES += \
    $$PWD/project_app.qrc

include ($$BENCHPATH/bench.pri)

#########USERS PART###############

OTHER_FILES += \
    README.TXT \
    changes.txt
