#include "Battery.h"

Battery::Battery(int maxCharge, int maxChargeRate, int timesUsed)
    : maxCharge(maxCharge),
      maxChargeRate(maxChargeRate),
      currentCharge(maxCharge),
      timesUsed(timesUsed)
{
    if (maxCharge < 0)
        this->maxCharge = 0;

    condition = 1;
    this->maxCharge *= condition;
}

void Battery::charge(int energy)
{
    if (currentCharge + energy >= maxCharge)
    {
        currentCharge = maxCharge;
        IDepleting::onChargeChanged(currentCharge);
    }
    else
    {
        currentCharge += energy;
        IDepleting::onChargeChanged(currentCharge);
    }

    condition = degrade(++timesUsed);
    maxCharge *= condition;
}

void Battery::deplete(int energy)
{
    if (currentCharge - energy < 0)
    {
        currentCharge = 0;
        IDepleting::onChargeChanged(currentCharge);
        throw typename IDepleting::EnergyDepletedException("No energy left in battery.");
    }
    else
    {
        currentCharge -= energy;
        IDepleting::onChargeChanged(currentCharge);
        IDepleting::onDepleted(energy);
    }

    condition = degrade(++timesUsed);
    maxCharge *= condition;
}
double Battery::degrade(int timesUsed) const
{
    return IDepleting::NoDegradation;
}

int Battery::getCharge() const { return currentCharge; }
int Battery::getMaxCharge() const { return maxCharge; }
int Battery::getTimesUsed() const { return timesUsed; }
double Battery::getCondition() const { return condition; }
int Battery::getMaxChargeRate() const { return maxChargeRate; }
