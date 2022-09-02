#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "qcustomplot.h"

#include "MCPDAQutil.h"
#include "mcpdaqetherdaqiface.h"
#include "mcpdaq_data.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

public slots:
    void etherdaq(void);
    void monitor(bool);

private:
    Ui::MainWindow *ui;
    //QCustomPlot *customPlot;
    //MCPDAQEtherDaqDialog *etherdaq_dialog;
    //etherdaq_config_t *m_ec;
    MCPDAQEtherDaqIface *m_eth_iface;
    mcpdaq_data *m_data;
};

#endif // MAINWINDOW_H
