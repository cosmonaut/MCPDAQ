#include "mcpdaq_implot.h"

mcpdaq_implot::mcpdaq_implot(QWidget *parent) : QWidget(parent)
{
    // main widget layout
    QVBoxLayout *vbox = new QVBoxLayout(this);
    // layout for controls under plot
    QHBoxLayout *hbox = new QHBoxLayout();


    // refresh rate spinbox
    m_refresh_rate_spinbox = new QDoubleSpinBox(this);
    m_refresh_rate_spinbox->setDecimals(1);
    m_refresh_rate_spinbox->setValue(1.0);
    m_refresh_rate_spinbox->setRange(0.1, 60.0);
    m_refresh_rate_spinbox->setSingleStep(1.0);

    // rebin combo box
    m_rebin_selector = new QComboBox(this);


    // CONFIGURE PLOT ITEMS

    m_plot = new QCustomPlot();

    // configure axis rect:
    // this will also allow rescaling the color scale by dragging/zooming
    m_plot->setInteractions(QCP::iRangeDrag|QCP::iRangeZoom);
    m_plot->axisRect()->setupFullAxesBox(true);


    // set up the QCPColorMap:
    QCPColorMap *colorMap = new QCPColorMap(m_plot->xAxis, m_plot->yAxis);
    int nx = 200;
    int ny = 200;
    colorMap->setInterpolate(FALSE);
    colorMap->data()->setSize(nx, ny); // we want the color map to have nx * ny data points
    // can set custom ranges -- we just want the pixels
    colorMap->data()->setRange(QCPRange(-0.5, 200.5), QCPRange(-0.5, 200.5));

    // Blast some nonsense data in there...
    int i = 0;
    int j = 0;
    for (i = 0; i < 200; i++) {
        for (j = 0; j < 200; j++) {
            colorMap->data()->setCell(i, j, i*j);
        }
    }

    // add a color scale:
    QCPColorScale *colorScale = new QCPColorScale(m_plot);
    m_plot->plotLayout()->addElement(0, 1, colorScale); // add it to the right of the main axis rect
    colorScale->setType(QCPAxis::atRight); // scale shall be vertical bar with tick/axis labels right (actually atRight is already the default)
    colorMap->setColorScale(colorScale); // associate the color map with the color scale
    //colorScale->axis()->setLabel("TUMBODOORS");

    // Choose color gradients that look good!
    MCPDAQColorGradient *cg = new MCPDAQColorGradient(MCPDAQColorGradient::viridis);

    // set the color gradient of the color map to one of the presets:
    //colorMap->setGradient(QCPColorGradient::gpHot);

    colorMap->setGradient(*cg);
    // we could have also created a QCPColorGradient instance and added own colors to
    // the gradient, see the documentation of QCPColorGradient for what's possible.

    // rescale the data dimension (color) such that all data points lie in the span visualized by the color gradient:
    colorMap->rescaleDataRange();

    // make sure the axis rect and color scale synchronize their bottom and top margins (so they line up):
    QCPMarginGroup *marginGroup = new QCPMarginGroup(m_plot);
    m_plot->axisRect()->setMarginGroup(QCP::msBottom|QCP::msTop, marginGroup);
    colorScale->setMarginGroup(QCP::msBottom|QCP::msTop, marginGroup);

    // rescale the key (x) and value (y) axes so the whole color map is visible:
    m_plot->rescaleAxes();

    // antialias settings
    m_plot->setNoAntialiasingOnDrag(TRUE);
    m_plot->setNotAntialiasedElement(QCP::aeAll, TRUE);

    //customplot->yAxis->setScaleRatio(ui->sectionPlot->xAxis,1.0);
    //m_plot->yAxis->setScaleRatio(m_plot->)
    // Gotta work on axis ratio. Might need to be captured any time there is a resize..?
    //m_plot->yAxis->setScaleRatio(m_plot->xAxis, 1.8);

    //m_plot->rescaleAxes();
    //m_plot->replot();


    QLabel *sp_label = new QLabel("Refresh Rate");
    QLabel *rb_label = new QLabel("Rebin");


    hbox->addWidget(sp_label);
    hbox->addWidget(m_refresh_rate_spinbox);
    hbox->addWidget(rb_label);
    hbox->addWidget(m_rebin_selector);
    hbox->addStretch();


    vbox->addWidget(m_plot);
    vbox->addLayout(hbox);

    // Handle signals and slots
    connect(m_plot->xAxis, SIGNAL(rangeChanged(QCPRange)), this, SLOT(on_x_range_changed(QCPRange)));
    connect(m_plot->yAxis, SIGNAL(rangeChanged(QCPRange)), this, SLOT(on_y_range_changed(QCPRange)));
    connect(m_plot, SIGNAL(mouseRelease(QMouseEvent*)), this, SLOT(mouse_cap(QMouseEvent*)));


}

// choose the form of the destructor.
mcpdaq_implot::~mcpdaq_implot()
{

}

void mcpdaq_implot::on_x_range_changed(QCPRange range)
{
    m_plot->xAxis->setRange(range.bounded(-0.5, 200.5));
}

void mcpdaq_implot::on_y_range_changed(QCPRange range)
{
    m_plot->yAxis->setRange(range.bounded(-0.5, 200.5));
}

void mcpdaq_implot::mouse_cap(QMouseEvent* mouse_event)
{
    if (mouse_event->button() == Qt::RightButton) {
        // unzoom
        m_plot->xAxis->setRange(QCPRange(-0.5, 200.5));
        m_plot->yAxis->setRange(QCPRange(-0.5, 200.5));
    }
}
