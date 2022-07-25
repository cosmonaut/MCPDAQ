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
    //m_plot->setInteractions(QCP::iRangeDrag|QCP::iRangeZoom);
    //m_plot->setInteractions(QCP::iRangeDrag);

    m_plot->axisRect()->setupFullAxesBox(true);
    // how best to handle zoom?

    // Useful for selecting rectangles, can do other thing sthere.
    //m_plot->setSelectionRectMode(QCP::srmZoom);
    m_plot->setSelectionRectMode(QCP::srmCustom);
    // use this to implement custom zoom?


    // set up the QCPColorMap:
    QCPColorMap *colorMap = new QCPColorMap(m_plot->xAxis, m_plot->yAxis);
    colorMap->setInterpolate(FALSE);
    colorMap->data()->setSize(x_px, y_px); // we want the color map to have x_px * y_px data points
    // can set custom ranges -- we just want the pixels
    //colorMap->data()->setRange(QCPRange(-0.5, 200.5), QCPRange(-0.5, 200.5));
    colorMap->data()->setRange(QCPRange(m_xmin, m_xmax), QCPRange(m_ymin, m_ymax));


    // Blast some nonsense data in there...
    int i = 0;
    int j = 0;
    //int c = 0;

    for (i = 0; i < x_px; i++) {
        //c = i % 2;
        for (j = 0; j < y_px; j++) {
            //colorMap->data()->setCell(i, j, i*j);
            //colorMap->data()->setCell(i, j, sin(i/6.0));
            colorMap->data()->setCell(i, j, cos(i/6.0) + cos(j/6.0));
            // checkerboard
            //colorMap->data()->setCell(i, j, c);
            //c += 1;
            //c = c % 2;
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


    hbox->addWidget(sp_label);
    hbox->addWidget(m_refresh_rate_spinbox);
    hbox->addWidget(rb_label);
    hbox->addWidget(m_rebin_selector);
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
            //qDebug() << tr_x - bl_x << " " << tr_y - bl_y;

            if (abs(tr_x - bl_x) > 10.0 && abs(tr_y - bl_y) > 10.0) {
                m_plot->axisRect()->zoom(*r);
                m_plot->replot();
            }
        }
    }
}


// should this be done at a lower level?
// or does it need to be combined with y due to aspect ratio?
// interaction of these two functions and changing the range here can lead to messiness.
// basically need a slot that catches when either change, fix both at same time?

// or something that catches a zoom attempt and stops before range is ever changed?
// this seems like the "right" way.

void mcpdaq_implot::on_x_range_changed(QCPRange new_range, QCPRange old_range)
{
    if (new_range.size() < 10.0) {
        // don't update
        //m_plot->xAxis->setRange(old_range.bounded(m_xmin, m_xmax));
        m_plot->xAxis->setRange(old_range);
    } else {
        //m_plot->xAxis->setRange(new_range.bounded(m_xmin, m_xmax));
        m_plot->xAxis->setRange(new_range);
    }
    m_plot->xAxis->setScaleRatio(m_plot->yAxis, 1.0);
    //qDebug() << "xchange";
}

void mcpdaq_implot::on_y_range_changed(QCPRange new_range, QCPRange old_range)
{
    if (new_range.size() < 10.0) {
        // don't update
        //m_plot->yAxis->setRange(old_range.bounded(m_ymin, m_ymax));
        m_plot->yAxis->setRange(old_range);
    } else {
        //m_plot->yAxis->setRange(new_range.bounded(m_ymin, m_ymax));
        m_plot->yAxis->setRange(new_range);
    }
    m_plot->xAxis->setScaleRatio(m_plot->yAxis, 1.0);
}

void mcpdaq_implot::mouse_press(QMouseEvent* mouse_event)
{
    if (mouse_event->button() == Qt::MiddleButton) {
        m_plot->setSelectionRectMode(QCP::srmNone);
    } else if (mouse_event->button() == Qt::RightButton) {
        m_plot->setSelectionRectMode(QCP::srmNone);
    }
}

void mcpdaq_implot::mouse_rel(QMouseEvent* mouse_event)
{
    qDebug() << "mcap";

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

    //QPoint pos = mouse_event->pos();

    m_plot->plottable()->pixelsToCoords(mouse_event->pos(), key, value);
    //m_plot->plottable()->pixelsToCoords(pos, key, value);
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
                //qDebug() << xdiff;
                yrange = m_plot->yAxis->range();
                ydiff = value - (yrange.lower + yrange.size()/2.0);
                //qDebug() << ydiff;
                m_plot->xAxis->moveRange(xdiff);
                m_plot->yAxis->moveRange(ydiff);
                m_plot->replot();
                //qDebug() << "move";
            }
        }
    }
}

void mcpdaq_implot::mouse_move(QMouseEvent* mouse_event)
{
    if (mouse_event->buttons() & Qt::MiddleButton) {
        //m_plot->setSelectionRectMode(QCP::srmNone);
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

void mcpdaq_implot::plot_click(QCPAbstractPlottable* plottable, int dataIndex, QMouseEvent* event)
{
    //UNREFERENCED_PARAMETER(dataIndex);
    Q_UNUSED(dataIndex);

    double key;
    double value;

    // stuff for middle click re-center
    double xdiff;
    double ydiff;

    QCPRange xrange;
    QCPRange yrange;

    qDebug() << "PLOT CLICK";

    // demo of getting plot click location
    plottable->pixelsToCoords(event->pos(), key, value);
    //qDebug() << key << " " << value;

    // middle click focus
    if (event->button() == Qt::MiddleButton) {
        // middle button navigation

        xrange = m_plot->xAxis->range();
        xdiff = key - (xrange.lower + xrange.size()/2.0);
        //qDebug() << xdiff;
        yrange = m_plot->yAxis->range();
        ydiff = value - (yrange.lower + yrange.size()/2.0);
        //qDebug() << ydiff;
        m_plot->xAxis->moveRange(xdiff);
        m_plot->yAxis->moveRange(ydiff);
        m_plot->replot();
        qDebug() << "move";
    }
}

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
