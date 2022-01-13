#include "MCPDAQutil.h"
#define TINYCOLORMAP_WITH_QT5
#include "tinycolormap.hpp"


// why even have this??
MCPDAQColorGradient::MCPDAQColorGradient()
{

}


MCPDAQColorGradient::MCPDAQColorGradient(MCPGradPreset preset)
{
    set_preset(preset);
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
        // NOT YET IMPLEMENTED
        loadPreset(QCPColorGradient::gpGrayscale);
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
