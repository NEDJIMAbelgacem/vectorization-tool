#pragma once

#include "entity.h"

enum TopologyCondition {
    NoTopology,
    Neighbours,
    NotNeighbours,
    Inside,
    Outside,
    Intersect,
    NoIntersect
};

enum DrawingStage {
    Instansing,
    Drawing,
    Submitting
};

class Entity;
class LineEntity;
class PointEntity;
class PolygonEntity;
class PolylineEntity;

class TopologyManager {
private:
    TopologyManager();
    static TopologyManager* manager;

    Entity* selected_entity = nullptr;
    TopologyCondition topology_condition = TopologyCondition::NoTopology;
    DrawingStage drawing_stage = DrawingStage::Drawing;
    int neighbourhood_threshhold = 5;
public:
    static TopologyManager* getManager();

    void setSelectedEntity(Entity* entity);
    void setTopologyCondition(TopologyCondition condition);
    TopologyCondition getTopologyCondition();
    Entity* getSelectedEntity();
    DrawingStage getDrawingStage();
    void setDrawingStage(DrawingStage stage);

    int getNeighbourhoodThreshold();
    void setNeighbourhoodThreshold(int threshold);

    bool isAcceptedTopology(LineEntity* entity);
    bool isAcceptedTopology(PointEntity* entity);
    bool isAcceptedTopology(PolygonEntity* entity);
    bool isAcceptedTopology(PolylineEntity* entity);
};

