#include "ControllerImpl.h"
#include "NetworkMessage.h"

ControllerImpl::ControllerImpl()
    : MControlGranted(std::make_shared<AgentControlGrantedMessage>(0)),
      MMoveAgentUp(std::make_shared<MoveAgentMessage>(DirectionVector<>::UP(), 0)),
      MMoveAgentDown(std::make_shared<MoveAgentMessage>(DirectionVector<>::DOWN(), 0)),
      MMoveAgentLeft(std::make_shared<MoveAgentMessage>(DirectionVector<>::LEFT(), 0)),
      MMoveAgentRight(std::make_shared<MoveAgentMessage>(DirectionVector<>::RIGHT(), 0)),
      MPickupPod(std::make_shared<PickupPodMessage>(0)),
      MPutdownPod(std::make_shared<PutDownPodMessage>(0)),
      MPutdownOrder(std::make_shared<PutDownOrderMessage>(0))
{
    controlMessages.reserve(500);

    controlMessages.emplace(std::make_pair(0, TargetedMessage(100, MControlGranted)));
    controlMessages.emplace(std::make_pair(0, TargetedMessage(100, MMoveAgentUp)));
    controlMessages.emplace(std::make_pair(1, TargetedMessage(100, MMoveAgentUp)));

    controlMessages.emplace(std::make_pair(0, TargetedMessage(101, MControlGranted)));
    controlMessages.emplace(std::make_pair(0, TargetedMessage(101, MMoveAgentLeft)));
    controlMessages.emplace(std::make_pair(2, TargetedMessage(101, MMoveAgentLeft)));
}

ControllerImpl::~ControllerImpl()
{
    networkAdapter.disconnect();
}

void ControllerImpl::tick(int timeStamp)
{
    broadcastMessages(timeStamp);
}

void ControllerImpl::broadcastMessages(int timeStamp)
{
    auto messages = controlMessages.equal_range(timeStamp);
    for (auto it = messages.first; it != messages.second; ++it)
        networkAdapter.send(it->second.message, it->second.address);

    controlMessages.erase(messages.first, messages.second);
}

const NetworkAdapter &ControllerImpl::getNetworkAdapter() const
{
    return networkAdapter;
}

NetworkAdapter &ControllerImpl::getNetworkAdapter()
{
    return networkAdapter;
}