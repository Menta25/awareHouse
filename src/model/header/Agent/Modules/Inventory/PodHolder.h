#ifndef POD_HOLDER__H
#define POD_HOLDER__H

#include "IContaining.h"
#include "OrderModel.h"
#include "Pod.h"
#include <memory>

// ####################### FORWARD DECLARATIONS ########################
// #####################################################################

/******************************************************************************
 * @brief This entity can store One pod which stores orders.
 * eg. delivery robot has this module so it can pick up and
 * transfer pods and then put them down.
 ******************************************************************************/
class PodHolder : public IContaining<std::unique_ptr<Pod<OrderModel>>>
{
public:
    PodHolder() = default;
    PodHolder(PodHolder &&other) = default;
    virtual ~PodHolder() = default;

    PodHolder(const PodHolder &other) = delete;
    PodHolder &operator=(const PodHolder &other) = delete;

public:
    /***************************************************************************************
     * @brief Don't use this function because unique pointer cant be reassigned without move.
     ***************************************************************************************/
    virtual void push(const std::unique_ptr<Pod<OrderModel>> &) override;

    /**************************************************************
     * @brief The PodHolder acquires the provided pod
     * item will be set to nullptr.
     * @throws runtime error if holder already has a pod.
     **************************************************************/
    virtual void push(std::unique_ptr<Pod<OrderModel>> &&item) override;

    /******************************************************************
     * @brief Removes and returns a pod if the holder has one.
     * std::nullopt returned when there's no associated pod.
     ******************************************************************/
    virtual std::optional<std::unique_ptr<Pod<OrderModel>>> pop(const std::unique_ptr<Pod<OrderModel>> &) override;

    /**************************************************************
     * @brief Returns whether the PodHolder has an associated pod
     **************************************************************/
    virtual bool empty() const override;

    /***************************************************************
     * @brief Returns the associated pod if the holder has one.
     * Returns nullptr if the holder has no pod.
     ***************************************************************/
    const std::unique_ptr<Pod<OrderModel>> &getChildPod() const;
    std::unique_ptr<Pod<OrderModel>> &getChildPod();

private:
    std::unique_ptr<Pod<OrderModel>> pod;
};

#endif /* POD_HOLDER__H */
