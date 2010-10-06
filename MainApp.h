#include <QMainWindow>
#include <QMap>
#include <QString>
#include <QTableWidgetItem>
#include <QApplication>
#include <QClipboard>
#include <QSettings>
#include "ui_TextTempl.h"

#define CT_LINEEDIT 0
#define CT_COMBOBOX 1

class MainApp 
    : public QMainWindow, private Ui::MainWindow
{
    Q_OBJECT

public:
    MainApp(QString fileName, QMainWindow *parent = 0);
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
    QMap<int, int> typeMap;

    QString templ;
    void setClosedState();

    void openFileString(QString fileName);
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
