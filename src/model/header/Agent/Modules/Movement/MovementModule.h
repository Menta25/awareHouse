#ifndef MOVEMENT_MODULE__H
#define MOVEMENT_MODULE__H

#include "IMoveMechanism.h"
#include <memory>

class MovementModule
{
public:
    explicit MovementModule(std::unique_ptr<IMoveMechanism> &&moveMechanism)
        : moveMechanism(std::move(moveMechanism))
    {
    }

public:
    // GETTER
    const IMoveMechanism *getMoveMechanism() const { return moveMechanism.get(); }
    IMoveMechanism *getMoveMechanism() { return moveMechanism.get(); }

    // SETTER
    void setMoveMechanism(std::unique_ptr<IMoveMechanism> &&moveMechanism) { this->moveMechanism = std::move(moveMechanism); }

private:
    std::unique_ptr<IMoveMechanism> moveMechanism;
};

#endif /* ROBOT_MOVEMENT_MODULE__H */