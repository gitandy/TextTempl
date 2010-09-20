#include <QMainWindow>
#include <QMap>
#include <QString>
#include <QTableWidgetItem>
#include <QApplication>
#include <QClipboard>
#include "ui_TextTempl.h"

class MainApp 
: public QMainWindow, private Ui::MainWindow
 {
     Q_OBJECT

 public:
     MainApp(QMainWindow *parent = 0);
     QMap<QString,  QTableWidgetItem*> fmap;
     QString templ;

 private slots: 
     void openFile();
     void saveFile();
     void showAbout();
     void copyToClip();
     void pasteFromClip();

 private:
     QMap<QString, QTableWidgetItem*> buildTable(QString templ);
     void resetTable();
     QClipboard *clipboard;
 }; 
