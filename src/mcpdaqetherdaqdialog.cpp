#include "mcpdaqetherdaqdialog.h"
#include "ui_mcpdaqetherdaqdialog.h"

MCPDAQEtherDaqDialog::MCPDAQEtherDaqDialog(QWidget *parent, etherdaq_config_t *ec) :
    QDialog(parent),
    ui(new Ui::MCPDAQEtherDaqDialog)
{
    ui->setupUi(this);

    // Populate ec values...
    ui->addr_lineEdit->setText(ec->addr.toString());
    ui->port_spinBox->setValue(ec->port);
    ui->pktlen_spinBox->setValue(ec->pkt_len);
    ui->xbits_spinBox->setValue(ec->xbits);
    ui->ybits_spinBox->setValue(ec->ybits);
    ui->pbits_spinBox->setValue(ec->pbits);
    ui->aspect_doubleSpinBox->setValue(ec->aratio);

    m_ec = *ec;
}

MCPDAQEtherDaqDialog::~MCPDAQEtherDaqDialog()
{
    delete ui;
}

// need a slot to set EC values on "ok"
// validation
void MCPDAQEtherDaqDialog::done(int r)
{
    if (r == QDialog::Accepted) {
        // Validate IP address.
        QHostAddress addr = QHostAddress(ui->addr_lineEdit->text());
        if (addr.isNull()) {
            qDebug() << "fail";
            QMessageBox::warning(this, "", "Invalid IP address");
        }

        // Set all ec values
        m_ec.addr = addr;
        m_ec.port = ui->port_spinBox->value();
        m_ec.pkt_len = ui->pktlen_spinBox->value();
        m_ec.xbits = ui->xbits_spinBox->value();
        m_ec.ybits = ui->ybits_spinBox->value();
        m_ec.pbits = ui->pbits_spinBox->value();
        m_ec.aratio = ui->aspect_doubleSpinBox->value();

        QDialog::done(r);
    } else {
        QDialog::done(r);
    }
}
