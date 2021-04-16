#include "ChargingStationList.h"

ChargingStationList::ChargingStationList(QObject* parent)
    : QObject(parent)
{}

QList<const ChargingStationPresenter*>* ChargingStationList::chargingStations() { return &m_chargingStations; }

bool ChargingStationList::setChargingStationAt(int index, const ChargingStationPresenter& chargingStation)
{
    if (index < 0 ||
        index >= m_chargingStations.size())
        return false;

    m_chargingStations[index] = &chargingStation;
    return true;
}

void ChargingStationList::appendChargingStation(const ChargingStationPresenter& chargingStation)
{
    emit preItemAppended();
    m_chargingStations.append(&chargingStation);
    emit postItemAppended();
}

void ChargingStationList::removeChargingStation(int index)
{
    if (index < 0 ||
        index >= m_chargingStations.size())
        return;

    emit preItemRemoved(index);
    m_chargingStations.removeAt(index);
    emit postItemRemoved();
}

void ChargingStationList::clear()
{
    emit preItemRemoved(0);
    m_chargingStations.clear();
    emit postItemRemoved();
}