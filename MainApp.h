/* Copyright (c) 2011, Andreas Schawo <andreas@schawo.de>

All rights reserved.

Redistribution and use in source and binary forms, with or without modification,
are permitted provided that the following conditions are met:

   1. Redistributions of source code must retain the above copyright notice,
      this list of conditions and the following disclaimer.
   2. Redistributions in binary form must reproduce the above copyright notice,
      this list of conditions and the following disclaimer in the documentation
      and/or other materials provided with the distribution.
   3. Neither the name of the authors nor the names of its contributors may
      be used to endorse or promote products derived from this software without
      specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE AUTHORS 'AS IS' AND ANY EXPRESS OR IMPLIED
WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
IN NO EVENT SHALL THE AUTHORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR
BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER
IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
POSSIBILITY OF SUCH DAMAGE.
*/

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

    QString appPath;

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
    void reloadTempl();
    void copyToClip();
    void pasteFromClip();
    void insertCol();
    void deleteCol();
    void showHelp();
    void showAbout();

private:
    QMap<QString, int> fieldsMap;
    QMap<int, QString> defaultsMap;
    QMap<int, int> typeMap;

    QString currentFile;

    QString wTitle;

    QString templ;
    void setClosedState();
    void setOpenedState();
    void openFileString(QString fileName);
    void writeTemplFile(QString fileName);
    void writeFile(QString fileName, int col);
    bool buildTable(QString templ);
    void fillDefaults(int col);
    void fillTable(QString text, QString colSep);
    QString retrieveTable(QString sep);
    void setCell(int row, int col, QString text);
    QString cellText(int row, int col);
    QClipboard *clipboard;

    QSettings *settings;

    Highlighter *highlighter;
};
