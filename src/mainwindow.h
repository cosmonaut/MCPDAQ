#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "qcustomplot.h"

#include "MCPDAQUtil.h"

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

private:
    Ui::MainWindow *ui;
    //QCustomPlot *customPlot;
    //MCPDAQEtherDaqDialog *etherdaq_dialog;
    etherdaq_config_t *m_ec;
};

#endif // MAINWINDOW_H
