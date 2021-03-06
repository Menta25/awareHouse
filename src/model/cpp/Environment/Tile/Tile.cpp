#include "Tile.h"
#include "IVolumeOccupant.h"

Tile::Tile(const Point &pos) : position(pos), occupant(nullptr)
    {}

Tile::Tile(const Tile  &other)
    : position(other.position),
      occupant(other.occupant)
    {}

Tile::Tile(      Tile  &&other)
    : position(std::move(other.position)),
      occupant(std::move(other.occupant))
    {}

Tile::~Tile() = default;

void Tile::occupyV(const Tile::OccupantType &occupant)
{
    if (this->occupant != nullptr && this->occupant != occupant)
        throw Tile::VolumeCollisionException("Volume collision happened", this->occupant, occupant);

    this->occupant = occupant;
}

void Tile::freeV()
{
    occupant = nullptr;
}

bool Tile::isVolumeFree() const
{
    return occupant == nullptr;
}

const Tile::OccupantType &Tile::getOccupant() const
{
    return occupant;
}

const Tile::Point &Tile::getPosition() const
{
    return position;
}