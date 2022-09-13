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

    m_data = new mcpdaq_data(this);

    // etherdaq interface
    m_eth_iface = new MCPDAQEtherDaqIface(this);

    // Main 1 Hz timer
    m_replot_timer = new QTimer(this);
    m_replot_timer->setInterval(1000);

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

    // plot connections
    connect(ui->actionMonitor, SIGNAL(triggered(bool)), ui->widget_implot, SLOT(run(bool)));

    // Connect main 1 Hz timer to get count rate.
    connect(m_replot_timer, SIGNAL(timeout()), m_data, SLOT(update_cr()));
    connect(m_data, SIGNAL(count_rate(double)), ui->widget_crplot, SLOT(append_data(double)));

    // Etherdaq connections

    // main data aggregator
    connect(m_eth_iface, SIGNAL(valid_data(const QList<photon_t> &)), m_data, SLOT(append_data(const QList<photon_t> &)));
    // implot
    connect(m_eth_iface, SIGNAL(valid_data(const QList<photon_t> &)), ui->widget_implot, SLOT(append_data(const QList<photon_t> &)));
    // phdplot
    connect(m_eth_iface, SIGNAL(valid_data(const QList<photon_t> &)), ui->widget_phdplot, SLOT(append_data(const QList<photon_t> &)));

    connect(m_replot_timer, SIGNAL(timeout()), ui->widget_phdplot, SLOT(vid_replot()));
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
        m_eth_iface->set_ec(&(etherdaq_dialog->m_ec));
    }
}

void MainWindow::monitor(bool c)
{
    if (c == true) {
        // Clear current plots
        ui->widget_implot->clear_data();
        ui->widget_phdplot->clear_data();
        ui->widget_crplot->clear_data();
        // Open interface and start timer
        m_eth_iface->open();
        m_replot_timer->start();
        qDebug() << "monitoring...";
    } else {
        // close interface and stop timer
        m_eth_iface->close();
        m_replot_timer->stop();
    }
}
