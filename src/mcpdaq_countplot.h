#ifndef MCPDAQ_COUNTPLOT_H
#define MCPDAQ_COUNTPLOT_H

#include <QWidget>

#include "qcustomplot.h"

class mcpdaq_countplot : public QWidget
{
    Q_OBJECT
public:
    explicit mcpdaq_countplot(QWidget *parent = nullptr);

public slots:
    // Append a single data point
    void append_data(double c);
    void clear_data(void);

signals:

private:
    // Plot
    QCustomPlot *m_plot;

    // Data structures
    QQueue<double> *m_ydata;
    QList<double> *m_xdata;
};

#endif // MCPDAQ_COUNTPLOT_H
