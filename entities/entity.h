#pragma once

#include <QGraphicsItem>
#include <QGraphicsEffect>
#include <QDomDocument>

#include "entitiesmanager.h"
#include "topologymanager.h"

enum TopologyCondition;
enum TopologyCheckResult {
    Accepted,
    Rejected,
    Ignore
};

class LineEntity;
class PolygonEntity;
class PointEntity;
class PolylineEntity;

class Entity {
protected:
    QGraphicsItem* item;
    QString type;
public:
    Entity(QString type);
    ~Entity();

    void sendToManager();
    QString getType();
    QGraphicsItem* getItem();

    virtual void selectedEvent();
    virtual void deselectedEvent();

    virtual TopologyCheckResult checkTopologyCondition(LineEntity* entity) = 0;
    virtual TopologyCheckResult checkTopologyCondition(PolygonEntity* entity) = 0;
    virtual TopologyCheckResult checkTopologyCondition(PointEntity* entity) = 0;
    virtual TopologyCheckResult checkTopologyCondition(PolylineEntity* entity) = 0;
    virtual void moveBy(QPointF dp) = 0;
    virtual QDomElement toDomElement(QDomDocument& doc);

    virtual void setColor(QColor color) { };
    virtual QColor getColor() { return QColor(); }
    virtual void setWidth(qreal width) { };
    virtual qreal getWidth() { return 1.0; }
};
