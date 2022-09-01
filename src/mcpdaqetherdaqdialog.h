#ifndef MCPDAQETHERDAQDIALOG_H
#define MCPDAQETHERDAQDIALOG_H

#include <QDialog>

#include "MCPDAQutil.h"

namespace Ui {
class MCPDAQEtherDaqDialog;
}

class MCPDAQEtherDaqDialog : public QDialog
{
    Q_OBJECT

public:
    //explicit MCPDAQEtherDaqDialog(QWidget *parent = nullptr, etherdaq_config_t *ec);
    explicit MCPDAQEtherDaqDialog(QWidget *parent, etherdaq_config_t *ec);
    ~MCPDAQEtherDaqDialog();

    etherdaq_config_t m_ec;

private slots:
    void done(int r);

private:
    Ui::MCPDAQEtherDaqDialog *ui;
};

#endif // MCPDAQETHERDAQDIALOG_H
