#include "mcpdaq_countplot.h"

mcpdaq_countplot::mcpdaq_countplot(QWidget *parent)
    : QWidget{parent}
{
    QHBoxLayout *hbox = new QHBoxLayout(this);

    // Setup data structures.
    int i = 0;
    m_ydata = new QQueue<double>;
    m_xdata = new QList<double>;

    for (i = 0; i < 60; i++) {
        m_xdata->insert(i, (double)(59 - i));
        m_ydata->enqueue(0.0);
    }

    // Set up plot.
    m_plot = new QCustomPlot();
    m_plot->addGraph();

    // Graph Settings
    QPen pen;
    pen.setColor(QColor("black"));
    m_plot->graph()->setPen(pen);
    m_plot->graph()->setLineStyle(QCPGraph::lsStepCenter);

    // Set data
    m_plot->graph()->setData(*m_xdata, *m_ydata);
    m_plot->xAxis->setRangeReversed(true);
    m_plot->graph()->rescaleAxes(true);
    m_plot->axisRect()->setupFullAxesBox();

    m_plot->xAxis->setLabel("Time (s)");
    m_plot->yAxis->setLabel("Rate (counts/s)");

    // Performance settings.
    m_plot->setNoAntialiasingOnDrag(true);
    m_plot->setNotAntialiasedElements(QCP::aeAll);
    m_plot->setPlottingHints(QCP::phFastPolylines);

    // Background colors to match Qt
    m_plot->setBackground(QBrush(QWidget::palette().color(QWidget::backgroundRole())));
    m_plot->axisRect()->setBackground(QBrush(QColor(230, 230, 230)));

    hbox->addWidget(m_plot);
}

void mcpdaq_countplot::append_data(double c)
{
    // Append a single data point
    m_ydata->enqueue(c);
    // pop old data to maintain size
    m_ydata->dequeue();
    m_plot->graph()->setData(*m_xdata, *m_ydata);
    m_plot->graph()->rescaleAxes(false);

    // Pretty up the range.
    double max = *std::max_element(m_ydata->begin(), m_ydata->end());
    max = max + max/10.0;
    if (max == 0.0) {
        max = 1.0;
    }
    QCPRange yrange = QCPRange(0.0, max);
    m_plot->yAxis->setRange(yrange);

    m_plot->replot(QCustomPlot::rpQueuedReplot);
}
