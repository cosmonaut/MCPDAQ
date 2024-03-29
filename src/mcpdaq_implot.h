#ifndef MCPDAQ_IMPLOT_H
#define MCPDAQ_IMPLOT_H

#include <QtWidgets/QWidget>

#include "qcustomplot.h"
#include "MCPDAQutil.h"
#include "mcpdaq_types.h"

class mcpdaq_implot : public QWidget
{
    Q_OBJECT
public:
    mcpdaq_implot(QWidget *parent = nullptr);
    //mcpdaq_implot(QWidget *parent = 0) : QCustomPlot(parent){};
    ~mcpdaq_implot();


public slots:
    //void on_x_range_changed(QCPRange new_range, QCPRange old_range);
    //void on_y_range_changed(QCPRange new_range, QCPRange old_range);

    void mouse_press(QMouseEvent* mouse_event);
    void mouse_rel(QMouseEvent* mouse_event);
    void mouse_move(QMouseEvent* mouse_event);
    void mouse_wheel(QWheelEvent* event);

    //void plot_click(QCPAbstractPlottable* plottable, int dataIndex, QMouseEvent* event);
    void before_replot(void);

    void rect_accepted(const QRect &rect, QMouseEvent* event);

    void cm_update(int index);
    void sc_update(int index);

    void append_data(const QList<photon_t> &data);
    void clear_data(void);

    void vid_replot(void);
    //void run(bool);

    //void update_config();

signals:
    void click_pos(const QString & message);

protected:
    void resizeEvent(QResizeEvent* event);

private:
    // plot items
    QCustomPlot *m_plot;
    QCPColorMap *colorMap;
    MCPDAQColorGradient *cg;

    //QCPColorMapData *m_data;

    // controls
    // This spinbox should probably become a combo box for "persistance"
    //QDoubleSpinBox *m_refresh_rate_spinbox;
    //QComboBox *m_rebin_selector;
    QComboBox *m_cm_selector;
    QComboBox *m_scaling_selector;
    // TODO: sync to mainwindow 1 Hz timer
    //QTimer *m_replot_timer;


    uint8_t x_bit = 11;
    uint8_t y_bit = 11;

    uint16_t x_px = 1 << x_bit;
    uint16_t y_px = 1 << y_bit;

    // range for plot
    float m_xmin = 0.0;
    float m_xmax = x_px - 1;
    float m_ymin = 0.0;
    float m_ymax = y_px - 1;
};

#endif // MCPDAQ_IMPLOT_H
