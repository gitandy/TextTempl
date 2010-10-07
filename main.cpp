#include <QApplication>
#include <QTranslator>
#include <QLocale>
#include <QDebug>
#include "MainApp.h"

int main(int argc, char* argv[])
{
    QApplication app(argc, argv);

    QTranslator myappTranslator;
    myappTranslator.load("TextTempl_" + QLocale::system().name(), app.applicationDirPath());
    app.installTranslator(&myappTranslator);

    QString fileName = "";

    if(app.arguments().count() > 1) {
        fileName = app.arguments().at(1);
    }

    MainApp *mw = new MainApp(fileName);

    mw->appPath = app.applicationDirPath();

    mw->show();

    return app.exec();
}
