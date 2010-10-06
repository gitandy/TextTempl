#include <iostream>
#include <QMessageBox>
#include <QStringList>
#include <QRegExp>
#include <QFileDialog>
#include <QDir>
#include <QFile>
#include <QIODevice>
#include <QTextStream>
#include <QSettings>
#include <QWidget>
#include <QLineEdit>
#include <QComboBox>

#include "MainApp.h"

#include "version.h"

MainApp::MainApp(QString fileName, QMainWindow *parent)
    : QMainWindow(parent)
{
    setupUi(this);

    this->clipboard = QApplication::clipboard();

    //Menu
    this->connect(this->actionOpen,SIGNAL(triggered()),SLOT(openFile()));
    this->connect(this->actionSave,SIGNAL(triggered()),SLOT(saveFile()));
    this->connect(this->actionSaveAll,SIGNAL(triggered()),SLOT(saveAll()));

    this->connect(this->actionLoadData,SIGNAL(triggered()),SLOT(openDataFile()));
    this->connect(this->actionSaveData,SIGNAL(triggered()),SLOT(saveDataFile()));

    this->connect(this->actionClose,SIGNAL(triggered()),SLOT(closeFile()));

    this->connect(this->actionAbout,SIGNAL(triggered()),SLOT(showAbout()));

    this->connect(this->actionCopy,SIGNAL(triggered()),SLOT(copyToClip()));
    this->connect(this->actionPaste,SIGNAL(triggered()),SLOT(pasteFromClip()));

    this->connect(this->actionInsertCol,SIGNAL(triggered()),SLOT(insertCol()));
    this->connect(this->actionDeleteCol,SIGNAL(triggered()),SLOT(deleteCol()));

    //Toolbar
    this->connect(this->actionOpen_TB,SIGNAL(triggered()),SLOT(openFile()));
    this->connect(this->actionSave_TB,SIGNAL(triggered()),SLOT(saveFile()));
    this->connect(this->actionSaveAll_TB,SIGNAL(triggered()),SLOT(saveAll()));

    this->connect(this->actionLoadData_TB,SIGNAL(triggered()),SLOT(openDataFile()));

    this->connect(this->actionCopy_TB,SIGNAL(triggered()),SLOT(copyToClip()));
    this->connect(this->actionPaste_TB,SIGNAL(triggered()),SLOT(pasteFromClip()));

    this->connect(this->actionInsertCol_TB,SIGNAL(triggered()),SLOT(insertCol()));
    this->connect(this->actionDeleteCol_TB,SIGNAL(triggered()),SLOT(deleteCol()));

    this->resetTable();

    this->tableWidget->resizeRowsToContents();
    this->tableWidget->resizeColumnsToContents();

    this->tableWidget->setEnabled(false);

    this->templ = "";

    //Settings
    this->settings = new QSettings("schawo.de", "TextTempl");

    if(!this->settings->contains("openpath")) {
        this->settings->setValue("openpath", QDir::homePath());
    }

    if(!this->settings->contains("datapath")) {
        this->settings->setValue("datapath", QDir::homePath());
    }

    if(!this->settings->contains("savepath")) {
        this->settings->setValue("savepath", QDir::homePath());
    }

    this->settings->sync();

    if(fileName != "") {
        this->openFileString(fileName);
    }
}

MainApp::~MainApp()
{
    this->settings->sync();
}

/*
 * private slot
 */
void MainApp::openFile()
{
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open template"), this->settings->value("openpath").toString(), tr("Templates") + " (*.templ);;" + tr("All Files") + " (*.*)");

    if(fileName != "") {
        this->openFileString(fileName);

        int posLastSep = fileName.lastIndexOf('/', -1);
        this->settings->setValue("openpath", fileName.left(posLastSep));
    }
}

void MainApp::openFileString(QString fileName)
{
    QFile *templFile = new QFile(fileName);

    if(!templFile->open(QIODevice::ReadOnly))
            QMessageBox::critical(this, tr("Error"), tr("Couldn't open") + " " + fileName + "\n" + templFile->errorString());
        else {
            QTextStream *ts = new QTextStream(templFile);
            templ = ts->readAll();
            templFile->close();

            this->buildTable(templ);

            tableWidget->resizeRowsToContents();

            if(!this->fieldsMap.empty()) {
                this->tableWidget->setEnabled(true);
                this->actionSave->setEnabled(true);
                this->actionSaveAll->setEnabled(true);
                this->actionLoadData->setEnabled(true);
                this->actionSaveData->setEnabled(true);
                this->actionClose->setEnabled(true);
                this->actionCopy->setEnabled(true);
                this->actionPaste->setEnabled(true);
                this->actionInsertCol->setEnabled(true);
                this->actionDeleteCol->setEnabled(true);

                this->actionSave_TB->setEnabled(true);
                this->actionSaveAll_TB->setEnabled(true);
                this->actionLoadData_TB->setEnabled(true);
                this->actionCopy_TB->setEnabled(true);
                this->actionPaste_TB->setEnabled(true);
                this->actionInsertCol_TB->setEnabled(true);
                this->actionDeleteCol_TB->setEnabled(true);
            }
            else {
                this->setClosedState();

                QMessageBox::warning(this, tr("Error"), tr("The file contains no or bad template data"));
            }
        }
}

/*
 * private slot
 */
void MainApp::openDataFile()
{
    QString fileName = QFileDialog::getOpenFileName(this, tr("Load data file"), this->settings->value("datapath").toString(), tr("CSV-File") + " (*.csv);;" + tr("All Files") + " (*.*)");

    if(fileName != "") {
        QFile *dataFile = new QFile(fileName);
        if(!dataFile->open(QIODevice::ReadOnly))
            QMessageBox::critical(this, tr("Error"), tr("Couldn't open") + " " + fileName + "\n" + dataFile->errorString());
        else {
            int posLastSep = fileName.lastIndexOf('/', -1);
            this->settings->setValue("datapath", fileName.left(posLastSep));

            QTextStream *ts = new QTextStream(dataFile);
            QString data = ts->readAll();
            dataFile->close();

            QString sep = ";";

            if(this->settings->contains("csv_sep")) {
                sep = this->settings->value("csv_sep").toString();
            }

            this->fillTable(data.replace("\"", ""), sep);
        }
    }
}

/*
 * private slot
 */
void MainApp::saveDataFile()
{
    QString fileName = QFileDialog::getSaveFileName(this, tr("Save data file"), this->settings->value("datapath").toString(), tr("CSV-File") + " (*.csv);;" + tr("All Files") + " (*.*)");

    if(fileName != "") {
        QFile *file = new QFile(fileName);
        if(!file->open(QIODevice::WriteOnly))
            QMessageBox::critical(this, tr("Error"), tr("Couldn't save to") + " " + fileName + "\n" + file->errorString());
        else {
            int posLastSep = fileName.lastIndexOf('/', -1);
            this->settings->setValue("datapath", fileName.left(posLastSep));

            QTextStream *ts = new QTextStream(file);

            QString sep = ";";

            if(this->settings->contains("csv_sep")) {
                sep = this->settings->value("csv_sep").toString();
            }

            if(this->settings->contains("csv_quote") && this->settings->value("csv_quote").toBool()) {
                *ts << "\"" << this->retrieveTable("\"" + sep + "\"").replace("\n", "\"\n\"") << "\"";
            } else {
                *ts << this->retrieveTable(sep);
            }

            ts->flush();
            file->close();
        }
    }
}

/*
 * private slot
 */
void MainApp::saveFile()
{
    int col = this->tableWidget->currentColumn();

    QString fileDefault = "";

    if(fieldsMap.contains(tr("Name"))){
        QString text = this->cellText(fieldsMap.value(tr("Name")), col);

        if(text.trimmed() != "") {
            fileDefault = "/" + text + ".txt";
        }
    }

    QString fileName = QFileDialog::getSaveFileName(this, tr("Save file"), this->settings->value("savepath").toString() + fileDefault, tr("All Files") + " (*.*)");

    if(fileName != "") {
        int posLastSep = fileName.lastIndexOf('/', -1);
        this->settings->setValue("savepath", fileName.left(posLastSep));

        this->writeFile(fileName, col);
    }
}

void MainApp::writeFile(QString fileName, int col)
{
    QFile *file = new QFile(fileName);
    if(!file->open(QIODevice::WriteOnly))
        QMessageBox::critical(this, tr("Error"), tr("Couldn't save to") + " " + fileName + "\n" + file->errorString());
    else {
        QTextStream *ts = new QTextStream(file);

        QString txt = templ.remove("$$" + tr("Name") + "@***$$").trimmed();

        QMapIterator<QString, int> i(fieldsMap);
        while (i.hasNext()) {
            QString repl = "\\$\\$" + i.next().key() + "@([a-zA-Z0-9_:+#!= ]*)\\$\\$";

            QString text = this->cellText(i.value(), col);

            txt.replace(QRegExp(repl), text);
        }

        *ts << txt;
        ts->flush();
        file->close();
    }
}

/*
 * private slot
 */
void MainApp::saveAll()
{
    QString dirName = QFileDialog::getExistingDirectory(this, tr("Save all"), this->settings->value("savepath").toString(), QFileDialog::ShowDirsOnly);

    this->settings->setValue("savepath", dirName);

    for(int c = 0; c < this->tableWidget->columnCount(); c++) {
        QString fileName = "";
        if(fieldsMap.contains(tr("Name"))){
            fileName = this->cellText(fieldsMap.value(tr("Name")), c);
        }

        if(fileName == "") {
            fileName = tr("unnamed") + "_" + QString::number(c + 1);
        }

        fileName = dirName + "/" + fileName + ".txt";

        this->writeFile(fileName, c);
    }
}

/*
 * private slot
 */
void MainApp::closeFile()
{
    this->resetTable();

    this->setClosedState();
}

void MainApp::setClosedState()
{
    this->tableWidget->setEnabled(false);
    this->actionSave->setEnabled(false);
    this->actionSaveAll->setEnabled(false);
    this->actionLoadData->setEnabled(false);
    this->actionSaveData->setEnabled(false);
    this->actionClose->setEnabled(false);
    this->actionCopy->setEnabled(false);
    this->actionPaste->setEnabled(false);
    this->actionInsertCol->setEnabled(false);
    this->actionDeleteCol->setEnabled(false);

    this->actionSave_TB->setEnabled(false);
    this->actionSaveAll_TB->setEnabled(false);
    this->actionLoadData_TB->setEnabled(false);
    this->actionCopy_TB->setEnabled(false);
    this->actionPaste_TB->setEnabled(false);
    this->actionInsertCol_TB->setEnabled(false);
    this->actionDeleteCol_TB->setEnabled(false);
}

void MainApp::buildTable(QString templ)
{
    this->resetTable();

    this->fieldsMap.clear();
    this->defaultsMap.clear();
    this->typeMap.clear();

    QStringList clst = templ.split("$$");

    int rowc = 0;

    for (int i = 1; i < clst.size(); i += 2) {
        QStringList pv = clst[i].split("@");

        if(!fieldsMap.contains(pv[0])) {
            //Header
            QTableWidgetItem *row = new QTableWidgetItem();

            QString param = pv[0];
            row->setText(param);

            fieldsMap.insert(pv[0], rowc);

            tableWidget->setRowCount(rowc + 1);
            tableWidget->setVerticalHeaderItem(rowc, row);

            //Content
            QString preVal = "";

            if(pv.size() > 0) {
                preVal = pv[1];
            }

            this->defaultsMap.insert(rowc, preVal);
            if(preVal.contains("::")) {
                this->typeMap.insert(rowc, CT_COMBOBOX);
            } else {
                this->typeMap.insert(rowc, CT_LINEEDIT);
            }

            rowc++;
        }
    }

    this->fillDefaults(0);
}

void MainApp::fillDefaults(int col)
{
    for(int row = 0; row < this->tableWidget->rowCount(); row++) {
        this->setCell(row, col, this->defaultsMap.value(row));
    }
}

/*
 * private slot
 */
void MainApp::resetTable() 
{
    tableWidget->clear();

    tableWidget->setRowCount(1);
    tableWidget->setColumnCount(1);

    QTableWidgetItem *__rowItem = new QTableWidgetItem();
    __rowItem->setText(QApplication::translate("MainWindow", "Parameter", 0, QApplication::UnicodeUTF8));
    tableWidget->setVerticalHeaderItem(0, __rowItem);

    QLineEdit *__item = new QLineEdit();
    __item->setFrame(false);
    __item->setText(QApplication::translate("MainWindow", "Value", 0, QApplication::UnicodeUTF8));
    tableWidget->setCellWidget(0, 0, __item);
}

/*
 * private slot
 */
void MainApp::insertCol()
{
    this->tableWidget->setColumnCount(this->tableWidget->columnCount() + 1);
    this->fillDefaults(this->tableWidget->columnCount() - 1);
}

/*
 * private slot
 */
void MainApp::deleteCol()
{
    if(this->tableWidget->columnCount() > 1) {
        this->tableWidget->setColumnCount(this->tableWidget->columnCount() - 1);
    }
}

/*
 * private slot
 */
void MainApp::copyToClip()
{
    this->clipboard->setText(this->retrieveTable("\t"));
}

QString MainApp::retrieveTable(QString sep)
{
    QString data = "";

    for(int r = 0; r < this->tableWidget->rowCount(); r++){
        for(int c = 0; c < this->tableWidget->columnCount(); c++){
            data += this->cellText(r, c);
            if(c != this->tableWidget->columnCount() - 1) {
                data += sep;
            }
        }
        if(r != this->tableWidget->rowCount() - 1) {
            data += "\n";
        }
    }

    return data;
}

/*
 * private slot
 */
void MainApp::pasteFromClip()
{
    this->fillTable(this->clipboard->text(), "\t");
}

void MainApp::fillTable(QString text, QString colSep)
{
    QStringList lineList = text.split("\n");

    for(int r = 0; r < lineList.size() && r < this->tableWidget->rowCount(); r++){
        QStringList colList = lineList[r].split(colSep);

        for(int c = 0; c < colList.size(); c++){
            if(c == this->tableWidget->columnCount()) {
                this->tableWidget->setColumnCount(c + 1);
            }
            this->setCell(r, c, colList[c].trimmed());
        }
    }
}

/*
 * private slot
 */
void MainApp::showAbout()
{
    QString modified = "";

    if(QString(MODIFIED) == "M") {
        modified = "-Modified";
    }

#ifdef QT_NO_DEBUG
    QString debug = "";
#else
    QString debug = "-Debug";
#endif

    QMessageBox::information(this, tr("About"), "(c) 2010 A. Schawo\n\n" + tr("TextTempl") + "\nVersion: " + QString(VERSION) + modified + debug);
}

/*
 * private
 */
void MainApp::setCell(int row, int col, QString text)
{
    QWidget *cell;

    if(this->typeMap.value(row) == CT_COMBOBOX) {
        cell = (QWidget *)new QComboBox();;
        ((QComboBox *)cell)->setFrame(false);
        ((QComboBox *)cell)->addItems(this->defaultsMap.value(row).split("::"));

        if(this->defaultsMap.value(row).trimmed().endsWith("::")) {
            ((QComboBox *)cell)->setEditable(true);;
        }

        if(int i = ((QComboBox *)cell)->findText(text) > 0) {
            ((QComboBox *)cell)->setCurrentIndex(i);
        }
    } else {
        cell = (QWidget *)new QLineEdit();;
        ((QLineEdit *)cell)->setFrame(false);
        ((QLineEdit *)cell)->setText(text);
    }

    this->tableWidget->setCellWidget(row, col, cell);
}

/*
 * private
 */
QString MainApp::cellText(int row, int col)
{
    QWidget *cell = this->tableWidget->cellWidget(row, col);

    QString text = "";

    if(cell != 0) {
        if(cell->inherits(QLineEdit::staticMetaObject.className())) {
            text = ((QLineEdit *)cell)->text();
        } else if(cell->inherits(QComboBox::staticMetaObject.className())) {
            text = ((QComboBox *)cell)->currentText();
        }
    }

    return text;
}
