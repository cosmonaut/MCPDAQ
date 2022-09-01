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
    m_cm_selector = new QComboBox(this);
    m_scaling_selector = new QComboBox(this);

    QMetaEnum metaEnum = QMetaEnum::fromType<MCPDAQColorGradient::MCPGradPreset>();
    int ii = 0;
    for (ii = 0; ii < metaEnum.keyCount(); ii++) {
        m_cm_selector->addItem(metaEnum.key(ii));
    }

    QMetaEnum scalEnum = QMetaEnum::fromType<MCPDAQColorGradient::MCPGradScaling>();
    for (ii = 0; ii < scalEnum.keyCount(); ii++) {
        m_scaling_selector->addItem(scalEnum.key(ii));
    }

    m_replot_timer = new QTimer();
    m_replot_timer->setInterval(1000);

    // CONFIGURE PLOT ITEMS

    m_plot = new QCustomPlot();
    //m_plot->setOpenGl(true);
    //qDebug() << m_plot->openGl();

    // Old zoom stuff...
    //m_plot->setInteractions(QCP::iRangeDrag|QCP::iRangeZoom);
    //m_plot->setInteractions(QCP::iRangeDrag);

    m_plot->axisRect()->setupFullAxesBox(true);

    // Useful for selecting rectangles, can do other thing sthere.
    //m_plot->setSelectionRectMode(QCP::srmZoom);
    // Custom interactions implemented.
    m_plot->setSelectionRectMode(QCP::srmCustom);

    //m_plot->addLayer("image", m_plot->layer("main"), QCustomPlot::limAbove);
    //m_plot->layer("image")->setMode(QCPLayer::lmBuffered);

    // set up the QCPColorMap:
    //QCPColorMap *colorMap = new QCPColorMap(m_plot->xAxis, m_plot->yAxis);
    colorMap = new QCPColorMap(m_plot->xAxis, m_plot->yAxis);
    //colorMap->setLayer("image");
    //colorMap->layer()->setMode(QCPLayer::lmBuffered);

    colorMap->setInterpolate(false);
    colorMap->data()->setSize(x_px, y_px); // we want the color map to have x_px * y_px data points
    colorMap->data()->setRange(QCPRange(m_xmin, m_xmax), QCPRange(m_ymin, m_ymax));

    //m_data = colorMap->data();

    // Blast some nonsense data in there...
    //int i = 0;
    //int j = 0;
    //int c = 0;

    colorMap->data()->fill(0.0);

    // Blast some nonsense data in there...
    //int i = 0;
    //int j = 0;
    //int c = 0;

//    for (i = 0; i < x_px; i++) {
//        //c = i % 2;
//        for (j = 0; j < y_px; j++) {
//            //colorMap->data()->setCell(i, j, i*j);
//            //colorMap->data()->setCell(i, j, sin(i/6.0));
//            colorMap->data()->setCell(i, j, 100.0*(cos(i/6.0) + cos(j/6.0)));
//            // checkerboard
//            //colorMap->data()->setCell(i, j, c);
//            //c += 1;
//            //c = c % 2;
//        }
//    }

    // add a color scale:
    QCPColorScale *colorScale = new QCPColorScale(m_plot);
    m_plot->plotLayout()->addElement(0, 1, colorScale); // add it to the right of the main axis rect
    colorScale->setType(QCPAxis::atRight); // scale shall be vertical bar with tick/axis labels right (actually atRight is already the default)
    colorMap->setColorScale(colorScale); // associate the color map with the color scale
    //colorScale->axis()->setLabel("Counts");

    // Choose color gradients that look good!
    //MCPDAQColorGradient *cg = new MCPDAQColorGradient(MCPDAQColorGradient::cubehelix);
    cg = new MCPDAQColorGradient(MCPDAQColorGradient::cubehelix);
    // TODO: there is a bug with the night and grayscale colormaps not doing log/sqrt scale properly...

    m_cm_selector->setCurrentIndex(MCPDAQColorGradient::cubehelix);
    colorMap->setGradient(*cg);

    // rescale the data dimension (color) such that all data points lie in the span visualized by the color gradient:
    colorMap->rescaleDataRange();

    // make sure the axis rect and color scale synchronize their bottom and top margins (so they line up):
    QCPMarginGroup *marginGroup = new QCPMarginGroup(m_plot);
    m_plot->axisRect()->setMarginGroup(QCP::msBottom|QCP::msTop, marginGroup);
    colorScale->setMarginGroup(QCP::msBottom|QCP::msTop, marginGroup);

    // rescale the key (x) and value (y) axes so the whole color map is visible:
    m_plot->rescaleAxes();

    // antialias settings
    m_plot->setNoAntialiasingOnDrag(true);
    m_plot->setNotAntialiasedElements(QCP::aeAll);
    m_plot->setPlottingHints(QCP::phFastPolylines);
    //m_plot->setNotAntialiasedElement(QCP::aeAll, true);

    //ui->mPlot->setNotAntialiasedElements(QCP::aeAll);
    //ui->mPlot->setNoAntialiasingOnDrag(true);
    //ui->mPlot->setPlottingHints(QCP::phFastPolylines);

    //m_plot->axisRect()->setBackground(QBrush(QColor(QColor("black"))));
    m_plot->setBackground(QBrush(QWidget::palette().color(QWidget::backgroundRole())));
    m_plot->axisRect()->setBackground(QBrush(QColor(186, 186, 186)));
    // 186, 186, 186 nice
    // 207, 207, 207

    m_plot->xAxis->setRange(QCPRange(m_xmin, m_xmax));
    m_plot->yAxis->setRange(QCPRange(m_ymin, m_ymax));
    m_plot->xAxis->setScaleRatio(m_plot->yAxis, 1.0);

    //m_plot->rescaleAxes();
    m_plot->replot();
    //m_plot->setMinimumSize(QSize(500, 500));

    //QSizePolicy p = this->sizePolicy();
    //p.setHeightForWidth( true );
    //this->setSizePolicy( p );
    //m_plot->setSizePolicy( p );

    QLabel *sp_label = new QLabel("Refresh Rate");
    QLabel *rb_label = new QLabel("Rebin");
    QLabel *cm_label = new QLabel("Color Map");
    QLabel *sc_label = new QLabel("Scaling");

    hbox->addWidget(sp_label);
    hbox->addWidget(m_refresh_rate_spinbox);
    hbox->addWidget(rb_label);
    hbox->addWidget(m_rebin_selector);
    hbox->addWidget(cm_label);
    hbox->addWidget(m_cm_selector);
    hbox->addWidget(sc_label);
    hbox->addWidget(m_scaling_selector);
    hbox->addStretch();

    vbox->addWidget(m_plot);
    vbox->addLayout(hbox);

    // Handle signals and slots

    //connect(m_plot->xAxis, SIGNAL(rangeChanged(QCPRange, QCPRange)), this, SLOT(on_x_range_changed(QCPRange, QCPRange)));
    //connect(m_plot->yAxis, SIGNAL(rangeChanged(QCPRange, QCPRange)), this, SLOT(on_y_range_changed(QCPRange, QCPRange)));

    connect(m_plot->selectionRect(), SIGNAL(accepted(const QRect &, QMouseEvent*)), this, SLOT(rect_accepted(const QRect &, QMouseEvent*)));
    // For unzooming
    connect(m_plot, SIGNAL(mousePress(QMouseEvent*)), this, SLOT(mouse_press(QMouseEvent*)));
    connect(m_plot, SIGNAL(mouseRelease(QMouseEvent*)), this, SLOT(mouse_rel(QMouseEvent*)));
    connect(m_plot, SIGNAL(mouseMove(QMouseEvent*)), this, SLOT(mouse_move(QMouseEvent*)));
    connect(m_plot, SIGNAL(mouseWheel(QWheelEvent*)), this, SLOT(mouse_wheel(QWheelEvent*)));

    //connect(m_plot, SIGNAL(plottableClick(QCPAbstractPlottable*, int, QMouseEvent*)), this, SLOT(plot_click(QCPAbstractPlottable*, int, QMouseEvent*)));
    connect(m_plot, SIGNAL(beforeReplot()), this, SLOT(before_replot()));

    connect(m_cm_selector, SIGNAL(currentIndexChanged(int)), this, SLOT(cm_update(int)));
    connect(m_scaling_selector, SIGNAL(currentIndexChanged(int)), this, SLOT(sc_update(int)));

    connect(m_replot_timer, SIGNAL(timeout()), this, SLOT(vid_replot()));
}

// choose the form of the destructor.
mcpdaq_implot::~mcpdaq_implot()
{

}


void mcpdaq_implot::rect_accepted(const QRect &rect, QMouseEvent* event)
{
    const QRect* r = &rect;

    double bl_x;
    double bl_y;
    double tr_x;
    double tr_y;

    qDebug() << "ACCEPT";

    if (event->button() == Qt::LeftButton) {
        // Only zoom if rubber band is within axisRect() inner rect (i.e. the plot itself).
        if (m_plot->axisRect()->rect().contains(*r)) {
            // test that zoom is large enough (at least 10x10 pixels)
            m_plot->plottable()->pixelsToCoords(r->bottomLeft(), bl_x, bl_y);
            m_plot->plottable()->pixelsToCoords(r->topRight(), tr_x, tr_y);

            if (abs(tr_x - bl_x) > 10.0 && abs(tr_y - bl_y) > 10.0) {
                m_plot->axisRect()->zoom(*r);
                m_plot->xAxis->setScaleRatio(m_plot->yAxis, 1.0);
                m_plot->replot();
            }
        }
    }
}

//void mcpdaq_implot::on_x_range_changed(QCPRange new_range, QCPRange old_range)
//{
//    if (new_range.size() < 10.0) {
//        // don't update
//        //m_plot->xAxis->setRange(old_range.bounded(m_xmin, m_xmax));
//        m_plot->xAxis->setRange(old_range);
//    } else {
//        //m_plot->xAxis->setRange(new_range.bounded(m_xmin, m_xmax));
//        m_plot->xAxis->setRange(new_range);
//    }
//    m_plot->xAxis->setScaleRatio(m_plot->yAxis, 1.0);
//    //qDebug() << "xchange";
//}

//void mcpdaq_implot::on_y_range_changed(QCPRange new_range, QCPRange old_range)
//{
//    if (new_range.size() < 10.0) {
//        // don't update
//        //m_plot->yAxis->setRange(old_range.bounded(m_ymin, m_ymax));
//        m_plot->yAxis->setRange(old_range);
//    } else {
//        //m_plot->yAxis->setRange(new_range.bounded(m_ymin, m_ymax));
//        m_plot->yAxis->setRange(new_range);
//    }
//    m_plot->xAxis->setScaleRatio(m_plot->yAxis, 1.0);
//}

void mcpdaq_implot::mouse_press(QMouseEvent* mouse_event)
{
    if (mouse_event->button() == Qt::MiddleButton) {
        m_plot->setSelectionRectMode(QCP::srmNone);
    } else if (mouse_event->button() == Qt::RightButton) {
        m_plot->setSelectionRectMode(QCP::srmNone);
    } else if (mouse_event->button() == Qt::LeftButton) {
        // emit location and data value
        double key;
        double value;
        m_plot->plottable()->pixelsToCoords(mouse_event->pos(), key, value);
        QString str;
        QTextStream(&str) << "X: " << QString::number(key, 'f', 1) << " Y: " << QString::number(value, 'f', 1) << " Data: " << QString::number(colorMap->data()->data(key, value), 'f', 1);
        emit click_pos(str);
    }
}

void mcpdaq_implot::mouse_rel(QMouseEvent* mouse_event)
{
    //qDebug() << "mcap";

    double key;
    double value;

    double xdiff;
    double ydiff;

    QCPRange xrange;
    QCPRange yrange;

    double bl_x;
    double bl_y;
    double tr_x;
    double tr_y;

    m_plot->plottable()->pixelsToCoords(mouse_event->pos(), key, value);
    //qDebug() << key << " " << value;

    if (mouse_event->button() == Qt::RightButton) {
        m_plot->setSelectionRectMode(QCP::srmCustom);
        // right button unzoom
        m_plot->xAxis->setRange(QCPRange(m_xmin, m_xmax));
        m_plot->yAxis->setRange(QCPRange(m_ymin, m_ymax));
        m_plot->xAxis->setScaleRatio(m_plot->yAxis, 1.0);
        m_plot->replot();
    } else if (mouse_event->button() == Qt::MiddleButton) {
        m_plot->setSelectionRectMode(QCP::srmCustom);

        // middle button navigations
        m_plot->plottable()->pixelsToCoords(m_plot->axisRect()->bottomLeft(), bl_x, bl_y);
        m_plot->plottable()->pixelsToCoords(m_plot->axisRect()->topRight(), tr_x, tr_y);

        if (key >= bl_x && key < tr_x) {
            if (value >= bl_y && value <= tr_y) {
                // middle button navigation
                xrange = m_plot->xAxis->range();
                xdiff = key - (xrange.lower + xrange.size()/2.0);
                yrange = m_plot->yAxis->range();
                ydiff = value - (yrange.lower + yrange.size()/2.0);
                m_plot->xAxis->moveRange(xdiff);
                m_plot->yAxis->moveRange(ydiff);
                m_plot->replot();
            }
        }
    }
}

void mcpdaq_implot::mouse_move(QMouseEvent* mouse_event)
{
    if (mouse_event->buttons() & Qt::MiddleButton) {
        //m_plot->setSelectionRectMode(QCP::srmNone);
        // may be used for color scaling
        // TODO
        qDebug() << mouse_event->pos();
    }
}

void mcpdaq_implot::mouse_wheel(QWheelEvent* event)
{
    double wangle;
    double zfact;
    double vm;
    double hm;
    QRectF r;
    QMarginsF m;

    // capture mouse wheel angle
    wangle = event->angleDelta().y();

    // based on wangle, create a QMarginsF that can shrink/expand the inner rect by some factor?
    r = m_plot->axisRect()->rect();

    // zoom factor is 2x for every wheel detent (15 degrees)
    zfact = 2.0*(abs(wangle)/120);

    if (wangle < 0) {
        // zoom out
        vm = r.height()*((zfact - 1.0)/2.0);
        hm = r.width()*((zfact - 1.0)/2.0);
        m = QMarginsF(hm, vm, hm, vm);
        r += m;
    } else {
        // zoom in
        vm = (r.height() - r.height()/zfact)/2.0;
        hm = (r.width() - r.width()/zfact)/2.0;
        m = QMarginsF(hm, vm, hm, vm);
        r -= m;
    }

    // Check coord space and limit zoom
    double vs = abs(m_plot->yAxis->pixelToCoord(r.top()) - m_plot->yAxis->pixelToCoord(r.bottom()));
    double hs = abs(m_plot->xAxis->pixelToCoord(r.right()) - m_plot->xAxis->pixelToCoord(r.left()));

    if (vs > 10.0 && vs < y_px*8.0) {
        if (hs > 10.0 && hs < x_px*8.0) {
            m_plot->axisRect()->zoom(r);
            m_plot->replot();
        }
    }
}

//void mcpdaq_implot::plot_click(QCPAbstractPlottable* plottable, int dataIndex, QMouseEvent* event)
//{
//    //UNREFERENCED_PARAMETER(dataIndex);
//    Q_UNUSED(dataIndex);

//    double key;
//    double value;

//    // stuff for middle click re-center
//    double xdiff;
//    double ydiff;

//    QCPRange xrange;
//    QCPRange yrange;

//    qDebug() << "PLOT CLICK";

//    // demo of getting plot click location
//    plottable->pixelsToCoords(event->pos(), key, value);
//    //qDebug() << key << " " << value;

//    // middle click focus
//    if (event->button() == Qt::MiddleButton) {
//        // middle button navigation

//        xrange = m_plot->xAxis->range();
//        xdiff = key - (xrange.lower + xrange.size()/2.0);
//        //qDebug() << xdiff;
//        yrange = m_plot->yAxis->range();
//        ydiff = value - (yrange.lower + yrange.size()/2.0);
//        //qDebug() << ydiff;
//        m_plot->xAxis->moveRange(xdiff);
//        m_plot->yAxis->moveRange(ydiff);
//        m_plot->replot();
//        qDebug() << "move";
//    }
//}

void mcpdaq_implot::resizeEvent(QResizeEvent* event)
{
    //UNREFERENCED_PARAMETER(event);
    Q_UNUSED(event);
    // this really needs to stay in here for some reason? before_replot() doesn't catch all...
    m_plot->xAxis->setScaleRatio(m_plot->yAxis, 1.0);
    m_plot->replot();
}

void mcpdaq_implot::before_replot()
{
    m_plot->xAxis->setScaleRatio(m_plot->yAxis, 1.0);
}

// colormap update
void mcpdaq_implot::cm_update(int index)
{
    // index of cmap changed.
    //MCPDAQColorGradient *cg = new MCPDAQColorGradient(static_cast<MCPDAQColorGradient::MCPGradPreset>(index));
    //cg = new MCPDAQColorGradient(static_cast<MCPDAQColorGradient::MCPGradPreset>(index));
    cg->set_preset(static_cast<MCPDAQColorGradient::MCPGradPreset>(index));

    QMetaEnum metaEnum = QMetaEnum::fromType<MCPDAQColorGradient::MCPGradScaling>();

    // Need to check scaling settings and update appropriately
    cg->setScaling((MCPDAQColorGradient::MCPGradScaling)metaEnum.value(m_scaling_selector->currentIndex()));

    colorMap->setGradient(*cg);
    m_plot->replot(QCustomPlot::rpQueuedReplot);
    //qDebug() << m_plot->replotTime();
}

// colormap scaling update
void mcpdaq_implot::sc_update(int index)
{
    // index of scaling changed.
    QMetaEnum metaEnum = QMetaEnum::fromType<MCPDAQColorGradient::MCPGradScaling>();

    cg->setScaling((MCPDAQColorGradient::MCPGradScaling)metaEnum.value(index));
    colorMap->setGradient(*cg);
    m_plot->replot(QCustomPlot::rpQueuedReplot);
    //qDebug() << m_plot->replotTime();
}

// Efficiency here will be interesting.
// https://www.qcustomplot.com/index.php/support/forum/1838
void mcpdaq_implot::append_data(const QList<photon_t> &data)
{
    //
    QList<photon_t>::ConstIterator it = data.constBegin();
    for (; it != data.constEnd(); ++it) {
        const photon_t& photon = *it;
        //qDebug() << photon.x;
        colorMap->data()->setCell(photon.x, photon.y, colorMap->data()->cell(photon.x, photon.y) + 1);
        //colorMap->data()->setCell(photon.x, photon.y, 10.0);
    }
}

void mcpdaq_implot::vid_replot()
{
    //QElapsedTimer timer;
    //timer.start();

    colorMap->data()->setCell(10, 10, 1.0);

    //qDebug() << timer.elapsed();

    //colorMap->data()->setData(10, 10, 20.0);

    //colorMap->data()->setCell(10, 10, 1.0);

    colorMap->rescaleDataRange();
    m_plot->replot(QCustomPlot::rpQueuedReplot);
    //m_plot->layer("image")->replot();
    //qDebug() << timer.elapsed();

    qDebug() << m_plot->replotTime();
    //qDebug() << m_plot->layerCount();
    //qDebug() << timer.elapsed();
    //qDebug();
}

void mcpdaq_implot::run(bool stat)
{
    if (stat == true) {
        m_replot_timer->start();
    } else {
        m_replot_timer->stop();
    }
}
