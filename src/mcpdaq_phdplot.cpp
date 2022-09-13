#include "mcpdaq_phdplot.h"

mcpdaq_phdplot::mcpdaq_phdplot(QWidget *parent)
    : QWidget{parent}
{
    QHBoxLayout *hbox = new QHBoxLayout(this);

    // Setup data structures.
    int i = 0;
    m_ydata = new QList<double>(p_px);
    //m_ydata = new QList<double>;
    m_xdata = new QList<double>;

    for (i = 0; i < p_px; i++) {
        m_xdata->insert(i, (double)i);
        //m_ydata->insert(i, (double)i);
    }
    m_ydata->fill(0.0);

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
    m_plot->graph()->rescaleAxes(true);
    m_plot->axisRect()->setupFullAxesBox();

    m_plot->axisRect()->setMinimumMargins(QMargins(0,0,0,0));

    m_plot->xAxis->setLabel("Pulse height");
    m_plot->yAxis->setLabel("Frequency");

    // Performance settings.
    m_plot->setNoAntialiasingOnDrag(true);
    m_plot->setNotAntialiasedElements(QCP::aeAll);
    m_plot->setPlottingHints(QCP::phFastPolylines);

    // Background colors to match Qt
    m_plot->setBackground(QBrush(QWidget::palette().color(QWidget::backgroundRole())));
    m_plot->axisRect()->setBackground(QBrush(QColor(230, 230, 230)));

    QCPRange yrange = QCPRange(0.0, 1.0);
    m_plot->yAxis->setRange(yrange);

    hbox->addWidget(m_plot);
}

void mcpdaq_phdplot::append_data(const QList<photon_t> &data)
{
    //auto pdata = m_plot->graph()->data();
    QList<photon_t>::ConstIterator it = data.constBegin();
    for (; it != data.constEnd(); ++it) {
        const photon_t& photon = *it;
        (*m_ydata)[photon.p] = m_ydata->at(photon.p) + 1.0;
    }
    //m_plot->graph()->setData(*m_xdata, *m_ydata);
}

void mcpdaq_phdplot::vid_replot(void) {
    m_plot->graph()->setData(*m_xdata, *m_ydata);

    // Pretty up the range.
    double max = *std::max_element(m_ydata->begin(), m_ydata->end());
    max = max + max/10.0;
    if (max == 0.0) {
        max = 1.0;
    }
    QCPRange yrange = QCPRange(0.0, max);
    m_plot->yAxis->setRange(yrange);

    //m_plot->graph()->rescaleAxes(false);
    m_plot->replot(QCustomPlot::rpQueuedReplot);
}
