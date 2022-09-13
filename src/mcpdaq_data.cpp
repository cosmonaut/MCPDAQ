#include "mcpdaq_data.h"

mcpdaq_data::mcpdaq_data(QObject *parent)
    : QObject{parent}
{
    // Data object.
    photon_data = new QList<photon_t>();

    // Initialize counts trackers.
    last_count = 0;
    current_count = 0;
    crate = 0;
}

// Append data to data object (for saving to fits files etc.)
void mcpdaq_data::append_data(const QList<photon_t> &data)
{
    photon_data->append(data);
}

// Compute latest count rate
void mcpdaq_data::update_cr(void)
{
    current_count = photon_data->size();
    crate = current_count - last_count;
    last_count = current_count;
    emit count_rate((double)crate);
    qDebug() << crate;
}
