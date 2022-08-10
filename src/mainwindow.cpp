#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "MCPDAQutil.h"
#include "mcpdaqetherdaqdialog.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    ui->statusbar->showMessage("OH HI, MARK");

    // etherdaq configuration
    m_ec = new etherdaq_config_t();
    m_ec->addr = QHostAddress("192.168.1.5");
    m_ec->port = 5555;
    m_ec->pkt_len = 1458;
    m_ec->xbits = 16;
    m_ec->ybits = 16;
    m_ec->pbits = 8;
    m_ec->aratio = 1.0;

    // Display plot data on status bar.
    connect(ui->widget, SIGNAL(click_pos(const QString &)), ui->statusbar, SLOT(showMessage(const QString &)));
    // EtherDaq dialog action
    connect(ui->actionEtherDAQ, SIGNAL(triggered()), this, SLOT(etherdaq()));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::etherdaq(void)
{
    MCPDAQEtherDaqDialog *etherdaq_dialog = new MCPDAQEtherDaqDialog(this, m_ec);
    int r = etherdaq_dialog->exec();
    if (r == QDialog::Accepted) {
        // update settings.
        *m_ec = etherdaq_dialog->m_ec;
        qDebug() << m_ec->addr;
    }
}
