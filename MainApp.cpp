#include <iostream>
#include <QMessageBox>
#include <QStringList>
#include <QRegExp>
#include <QFileDialog>
#include <QDir>
#include <QFile>
#include <QIODevice>
#include <QTextStream>

#include "MainApp.h"

#include "version.h"

MainApp::MainApp(QMainWindow *parent)
    : QMainWindow(parent)
{
    setupUi(this);

    clipboard = QApplication::clipboard();

    this->connect(this->actionOpen,SIGNAL(triggered()),SLOT(openFile()));
    this->connect(this->actionSave,SIGNAL(triggered()),SLOT(saveFile()));
    this->connect(this->actionAbout,SIGNAL(triggered()),SLOT(showAbout()));

    this->connect(this->actionCopy,SIGNAL(triggered()),SLOT(copyToClip()));
    this->connect(this->actionPaste,SIGNAL(triggered()),SLOT(pasteFromClip()));

    this->connect(this->actionInsertCol,SIGNAL(triggered()),SLOT(insertCol()));
    this->connect(this->actionDeleteCol,SIGNAL(triggered()),SLOT(deleteCol()));

    this->resetTable();

    tableWidget->resizeRowsToContents();
    tableWidget->resizeColumnsToContents();

    tableWidget->setEnabled(false);
    actionSave->setEnabled(false);
    actionCopy->setEnabled(false);
    actionPaste->setEnabled(false);

    templ = "";

    this->last_openpath = QDir::homePath();
    this->last_savepath = "";
}

/*
 * private slot
 */
void MainApp::openFile()
{
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open template"), this->last_openpath, tr("Templates") + " (*.templ);;" + tr("All Files") + " (*.*)");

    if(fileName != "") {
        QFile *templFile = new QFile(fileName);
        if(!templFile->open(QIODevice::ReadOnly))
            QMessageBox::critical(this, tr("Error"), tr("Couldn't open") + " " + fileName + "\n" + templFile->errorString());
        else {
            int posLastSep = fileName.lastIndexOf('/', -1);
            this->last_openpath = fileName.left(posLastSep);

            if(this->last_savepath == "") {
                this->last_savepath = this->last_openpath;
            }

            QTextStream *ts = new QTextStream(templFile);
            templ = ts->readAll();
            templFile->close();

            fmap = buildTable(templ);

            tableWidget->resizeRowsToContents();

            if(!fmap.empty()) {
                this->tableWidget->setEnabled(true);
                this->actionSave->setEnabled(true);
                this->actionCopy->setEnabled(true);
                this->actionPaste->setEnabled(true);
                this->actionInsertCol->setEnabled(true);
                this->actionDeleteCol->setEnabled(true);
            }
            else {
                this->tableWidget->setEnabled(false);
                this->actionSave->setEnabled(false);
                this->actionCopy->setEnabled(false);
                this->actionPaste->setEnabled(false);
                this->actionInsertCol->setEnabled(false);
                this->actionDeleteCol->setEnabled(false);

                QMessageBox::warning(this, tr("Error"), tr("The file contains no or bad template data"));
            }
        }
    }
}

/*
 * private slot
 */
void MainApp::saveFile()
{
    QString fileName = QFileDialog::getSaveFileName(this, tr("Save file"), this->last_savepath, tr("All Files") + " (*.*)");

    if(fileName != "") {
        QFile *file = new QFile(fileName);
        if(!file->open(QIODevice::WriteOnly))
            QMessageBox::critical(this, tr("Error"), tr("Couldn't save to") + " " + fileName + "\n" + file->errorString());
        else {
            int posLastSep = fileName.lastIndexOf('/', -1);
            this->last_savepath = fileName.left(posLastSep);

            QTextStream *ts = new QTextStream(file);

            QString txt = templ;

            QMapIterator<QString, QTableWidgetItem*> i(fmap);
            while (i.hasNext()) {
                QString repl = "\\$\\$" + i.next().key() + "@([a-zA-Z0-9_:+#!= ]*)\\$\\$";

                txt.replace(QRegExp(repl), i.value()->text());
            }

            *ts << txt;
            ts->flush();
            file->close();
        }
    }
}

QMap<QString,QTableWidgetItem*> MainApp::buildTable(QString templ)
{
    this->resetTable();

    QMap<QString,QTableWidgetItem*> fmap;

    QStringList clst = templ.split("$$");

    int rowc = 0;

    for (int i = 1; i < clst.size(); i += 2) {
        QStringList pv = clst[i].split("@");

        if(!fmap.contains(pv[0])) {
            QTableWidgetItem *row = new QTableWidgetItem();

            QString param = pv[0];
            row->setText(param + ":");

            QTableWidgetItem *cell = new QTableWidgetItem();

            if(pv.size() > 0) {
                QString preVal = pv[1];
                cell->setText(preVal);
            }

            fmap.insert(pv[0], cell);

            tableWidget->setRowCount(rowc + 1);
            tableWidget->setVerticalHeaderItem(rowc, row);
            tableWidget->setItem(rowc, 0, cell);

            rowc++;
        }
    }

    return fmap;
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

    QTableWidgetItem *__item = new QTableWidgetItem();
    __item->setText(QApplication::translate("MainWindow", "Value", 0, QApplication::UnicodeUTF8));
    tableWidget->setItem(0, 0, __item);
}

/*
 * private slot
 */
void MainApp::insertCol()
{
    this->tableWidget->setColumnCount(this->tableWidget->columnCount() + 1);
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
    QString clipText = "";

    for(int r = 0; r < this->tableWidget->rowCount(); r++){
        for(int c = 0; c < this->tableWidget->columnCount(); c++){
            QTableWidgetItem *item = this->tableWidget->item(r, c);
            QString text = " ";
            if(item != 0) {
                text = item->text();
            }
            clipText += text;
            if(c != this->tableWidget->columnCount() - 1) {
                clipText += "\t";
            }
        }
        if(r != this->tableWidget->rowCount() - 1) {
           clipText += "\n";
        }
    }

    this->clipboard->setText(clipText);
}

/*
 * private slot
 */
void MainApp::pasteFromClip()
{
    QStringList lineList = this->clipboard->text().split("\n");

    for(int r = 0; r < lineList.size() && r < this->tableWidget->rowCount(); r++){
        QStringList colList = lineList[r].split("\t");

        for(int c = 0; c < colList.size() && c < this->tableWidget->columnCount(); c++){
            QTableWidgetItem *item = new QTableWidgetItem(colList[c].trimmed());
            this->tableWidget->setItem(r, c , item);
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
