#ifndef MCPDAQETHERDAQIFACE_H
#define MCPDAQETHERDAQIFACE_H

#include <QObject>
#include <QUdpSocket>

#include "MCPDAQutil.h"
#include "mcpdaq_types.h"

class MCPDAQEtherDaqIface : public QObject
{
    Q_OBJECT
public:
    explicit MCPDAQEtherDaqIface(QObject *parent = nullptr);

    void open(void);
    void close(void);

    void set_ec(etherdaq_config_t *);
    etherdaq_config_t get_ec(void);

    void parse_pkt(QByteArray);

signals:
    void valid_data(const QList<photon_t> &data);

private slots:
    void rx(void);

private:
    QUdpSocket *m_udp_sock;
    etherdaq_config_t *m_ec;
    quint16 last_pnum;
    quint32 pkt_slips;
};

#endif // MCPDAQETHERDAQIFACE_H
