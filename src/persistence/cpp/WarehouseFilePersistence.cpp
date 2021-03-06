#include "WarehouseFilePersistence.h"
#include "ChargingStationLoader.h"
#include "DeliveryStationLoader.h"
#include "Pod.h"
#include "PodDockLoader.h"
#include "Point.h"
#include "RobotLoader.h"
#include "State.h"
#include <QFile>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonValue>
#include <memory>
#include <vector>

WarehouseFilePersistence::WarehouseFilePersistence() {}
WarehouseFilePersistence::~WarehouseFilePersistence() {}

State *WarehouseFilePersistence::load(const QString &resource, const Settings *settings)
{
    QString jsonString;
    QFile sourceFile(resource);
    if (sourceFile.exists())
        if (sourceFile.open(QIODevice::ReadOnly | QIODevice::Text))
        {
            jsonString = sourceFile.readAll();
            sourceFile.close();

            QJsonDocument warehouseDoc = QJsonDocument::fromJson(jsonString.toUtf8());
            QJsonObject warehouseJsonObj = warehouseDoc.object();
            return loadFromJsonObject(warehouseJsonObj, settings);
        }
    return nullptr;
}

bool WarehouseFilePersistence::save(const State &state, const QString &resource) const
{
    QFile saveFile(resource);
    if (!saveFile.open(QIODevice::WriteOnly))
        return false;

    // Object root
    QJsonObject saveObject;

    // Primary child
    QJsonObject warehouseLayoutData;

    warehouseLayoutData.insert("RowCount", state.getRowCount());
    warehouseLayoutData.insert("ColCount", state.getColCount());

    // Write Charging Stations
    QJsonArray chargingStations;
    for (const auto &station : state.getChargingStations())
        chargingStations.append(ChargingStationLoader::save(*station));

    warehouseLayoutData.insert("ChargingStations", chargingStations);

    // Write PodDocks
    QJsonArray podDocks;
    for (const auto &podDock : state.getPodDocks())
        podDocks.append(PodDockLoader::save(*podDock));

    warehouseLayoutData.insert("PodDocks", podDocks);

    // Write Robots
    QJsonArray robots;
    for (const auto &robot : state.getRobots())
        robots.append(RobotLoader::save(*robot));

    warehouseLayoutData.insert("DeliveryRobots", robots);

    saveObject.insert("WarehouseLayoutData", warehouseLayoutData);
    saveFile.write(QJsonDocument(saveObject).toJson());
    return true;
}

State *WarehouseFilePersistence::loadFromJsonObject(QJsonObject json, const Settings *settings)
{

    if (json.contains("WarehouseLayoutData") && json["WarehouseLayoutData"].isObject())
    {
        QJsonObject WarehouseLayoutData = json["WarehouseLayoutData"].toObject();
        int rowSize;
        int colSize;

        // Read Row and Column
        if (WarehouseLayoutData.contains("RowCount") && WarehouseLayoutData["RowCount"].isDouble() &&
            WarehouseLayoutData.contains("ColCount") && WarehouseLayoutData["ColCount"].isDouble())
        {
            rowSize = WarehouseLayoutData["RowCount"].toInt();
            colSize = WarehouseLayoutData["ColCount"].toInt();
        }

        // Create Environment
        std::shared_ptr<ObservableNavEnvironment> env = std::make_shared<ObservableNavEnvironment>(rowSize, colSize, 3);
        std::vector<std::shared_ptr<ChargingStation>> chStations;
        std::vector<std::shared_ptr<DeliveryStation>> deliveryStations;
        std::vector<std::shared_ptr<PodDock>> podDocks;
        std::vector<Pod<OrderModel> *> pods;
        std::vector<std::shared_ptr<DeliveryRobot>> robots;

        loadChargingStation(chStations, env, WarehouseLayoutData);
        loadDeliveryStation(deliveryStations, env, WarehouseLayoutData);
        loadPodDock(podDocks, pods, env, WarehouseLayoutData);
        loadRobots(robots, env, WarehouseLayoutData, settings);

        // Construct State from read data
        return new State(std::move(env),
                         std::move(chStations),
                         std::move(deliveryStations),
                         std::move(podDocks),
                         std::move(pods),
                         std::move(robots),
                         rowSize, colSize);
    }
    return nullptr;
}

void WarehouseFilePersistence::loadChargingStation(std::vector<std::shared_ptr<ChargingStation>> &chStations,
                                                   std::shared_ptr<ObservableNavEnvironment> &env,
                                                   QJsonObject &warehouseLayoutData)
{
    if (warehouseLayoutData.contains("ChargingStations") && warehouseLayoutData["ChargingStations"].isArray())
    {
        QJsonArray chargingStationsJson = warehouseLayoutData["ChargingStations"].toArray();
        chStations.reserve(chargingStationsJson.size());

        for (int i = 0; i < chargingStationsJson.size(); ++i)
        {
            std::shared_ptr<ChargingStation> station = ChargingStationLoader::load(chargingStationsJson[i].toObject());

            env->getBuffer()[env->getCoordAsIndex(station->getPosition())] = station;
            chStations.push_back(station);
        }
    }
}

void WarehouseFilePersistence::loadDeliveryStation(std::vector<std::shared_ptr<DeliveryStation>> &deliveryStations,
                                                   std::shared_ptr<ObservableNavEnvironment> &env,
                                                   QJsonObject &warehouseLayoutData)
{
    if (warehouseLayoutData.contains("DeliveryStations") && warehouseLayoutData["DeliveryStations"].isArray())
    {
        QJsonArray deliveryStationsJson = warehouseLayoutData["DeliveryStations"].toArray();
        deliveryStations.reserve(deliveryStationsJson.size());

        for (int i = 0; i < deliveryStationsJson.size(); ++i)
        {
            std::shared_ptr<DeliveryStation> station = DeliveryStationLoader::load(deliveryStationsJson[i].toObject());

            env->getBuffer()[env->getCoordAsIndex(station->getPosition())] = station;
            deliveryStations.push_back(station);
        }
    }
}

void WarehouseFilePersistence::loadPodDock(std::vector<std::shared_ptr<PodDock>> &podDocks,
                                           std::vector<Pod<OrderModel> *> &pods,
                                           std::shared_ptr<ObservableNavEnvironment> &env,
                                           QJsonObject &warehouseLayoutData)
{
    if (warehouseLayoutData.contains("PodDocks") && warehouseLayoutData["PodDocks"].isArray())
    {
        QJsonArray PodDocksJSon = warehouseLayoutData["PodDocks"].toArray();
        podDocks.reserve(PodDocksJSon.size());

        for (int i = 0; i < PodDocksJSon.size(); ++i)
        {
            std::shared_ptr<PodDock> podDock = PodDockLoader::load(PodDocksJSon[i].toObject());

            env->getBuffer()[env->getCoordAsIndex(podDock->getPosition())] = podDock;
            podDock->addAssociatedPod(env);
            pods.push_back(podDock->getPodHolder().getChildPod().get());
            PodDockLoader::loadOrders(PodDocksJSon[i].toObject(), *podDock->getPodHolder().getChildPod());

            podDocks.push_back(podDock);
        }
    }
}

void WarehouseFilePersistence::loadRobots(std::vector<std::shared_ptr<DeliveryRobot>> &robots,
                                          std::shared_ptr<ObservableNavEnvironment> &env,
                                          QJsonObject &warehouseLayoutData,
                                          const Settings *settings)
{
    if (warehouseLayoutData.contains("DeliveryRobots") && warehouseLayoutData["DeliveryRobots"].isArray())
    {
        QJsonArray RobotsArray = warehouseLayoutData["DeliveryRobots"].toArray();
        robots.reserve(RobotsArray.size());

        for (int i = 0; i < RobotsArray.size(); ++i)
            robots.push_back(RobotLoader::load(RobotsArray[i].toObject(), env, settings));
    }
}
