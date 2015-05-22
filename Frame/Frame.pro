######################################################################
# Automatically generated by qmake (2.01a) Tue May 19 19:51:11 2015
######################################################################

TEMPLATE = app
TARGET = 
DEPENDPATH += . CodeEdit monitor Shell
INCLUDEPATH += . Shell CodeEdit monitor

# Input
HEADERS += \
           FrameWindow.h \
           CodeEdit/codeEdit.h \
           CodeEdit/codeWindow.h \
           CodeEdit/highlight.h \
           monitor/dataclass.h \
           monitor/monitor.h \
           Shell/shell.h \
           Shell/testShell.h \
    MyQtFile.h \
    filesystem/sfs_inode.h \
    filesystem/sfs.h \
    filesystem/mksfs.h \
    filesystem/init.h \
    filesystem/fileinterface.h \
    filesystem/file.h \
    filesystem/error.h \
    filesystem/dev.h \
    filesystem/blockbitmap.h
FORMS += CodeEdit/codeWindow.ui monitor/monitor.ui
SOURCES += \
           FrameWindow.cpp \
           main.cpp \
           CodeEdit/codeEdit.cpp \
           CodeEdit/codeWindow.cpp \
           CodeEdit/highlight.cpp \
           monitor/monitor.cpp \
           Shell/shell.cpp \
    MyQtFile.cpp \
    filesystem/sfs_inode.c \
    filesystem/sfs.c \
    filesystem/mksfs.c \
    filesystem/init.c \
    filesystem/fileinterface.c \
    filesystem/file.c \
    filesystem/dev.c \
    filesystem/ctrl_write.c \
    filesystem/ctrl_read.c \
    filesystem/ctrl_open.c \
    filesystem/ctrl_lseek.c \
    filesystem/ctrl_directory.c \
    filesystem/ctrl_delect.c \
    filesystem/ctrl_close.c \
    filesystem/blockbitmap.c
RESOURCES += source.qrc

OTHER_FILES +=
