#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "MCPDAQutil.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    ui->statusbar->showMessage("OH HI, MARK");

    // Display plot data on status bar.
    connect(ui->widget, SIGNAL(click_pos(const QString &)), ui->statusbar, SLOT(showMessage(const QString &)));
}

MainWindow::~MainWindow()
{
    delete ui;
}

