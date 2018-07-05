#-------------------------------------------------
#
# Project created by QtCreator 2018-05-28T13:06:26
#
#-------------------------------------------------

QT       += core gui
QT       +=sql
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = databaseexp
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


SOURCES += \
        main.cpp \
        mainwindow.cpp \
    mydatabase.cpp \
    reader_login.cpp \
    admin_login.cpp \
    admin.cpp \
    borrow_book.cpp \
    insert_reader.cpp \
    change_book.cpp \
    borrow_status.cpp \
    borrow_history.cpp \
    book_store_status.cpp \
    reader.cpp \
    about.cpp

HEADERS += \
        mainwindow.h \
    mydatabase.h \
    reader_login.h \
    admin_login.h \
    admin.h \
    borrow_book.h \
    insert_reader.h \
    change_book.h \
    borrow_status.h \
    borrow_history.h \
    book_store_status.h \
    reader.h \
    about.h

FORMS += \
        mainwindow.ui \
    reader_login.ui \
    admin_login.ui \
    admin.ui \
    borrow_book.ui \
    insert_reader.ui \
    change_book.ui \
    borrow_status.ui \
    borrow_history.ui \
    book_store_status.ui \
    reader.ui \
    about.ui

RESOURCES += \
    test.qrc
