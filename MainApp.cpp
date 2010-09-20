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

    this->resetTable();

    tableWidget->resizeRowsToContents();
    tableWidget->resizeColumnsToContents();

    tableWidget->setEnabled(false);
    actionSave->setEnabled(false);
    actionCopy->setEnabled(false);
    actionPaste->setEnabled(false);

    templ = "";
}

void MainApp::openFile()
{
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open template"), QDir::homePath(), tr("Templates") + " (*.templ);;" + tr("All Files") + " (*.*)");

    if(fileName != "") {
        QFile *templFile = new QFile(fileName);
        if(!templFile->open(QIODevice::ReadOnly))
            QMessageBox::critical(this, tr("Error"), tr("Couldn't open") + " " + fileName + "\n" + templFile->errorString());
        else {
            QTextStream *ts = new QTextStream(templFile);
            templ = ts->readAll();
            templFile->close();

            fmap = buildTable(templ);

            tableWidget->resizeRowsToContents();

            if(!fmap.empty()) {
                tableWidget->setEnabled(true);
                actionSave->setEnabled(true);
                actionCopy->setEnabled(true);
                actionPaste->setEnabled(true);
            }
            else {
                tableWidget->setEnabled(false);
                actionSave->setEnabled(false);
                actionCopy->setEnabled(false);
                actionPaste->setEnabled(false);

                QMessageBox::warning(this, tr("Error"), tr("The file contains no or bad template data"));
            }
        }
    }
}

void MainApp::saveFile()
{
    QString fileName = QFileDialog::getSaveFileName(this, tr("Save file"), QDir::homePath(), tr("All Files") + " (*.*)");

    if(fileName != "") {
        QFile *file = new QFile(fileName);
        if(!file->open(QIODevice::WriteOnly))
            QMessageBox::critical(this, tr("Error"), tr("Couldn't save to") + " " + fileName + "\n" + file->errorString());
        else {
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

void MainApp::resetTable() 
{
    tableWidget->clear();

    tableWidget->setRowCount(1);
    tableWidget->setColumnCount(1);

    QTableWidgetItem *__colItem = new QTableWidgetItem();
    __colItem->setText(QApplication::translate("MainWindow", "Content", 0, QApplication::UnicodeUTF8));
    tableWidget->setHorizontalHeaderItem(0, __colItem);
    if (tableWidget->rowCount() < 1)
        tableWidget->setRowCount(1);

    QTableWidgetItem *__rowItem = new QTableWidgetItem();
    __rowItem->setText(QApplication::translate("MainWindow", "Parameter", 0, QApplication::UnicodeUTF8));
    tableWidget->setVerticalHeaderItem(0, __rowItem);

    QTableWidgetItem *__item = new QTableWidgetItem();
    __item->setText(QApplication::translate("MainWindow", "Value", 0, QApplication::UnicodeUTF8));
    tableWidget->setItem(0, 0, __item);
}

void MainApp::copyToClip()
{
    QString clipText = "";

    int rCount = tableWidget->rowCount();
    for(int i = 0; i < rCount; i++){
        QTableWidgetItem *item = tableWidget->item(i, 0);
        clipText = clipText + item->text() + "\n";
    }

    clipboard->setText(clipText);
}

void MainApp::pasteFromClip()
{
    QString clipText = clipboard->text();

    QStringList clipList = clipText.split("\n");

    int rCount = tableWidget->rowCount();
    for(int i = 0; i < clipList.size() && i < rCount; i++){
        QTableWidgetItem *item = tableWidget->item(i, 0);
        item->setText(clipList[i].trimmed());
    }
}

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
