#include <QMainWindow>
#include <QMap>
#include <QString>
#include <QTableWidgetItem>
#include <QApplication>
#include <QClipboard>
#include <QSettings>

#include "ui_TextTempl.h"

#include "Highlighter.h"

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
    void newFile();
    void openFile();
    void saveFile();
    void saveFileAs();
    void createFile();
    void createAll();
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

    QString currentFile;

    QString templ;
    void setClosedState();
    void setOpenedState();
    void openFileString(QString fileName);
    void writeTemplFile(QString fileName);
    void writeFile(QString fileName, int col);
    void buildTable(QString templ);
    void fillDefaults(int col);
    void fillTable(QString text, QString colSep);
    QString retrieveTable(QString sep);
    void setCell(int row, int col, QString text);
    QString cellText(int row, int col);
    QClipboard *clipboard;

    QSettings *settings;

    Highlighter *highlighter;
};
