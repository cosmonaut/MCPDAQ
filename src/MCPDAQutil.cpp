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
        break;
    case gpHot:
        loadPreset(QCPColorGradient::gpHot);
        break;
    case gpCold:
        loadPreset(QCPColorGradient::gpCold);
        break;
    case gpNight:
        loadPreset(QCPColorGradient::gpNight);
        break;
    case gpThermal:
        loadPreset(QCPColorGradient::gpThermal);
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

// To be implemented: currently just a linear scale.
void MCPDAQColorGradient::set_binary_colorstops(void)
{
    set_lin_colorstops();
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
        // not yet implemented.
        set_lin_colorstops();
        //set_binary_colorstops();
        break;
    default:
        set_lin_colorstops();
        break;
    }
}
