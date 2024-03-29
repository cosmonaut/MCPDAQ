QT       += core gui network
#opengl

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets printsupport

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0
#DEFINES += QCUSTOMPLOT_USE_OPENGL
#LIBS += -lOpenGL32

SOURCES += \
    MCPDAQutil.cpp \
    main.cpp \
    mainwindow.cpp \
    mcpdaq_countplot.cpp \
    mcpdaq_data.cpp \
    mcpdaq_implot.cpp \
    mcpdaq_phdplot.cpp \
    mcpdaqetherdaqdialog.cpp \
    mcpdaqetherdaqiface.cpp \
    qcustomplot.cpp

HEADERS += \
    MCPDAQutil.h \
    mainwindow.h \
    mcpdaq_countplot.h \
    mcpdaq_data.h \
    mcpdaq_implot.h \
    mcpdaq_phdplot.h \
    mcpdaq_types.h \
    mcpdaqetherdaqdialog.h \
    mcpdaqetherdaqiface.h \
    qcustomplot.h \
    tinycolormap.hpp

FORMS += \
    mainwindow.ui \
    mcpdaqetherdaqdialog.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
