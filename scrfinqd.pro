QT += core network
QT -= gui

CONFIG += c++11

TARGET = scrfinqd
CONFIG += console
CONFIG -= app_bundle

TEMPLATE = app

SOURCES += main.cpp \
    worker.cpp

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

HEADERS += \
    worker.h

DISTFILES += \
    config.txt \
    out.example/NVTK.MICEX_ts_1546498800000-1547814060000.json \
    out.example/NVTK.MICEX_ts_1547814060000-1549271220000.json \
    out.example/NVTK.MICEX_ts_1549271220000-1550574960000.json \
    out.example/NVTK.MICEX_ts_1550574960000-1551884940000.json \
    out.example/NVTK.MICEX_ts_1551884940000-1553520900000.json \
    out.example/NVTK.MICEX_ts_1553520900000-1554983400000.json \
    out.example/NVTK.MICEX_ts_1554983400000-1556533140000.json \
    out.example/NVTK.MICEX_ts_1556533140000-1558020060000.json \
    out.example/NVTK.MICEX_ts_1558020060000-1559550780000.json \
    out.example/NVTK.MICEX_ts_1559550780000-1560933420000.json \
    out.example/NVTK.MICEX_ts_1560933420000-1562226720000.json \
    out.example/NVTK.MICEX_ts_1562226720000-1563781080000.json \
    out.example/NVTK.MICEX_ts_1563781080000-1565102100000.json \
    out.example/NVTK.MICEX_ts_1565102100000-1566462180000.json \
    out.example/NVTK.MICEX_ts_1566462180000-1568038800000.json \
    out.example/NVTK.MICEX_ts_1568038800000-1569415200000.json \
    out.example/NVTK.MICEX_ts_1569415200000-1570794420000.json \
    out.example/NVTK.MICEX_ts_1570794420000-1572428400000.json \
    out.example/NVTK.MICEX_ts_1572428400000-1574073240000.json \
    out.example/NVTK.MICEX_ts_1574073240000-1575462180000.json \
    out.example/NVTK.MICEX_ts_1575462180000-1576845420000.json \
    out.example/NVTK.MICEX_ts_1576845420000-1578991740000.json \
    out.example/NVTK.MICEX_ts_1578991740000-1579493962000.json
