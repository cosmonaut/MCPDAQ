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
        m_xdata->insert(i, (double)i);
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
    m_plot->graph()->rescaleAxes(true);
}
