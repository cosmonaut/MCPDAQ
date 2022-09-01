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

    // etherdaq interface
    m_eth_iface = new MCPDAQEtherDaqIface(this);

    // etherdaq configuration
//    m_ec = new etherdaq_config_t();
//    m_ec->addr = QHostAddress("192.168.1.5");
//    m_ec->port = 5555;
//    m_ec->pkt_len = 1458;
//    m_ec->xbits = 16;
//    m_ec->ybits = 16;
//    m_ec->pbits = 8;
//    m_ec->aratio = 1.0;

    // Display plot data on status bar.
    connect(ui->widget_implot, SIGNAL(click_pos(const QString &)), ui->statusbar, SLOT(showMessage(const QString &)));
    // EtherDaq dialog action
    connect(ui->actionEtherDAQ, SIGNAL(triggered()), this, SLOT(etherdaq()));
    // monitor button
    connect(ui->actionMonitor, SIGNAL(triggered(bool)), this, SLOT(monitor(bool)));
    //connect(ui->actionMonitor, SIGNAL(checkableChanged(bool)), this, SLOT(monitor(bool)));
    connect(ui->actionMonitor, SIGNAL(triggered(bool)), ui->widget_implot, SLOT(run(bool)));

    // etherdaq connections
    connect(m_eth_iface, SIGNAL(valid_data(const QList<photon_t> &)), ui->widget_implot, SLOT(append_data(const QList<photon_t> &)));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::etherdaq(void)
{
    etherdaq_config_t ec = m_eth_iface->get_ec();
    MCPDAQEtherDaqDialog *etherdaq_dialog = new MCPDAQEtherDaqDialog(this, &ec);
    int r = etherdaq_dialog->exec();
    if (r == QDialog::Accepted) {
        // update settings.
        //*m_ec = etherdaq_dialog->m_ec;
        m_eth_iface->set_ec(&(etherdaq_dialog->m_ec));
        //qDebug() << m_ec->addr;
        // Need to close/reopen port?
    }
}

void MainWindow::monitor(bool c)
{
    //qDebug () << c;
    if (c == true) {
        //
        m_eth_iface->open();
        qDebug() << "monitoring...";
    } else {
        //
        m_eth_iface->close();
    }
}
