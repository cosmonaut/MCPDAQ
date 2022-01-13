#ifndef MCPDAQUTIL_H
#define MCPDAQUTIL_H

#include "qcustomplot.h"


// Color gradient class to just add several desired color gradients to what is already available in QCustomPlot
class MCPDAQColorGradient : public QCPColorGradient
{
public:
    enum MCPGradPreset {viridis,
                        cividis,
                        magma,
                        inferno,
                        plasma,
                        parula,
                        cubehelix,
                        gpGrayscale,
                        gpHot,
                        gpCold,
                        gpNight,
                        gpThermal};
    Q_ENUM(MCPGradPreset);

    MCPDAQColorGradient();
    MCPDAQColorGradient(MCPGradPreset preset);

    void set_preset(MCPGradPreset preset);
};


#endif // MCPDAQUTIL_H
