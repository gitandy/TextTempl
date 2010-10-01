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

 private slots: 
     void openFile();
     void saveFile();
     void saveAll();
     void openDataFile();
     void saveDataFile();
     void closeFile();
     void showAbout();
     void copyToClip();
     void pasteFromClip();
     void insertCol();
     void deleteCol();

 private:
     QMap<QString, int> fieldsMap;
     QMap<int, QString> defaultsMap;

     QString templ;

     void writeFile(QString fileName, int col);
     void buildTable(QString templ);
     void fillDefaults(int col);
     void resetTable();
     void fillTable(QString text, QString colSep);
     QString retrieveTable(QString sep);
     void setCell(int row, int col, QString text);
     QString cellText(int row, int col);
     QClipboard *clipboard;

     QSettings *settings;
 }; 
