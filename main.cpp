#include <QApplication>
#include <QTranslator>
#include <QLocale>
#include "MainApp.h"

int main(int argc, char* argv[])
{
    QApplication app(argc, argv);

    QTranslator myappTranslator;
    myappTranslator.load("TextTempl_" + QLocale::system().name());
    app.installTranslator(&myappTranslator);

    QMainWindow *mw = new MainApp();

    mw->show();

    return app.exec();
}
