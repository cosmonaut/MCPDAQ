#include "mcpdaqetherdaqiface.h"
#include <QNetworkDatagram>


MCPDAQEtherDaqIface::MCPDAQEtherDaqIface(QObject *parent)
    : QObject{parent}
{
    // etherdaq configuration struct
    m_ec = new etherdaq_config_t();

    // defaults
    m_ec->addr = QHostAddress("192.168.1.5");
    m_ec->port = 5555;
    m_ec->pkt_len = 1458;
    m_ec->xbits = 12;
    m_ec->ybits = 12;
    m_ec->pbits = 8;
    m_ec->aratio = 1.0;

    m_udp_sock = new QUdpSocket(this);

    // connect UDP socket readyRead
    connect(m_udp_sock, &QUdpSocket::readyRead, this, &MCPDAQEtherDaqIface::rx);
}


void MCPDAQEtherDaqIface::open(void)
{
    // bind socket
    m_udp_sock->bind(QHostAddress::LocalHost, m_ec->port);
}

void MCPDAQEtherDaqIface::close(void)
{
    // close socket
    m_udp_sock->close();
}

void MCPDAQEtherDaqIface::set_ec(etherdaq_config_t *ec)
{
    // Configure etherdaq interface
    m_ec = ec;
    qDebug() << m_ec->addr;
}

etherdaq_config_t MCPDAQEtherDaqIface::get_ec(void)
{
    etherdaq_config_t ec = *m_ec;
    return(ec);
}

void MCPDAQEtherDaqIface::rx(void)
{
    while (m_udp_sock->hasPendingDatagrams()) {
        QNetworkDatagram datagram = m_udp_sock->receiveDatagram();
        // Check anything here before passing to parse
        parse_pkt(datagram.data());
    }
}

void MCPDAQEtherDaqIface::parse_pkt(QByteArray data)
{
    // Cast to 16-bit words (in our transmitted order)
    quint16_le* p = (quint16_le*)data.data();
    // max length of packet in uint16s
    int plen = (int)(data.size())/sizeof(quint16_le);

    quint16 nphot = p[0];
    quint16 pnum = p[1];

    quint16 xmask = 0xffff >> (16 - m_ec->xbits);
    quint16 ymask = 0xffff >> (16 - m_ec->ybits);
    quint16 pmask = 0xffff >> (16 - m_ec->pbits);

    //qDebug() << nphot << " " << pnum;

    if (nphot > 244) {
        nphot = 244;
    }

    if (nphot > plen - 3) {
        nphot = plen - 3;
        qDebug() << "WARNING: packet size issue";
    }

    // Photon list.
    QList<photon_t> pdat(nphot);

//    for (int i = 0; i < plen; i++) {
//        qDebug() << i << Qt::hex <<  p[i];
//    }

    for (int i = 0; i < nphot; i++) {
        pdat[i].x = p[i*3 + 3] & xmask;
        pdat[i].y = p[i*3 + 4] & ymask;
        pdat[i].p = p[i*3 + 5] & pmask;
    }

    emit valid_data(pdat);

    //qDebug() << pdat;
//    for (int i = 0; i < nphot; i++) {
//        qDebug() << pdat[i].x;
//    }
}
