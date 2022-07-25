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
    //mcpdaq_implot(QWidget *parent = 0) : QCustomPlot(parent){};
    ~mcpdaq_implot();

public slots:
    void on_x_range_changed(QCPRange new_range, QCPRange old_range);
    void on_y_range_changed(QCPRange new_range, QCPRange old_range);

    void mouse_press(QMouseEvent* mouse_event);
    void mouse_rel(QMouseEvent* mouse_event);
    void mouse_move(QMouseEvent* mouse_event);
    void mouse_wheel(QWheelEvent* event);

    void plot_click(QCPAbstractPlottable* plottable, int dataIndex, QMouseEvent* event);
    void before_replot(void);

    void rect_accepted(const QRect &rect, QMouseEvent* event);

protected:
    void resizeEvent(QResizeEvent* event);

private:
    QCustomPlot *m_plot;
    QDoubleSpinBox *m_refresh_rate_spinbox;
    QComboBox *m_rebin_selector;

    uint8_t x_bit = 10;
    uint8_t y_bit = 10;

    uint16_t x_px = 1 << x_bit;
    uint16_t y_px = 1 << y_bit;

    // range for plot
    float m_xmin = 0.0;
    float m_xmax = x_px - 1;
    float m_ymin = 0.0;
    float m_ymax = y_px - 1;
};

#endif // MCPDAQ_IMPLOT_H
