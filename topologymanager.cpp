#include "topologymanager.h"

TopologyManager::TopologyManager()
{

}

TopologyManager* TopologyManager::manager = nullptr;

TopologyManager* TopologyManager::getManager() {
    if (manager == nullptr) manager = new TopologyManager;
    return manager;
}


void TopologyManager::setSelectedEntity(Entity* entity) {
    this->selected_entity = entity;
}

Entity* TopologyManager::getSelectedEntity() {
    return selected_entity;
}

void TopologyManager::setTopologyCondition(TopologyCondition condition) {
    this->topology_condition = condition;
}

TopologyCondition TopologyManager::getTopologyCondition() {
    return topology_condition;
}

void TopologyManager::setDrawingStage(DrawingStage stage) {
    this->drawing_stage = stage;
}
DrawingStage TopologyManager::getDrawingStage() {
    return  drawing_stage;
}

bool TopologyManager::isAcceptedTopology(LineEntity* entity) {
    qDebug() << __FUNCTION__ << "(Line)";
    if (topology_condition == TopologyCondition::NoTopology) return true;
    Entity* selected_entity = getSelectedEntity();
    if (selected_entity == nullptr) return true;
    TopologyCheckResult res = selected_entity->checkTopologyCondition(entity);
    if (res == TopologyCheckResult::Rejected) return false;
    return true;
}

bool TopologyManager::isAcceptedTopology(PointEntity* entity) {
    qDebug() << __FUNCTION__ << "(Point)";
    if (topology_condition == TopologyCondition::NoTopology) return true;
    Entity* selected_entity = getSelectedEntity();
    if (selected_entity == nullptr) return true;
    TopologyCheckResult res = selected_entity->checkTopologyCondition(entity);
    if (res == TopologyCheckResult::Rejected) return false;
    return true;
}

bool TopologyManager::isAcceptedTopology(PolygonEntity* entity) {
    qDebug() << __FUNCTION__ << "(Polygon)";
    if (topology_condition == TopologyCondition::NoTopology) return true;
    Entity* selected_entity = getSelectedEntity();
    if (selected_entity == nullptr) return true;
    TopologyCheckResult res = selected_entity->checkTopologyCondition(entity);
    if (res == TopologyCheckResult::Rejected) return false;
    return true;
}

bool TopologyManager::isAcceptedTopology(PolylineEntity* entity) {
    qDebug() << __FUNCTION__ << "(Polyline)";
    if (topology_condition == TopologyCondition::NoTopology) return true;
    Entity* selected_entity = getSelectedEntity();
    qDebug() << selected_entity;
    if (selected_entity == nullptr) return true;
    qDebug() << selected_entity;
    TopologyCheckResult res = selected_entity->checkTopologyCondition(entity);
    if (res == TopologyCheckResult::Rejected) return false;
    return true;
}


int TopologyManager::getNeighbourhoodThreshold() {
    return neighbourhood_threshhold;
}

void TopologyManager::setNeighbourhoodThreshold(int threshold) {
    this->neighbourhood_threshhold = threshold;
}
