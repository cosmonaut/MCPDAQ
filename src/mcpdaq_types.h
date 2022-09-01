#ifndef MCPDAQ_TYPES_H
#define MCPDAQ_TYPES_H

typedef struct {
    QHostAddress addr;
    quint16 port;
    quint16 pkt_len;
    quint8 xbits;
    quint8 ybits;
    quint8 pbits;
    double aratio;
} etherdaq_config_t;

typedef struct {
    quint16 x;
    quint16 y;
    quint16 p;
} photon_t;

#endif // MCPDAQ_TYPES_H
