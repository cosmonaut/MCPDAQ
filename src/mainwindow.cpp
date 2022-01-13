#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "MCPDAQutil.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    ui->statusbar->showMessage("OH HI, MARK");

}

MainWindow::~MainWindow()
{
    delete ui;
}

