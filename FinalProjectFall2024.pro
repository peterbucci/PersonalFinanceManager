QT += core gui widgets charts sql

CONFIG += c++17

SOURCES += \
    GraphView.cpp \
    Ledger.cpp \
    LoginWindow.cpp \
    PasswordManager.cpp \
    SignUpWindow.cpp \
    Transaction.cpp \
    TransactionForm.cpp \
    ViewTransactions.cpp \
    main.cpp \
    MainWindow.cpp \
    settings.cpp \
    user.cpp \
    userlogin.cpp

HEADERS += \
    GraphView.h \
    Ledger.h \
    LoginWindow.h \
    MainWindow.h \
    PasswordManager.h \
    SignUpWindow.h \
    Transaction.h \
    TransactionForm.h \
    User.h \
    ViewTransactions.h \
    settings.h \
    userlogin.h

FORMS += \
    GraphView.ui \
    LoginWindow.ui \
    MainWindow.ui \
    SignUpWindow.ui \
    TransactionForm.ui \
    ViewTransactions.ui \
    settings.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
