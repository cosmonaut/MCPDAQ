#ifndef MCPDAQUTIL_H
#define MCPDAQUTIL_H

//#include <QtGlobal>
#include <QHostAddress>

#include "qcustomplot.h"


typedef struct {
    QHostAddress addr;
    quint16 port;
    quint16 pkt_len;
    quint8 xbits;
    quint8 ybits;
    quint8 pbits;
    double aratio;
} etherdaq_config_t;


// Color gradient class to just add several desired color gradients to what is already available in QCustomPlot
class MCPDAQColorGradient : public QCPColorGradient
{
    Q_GADGET
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

    enum MCPGradScaling {LIN,
                         LOG,
                         SQRT,
                         BIN};
    Q_ENUM(MCPGradScaling);

    MCPDAQColorGradient();
    MCPDAQColorGradient(MCPGradPreset preset);
    ~MCPDAQColorGradient();

    void set_preset(MCPGradPreset preset);

    void setScaling(MCPGradScaling scaling);

    void set_lin_colorstops(void);
    void set_log_colorstops(void);
    void set_sqrt_colorstops(void);
    void set_squared_colorstops(void);
    void set_binary_colorstops(void);

private:

};


#endif // MCPDAQUTIL_H
