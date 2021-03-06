#ifndef STATE__H
#define STATE__H

#include "Agent.h"
#include "ChargingStation.h"
#include "DeliveryRobot.h"
#include "DeliveryStation.h"
#include "ObservableEnvironment.h"
#include "PathFinder.h"
#include "PodDock.h"
#include "TaskManager.h"
#include <memory>
// ######################## FORWARD DECLARATIONS ######################### //
// ###################################################################### //
/*************************************************************************
 * @brief A wrapper class for changing objects.
 **************************************************************************/
class State
{
public:
    //Construct With Copy
    State(const std::shared_ptr<ObservableNavEnvironment> &environment,
          const std::vector<std::shared_ptr<ChargingStation>> &chStations,
          const std::vector<std::shared_ptr<DeliveryStation>> &deliveryStations,
          const std::vector<std::shared_ptr<PodDock>> &podDocks,
          const std::vector<Pod<OrderModel> *> &pods,
          const std::vector<std::shared_ptr<DeliveryRobot>> &robots,
          int rowCount, int colCount);

    //Construct With Move (preferred)
    State(std::shared_ptr<ObservableNavEnvironment> &&environment,
          std::vector<std::shared_ptr<ChargingStation>> &&chStations,
          std::vector<std::shared_ptr<DeliveryStation>> &&deliveryStations,
          std::vector<std::shared_ptr<PodDock>> &&podDocks,
          std::vector<Pod<OrderModel> *> &&pods,
          std::vector<std::shared_ptr<DeliveryRobot>> &&robots,
          int rowCount, int colCount);

    State(const State &other) = delete;
    State &operator=(const State &other) = delete;
    virtual ~State();

public:
    int getRowCount() const;
    int getColCount() const;

    const std::vector<std::shared_ptr<ChargingStation>> &getChargingStations() const;
    std::vector<std::shared_ptr<ChargingStation>> &getChargingStations();
    
    const std::vector<std::shared_ptr<PodDock>> &getPodDocks() const;
    const std::vector<Pod<OrderModel> *> &getPods() const;
    const std::vector<std::shared_ptr<DeliveryStation>> &getDeliveryStations() const;
    const std::vector<std::shared_ptr<DeliveryRobot>> &getRobots() const;

    const PathFinder &getPathFinder() const;
    PathFinder &getPathFinder();

    const TaskManager &getTaskManager() const;
    TaskManager &getTaskManager();

public:
    void tick(int timeStamp);

private:
    size_t rowCount;
    size_t colCount;

    std::shared_ptr<ObservableNavEnvironment> environment;
    std::vector<std::shared_ptr<ChargingStation>> chStations;
    std::vector<std::shared_ptr<PodDock>> podDocks;
    std::vector<Pod<OrderModel> *> pods; // Pod-ot az ownere deleteli
    std::vector<std::shared_ptr<DeliveryStation>> deliveryStations;
    std::vector<std::shared_ptr<DeliveryRobot>> robots;

    PathFinder pathFinder;
    TaskManager taskManager;
};

#endif /* STATE__H */
