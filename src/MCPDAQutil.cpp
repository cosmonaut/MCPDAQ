#include "MCPDAQutil.h"
#define TINYCOLORMAP_WITH_QT5
#include "tinycolormap.hpp"

// default constructor
MCPDAQColorGradient::MCPDAQColorGradient()
{
    set_preset(MCPDAQColorGradient::cubehelix);
}

// constructor with preset
MCPDAQColorGradient::MCPDAQColorGradient(MCPGradPreset preset)
{
    set_preset(preset);
}

MCPDAQColorGradient::~MCPDAQColorGradient()
{

}

void MCPDAQColorGradient::set_preset(MCPGradPreset preset)
{
    int i = 0;
    float f = 0.0;
    const QCPRange range = QCPRange(0.0, 1.0);
    QMap<double, QColor> cmap;

    clearColorStops();
    switch(preset) {
    case viridis:
        setColorInterpolation(ciRGB);
        for (i = 0; i < 256; i++) {
            f = i/255.0;
            const QColor color = tinycolormap::GetColor(f, tinycolormap::ColormapType::Viridis).ConvertToQColor();
            setColorStopAt(f, color);
        }
        break;
    case cividis:
        setColorInterpolation(ciRGB);
        for (i = 0; i < 256; i++) {
            f = i/255.0;
            const QColor color = tinycolormap::GetColor(f, tinycolormap::ColormapType::Cividis).ConvertToQColor();
            setColorStopAt(f, color);
        }
        break;
    case magma:
        setColorInterpolation(ciRGB);
        for (i = 0; i < 256; i++) {
            f = i/255.0;
            const QColor color = tinycolormap::GetColor(f, tinycolormap::ColormapType::Magma).ConvertToQColor();
            setColorStopAt(f, color);
        }
        break;
    case inferno:
        setColorInterpolation(ciRGB);
        for (i = 0; i < 256; i++) {
            f = i/255.0;
            const QColor color = tinycolormap::GetColor(f, tinycolormap::ColormapType::Inferno).ConvertToQColor();
            setColorStopAt(f, color);
        }
        break;
    case plasma:
        setColorInterpolation(ciRGB);
        for (i = 0; i < 256; i++) {
            f = i/255.0;
            const QColor color = tinycolormap::GetColor(f, tinycolormap::ColormapType::Plasma).ConvertToQColor();
            setColorStopAt(f, color);
        }
        break;
    case parula:
        setColorInterpolation(ciRGB);
        for (i = 0; i < 256; i++) {
            f = i/255.0;
            const QColor color = tinycolormap::GetColor(f, tinycolormap::ColormapType::Parula).ConvertToQColor();
            setColorStopAt(f, color);
        }
        break;
    case cubehelix:
        setColorInterpolation(ciRGB);
        for (i = 0; i < 256; i++) {
            f = i/255.0;
            const QColor color = tinycolormap::GetColor(f, tinycolormap::ColormapType::Cubehelix).ConvertToQColor();
            setColorStopAt(f, color);
        }
        break;
    case gpGrayscale:
        loadPreset(QCPColorGradient::gpGrayscale);
        for (i = 0; i < 256; i++) {
            f = i/255.0;
            QColor c = QColor(color(f, range, false));
            cmap.insert(f, c);
        }
        clearColorStops();
        setColorStops(cmap);
        break;
    case gpHot:
        loadPreset(QCPColorGradient::gpHot);
        for (i = 0; i < 256; i++) {
            f = i/255.0;
            QColor c = QColor(color(f, range, false));
            cmap.insert(f, c);
        }
        clearColorStops();
        setColorStops(cmap);
        break;
    case gpCold:
        loadPreset(QCPColorGradient::gpCold);
        for (i = 0; i < 256; i++) {
            f = i/255.0;
            QColor c = QColor(color(f, range, false));
            cmap.insert(f, c);
        }
        clearColorStops();
        setColorStops(cmap);
        break;
    case gpNight:
        loadPreset(QCPColorGradient::gpNight);
        for (i = 0; i < 256; i++) {
            f = i/255.0;
            QColor c = QColor(color(f, range, false));
            cmap.insert(f, c);
        }
        clearColorStops();
        setColorStops(cmap);
        break;
    case gpThermal:
        loadPreset(QCPColorGradient::gpThermal);
        for (i = 0; i < 256; i++) {
            f = i/255.0;
            QColor c = QColor(color(f, range, false));
            cmap.insert(f, c);
        }
        clearColorStops();
        setColorStops(cmap);
        break;
    }
}

void MCPDAQColorGradient::set_lin_colorstops(void)
{
    QList<QColor> cvals;
    int n;
    int i;
    double f;

    // The original set of linearly spaced color values.
    cvals = colorStops().values();
    n = cvals.size();

    // Clear the current color stops
    clearColorStops();

    // Set new color stops with log spacing...
    for (i = 0; i < n; i++) {
        f = double(i)/(n - 1.0);
        setColorStopAt(f, cvals[i]);
    }
}

void MCPDAQColorGradient::set_log_colorstops(void)
{
    QList<QColor> cvals;
    int n;
    int i;
    double f;
    double r;
    //double a = 10.0;

    //double d = log(a + 1.0);

    // The original set of linearly spaced color values.
    cvals = colorStops().values();
    n = cvals.size();

    // Clear the current color stops
    clearColorStops();

    // Set new color stops with log spacing...
    for (i = 0; i < n; i++) {
        // 0 - 1
        r = double(i)/(n - 1.0);
        f = pow(10.0, r - 1.0);
        //f = pow(10.0, ((double)i)/(n - 1.0) - 1.0);

        // Push low end all the way down
        if (i == 0) {
            setColorStopAt(0.0, cvals[i]);
        } else {
            setColorStopAt(f, cvals[i]);
        }
    }
}

void MCPDAQColorGradient::set_sqrt_colorstops(void)
{
    QList<QColor> cvals;
    int n;
    int i;
    double f;

    // The original set of linearly spaced color values.
    cvals = colorStops().values();
    n = cvals.size();

    // Clear the current color stops
    clearColorStops();

    // Set new color stops with log spacing...
    for (i = 0; i < n; i++) {
        //f = pow(10.0, ((double)i)/(n - 1.0) - 1.0);
        f = sqrt((double)i/(n - 1.0));
        setColorStopAt(f, cvals[i]);
    }
}

void MCPDAQColorGradient::set_squared_colorstops(void)
{
    QList<QColor> cvals;
    int n;
    int i;
    double f;

    // The original set of linearly spaced color values.
    cvals = colorStops().values();
    n = cvals.size();

    // Clear the current color stops
    clearColorStops();

    // Set new color stops with log spacing...
    for (i = 0; i < n; i++) {
        f = pow(((double)i)/(n - 1.0), 2.0);
        //f = sqrt((double)i/(n - 1.0));
        setColorStopAt(f, cvals[i]);
    }
}

// Somewhat of a hack -- this hides all of the gradient in a tiny portion near
// 0 but has the desired effect of showing any pixel as fullbright if there are
// more than 0 photons in that pixel
void MCPDAQColorGradient::set_binary_colorstops(void)
{
    QList<QColor> cvals;

    int n;
    int i;
    double f;

    cvals = colorStops().values();
    n = cvals.size();

    clearColorStops();

    for (i = 1; i < n - 1; i++) {
        f = (double)i*1e-7;
        setColorStopAt(f, cvals[i]);
    }
    setColorStopAt(0.0, cvals[0]);
    setColorStopAt(1.0, cvals[n - 1]);
}

void MCPDAQColorGradient::setScaling(MCPGradScaling scaling)
{
    switch(scaling) {
    case MCPDAQColorGradient::LIN:
        set_lin_colorstops();
        break;
    case MCPDAQColorGradient::LOG:
        set_log_colorstops();
        break;
    case MCPDAQColorGradient::SQRT:
        set_squared_colorstops();
        break;
    case MCPDAQColorGradient::BIN:
        set_binary_colorstops();
        break;
    default:
        set_lin_colorstops();
        break;
    }
}
