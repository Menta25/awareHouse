#ifndef CONTROLLER_IMPL__H
#define CONTROLLER_IMPL__H

#include "NetworkAdapter.h"
#include <memory>
#include <unordered_map>
#include <map>

// ###################### FORWARD DECLARATIONS ####################### //
class AbstractNetworkMessage;
class SchedulerImpl;
class TaskAssignment;
class PathFinder;
class Node;
class ChargingStation;
// ################################################################### //

class ControllerImpl
{
public:
    ControllerImpl(std::vector<std::shared_ptr<ChargingStation>>* chargingStations = nullptr ,PathFinder *pathFinder = nullptr);
    virtual ~ControllerImpl();

public:
    void tick(int timeStamp);
    void reset();
    bool PlanTask(TaskAssignment *assignment, int timeStamp);
    bool PlanCharge(const AgentControlData &assignment, int timeStamp);
    void translatePath(const std::vector<std::shared_ptr<Node>> &path, int address);
    void registerRoundTrip(const std::vector<std::vector<std::shared_ptr<Node>>> &roundTrip, TaskAssignment *assignment, 
                           int startTime, int waypointCount);

    // ############################ Setter ####################################
    void setPathFinder(PathFinder *pathfinder);
    void setChargingStations(std::vector<std::shared_ptr<ChargingStation>>* chargingStations);

    // ############################ Getter ####################################
    const NetworkAdapter &getNetworkAdapter() const;
    NetworkAdapter &getNetworkAdapter();

private:
    void broadcastMessages(int timeStamp);

private:
    std::vector<std::shared_ptr<ChargingStation>>* chargingStations;
    PathFinder *pathFinder;
    NetworkAdapter networkAdapter;
    std::unordered_multimap<int, TargetedMessage> controlMessages;
private:
    // ################## Messages #############################
    std::shared_ptr<AgentControlGrantedMessage> MControlGranted;
    std::shared_ptr<AgentControlGiveUpMessage> MControlGiveUp;
    std::shared_ptr<ChargeAgentMessage> MChargeAgent;
    std::shared_ptr<MoveAgentMessage> MMoveAgentUp;
    std::shared_ptr<MoveAgentMessage> MMoveAgentDown;
    std::shared_ptr<MoveAgentMessage> MMoveAgentLeft;
    std::shared_ptr<MoveAgentMessage> MMoveAgentRight;
    std::shared_ptr<PickupPodMessage> MPickupPod;
    std::shared_ptr<PutDownPodMessage> MPutdownPod;
    std::shared_ptr<PutDownOrderMessage> MPutdownOrder;
    // #########################################################
    std::map<DirectionVector<>,std::shared_ptr<MoveAgentMessage>> directionToMessage;

    static const int MIN_ENERGY_LEFT = 20;
};

#endif /* CONTROLLER_IMPL__H */