#ifndef MCPDAQ_IMPLOT_H
#define MCPDAQ_IMPLOT_H

#include <QtWidgets/QWidget>

#include "qcustomplot.h"
#include "MCPDAQutil.h"

class mcpdaq_implot : public QWidget
{
    Q_OBJECT
public:
    mcpdaq_implot(QWidget *parent = 0);
    ~mcpdaq_implot();

public slots:
    void on_x_range_changed(QCPRange range);
    void on_y_range_changed(QCPRange range);
    void mouse_cap(QMouseEvent* mouse_event);


private:
    QCustomPlot *m_plot;
    QDoubleSpinBox *m_refresh_rate_spinbox;
    QComboBox *m_rebin_selector;

    float m_xmin = -0.5;
    float m_xmax = 100.5;
    float m_ymin = -0.5;
    float m_ymax = 100.5;
};

#endif // MCPDAQ_IMPLOT_H
