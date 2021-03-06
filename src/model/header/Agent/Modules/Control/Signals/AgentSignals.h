#ifndef AGENT_SIGNALS__H
#define AGENT_SIGNALS__H

/*******************************************************
 * @brief Agents can send signals to IAgentSignalHandler
 * subclasses. Signals may or may not carry additional
 * arguments.
 *
 *******************************************************/
struct AgentSignal
{
};

/*******************************************************
 * @brief Sent when agent request a charge from a charger
 * entity.
 *******************************************************/
struct ChargeSignal : public AgentSignal
{
};

/*******************************************************
 * @brief Sent when agent request to pick up a pod from
 * a pod containing entity
 *******************************************************/
struct PickupPodSignal : public AgentSignal
{
};

/*******************************************************
 * @brief Sent when agent request to put down a pod to
 * a pod containing entity
 *******************************************************/
struct PutDownPodSignal : public AgentSignal
{
};

/*******************************************************
 * @brief Sent when agent request to put down an order
 * into an order containing entity
 *******************************************************/
struct PutDownOrderSignal : public AgentSignal
{
};

/*******************************************************
 * @brief Sent when an agent claims a charging station
 * so it can charge itself with it.
 *******************************************************/
struct ClaimChStationSignal : public AgentSignal
{
};

/********************************************************
 * @brief Sent when an agent no longer claims a charging
 * station indicating it is free for claim
 ********************************************************/
struct UnClaimChStationSignal : public AgentSignal
{
};

#endif
