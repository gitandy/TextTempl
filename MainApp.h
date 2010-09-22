#include <QMainWindow>
#include <QMap>
#include <QString>
#include <QTableWidgetItem>
#include <QApplication>
#include <QClipboard>
#include <QSettings>
#include "ui_TextTempl.h"

class MainApp 
: public QMainWindow, private Ui::MainWindow
 {
     Q_OBJECT

 public:
     MainApp(QMainWindow *parent = 0);
     virtual ~MainApp();
     QMap<QString, int> fmap;

     QString templ;

 private slots: 
     void openFile();
     void saveFile();
     void showAbout();
     void copyToClip();
     void pasteFromClip();
     void insertCol();
     void deleteCol();

 private:
     QMap<QString, int> buildTable(QString templ);
     void resetTable();
     QClipboard *clipboard;

     QSettings *settings;
 }; 
