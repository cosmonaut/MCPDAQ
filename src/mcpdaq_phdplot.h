#ifndef MCPDAQ_PHDPLOT_H
#define MCPDAQ_PHDPLOT_H

#include <QWidget>
#include "qcustomplot.h"
#include "mcpdaq_types.h"

class mcpdaq_phdplot : public QWidget
{
    Q_OBJECT
public:
    explicit mcpdaq_phdplot(QWidget *parent = nullptr);

public slots:
    void append_data(const QList<photon_t> &data);
    void vid_replot(void);

signals:

private:
    // Plot
    QCustomPlot *m_plot;

    // pulse height settings
    quint8 p_bit = 8;
    quint16 p_px = 1 << p_bit;

    // Data
    QList<double> *m_xdata;
    QList<double> *m_ydata;
};

#endif // MCPDAQ_PHDPLOT_H
