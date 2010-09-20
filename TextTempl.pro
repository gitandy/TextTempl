TEMPLATE = app

OBJECTS_DIR = obj
MOC_DIR = moc

FORMS += TextTempl.ui

RESOURCES += TextTempl.qrc

win32:RC_FILE = TextTempl.rc

SOURCES += main.cpp \
           MainApp.cpp \

HEADERS += MainApp.h

TRANSLATIONS = TextTempl_de.ts
