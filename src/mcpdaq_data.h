#ifndef MCPDAQ_DATA_H
#define MCPDAQ_DATA_H

#include <QObject>
#include <QTimer>

#include "mcpdaq_types.h"

class mcpdaq_data : public QObject
{
    Q_OBJECT
public:
    explicit mcpdaq_data(QObject *parent = nullptr);

public slots:
    void append_data(const QList<photon_t> &data);
    void monitor_rate(bool);
    void update_cr(void);

signals:
    void count_rate(double);

private:
    QList<photon_t> *photon_data;
    QTimer *rate_timer;

    size_t last_count;
    size_t current_count;
    size_t crate;
};

#endif // MCPDAQ_DATA_H
