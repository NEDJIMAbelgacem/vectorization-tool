#pragma once
#include <QPixmap>
#include <QGraphicsPixmapItem>

#include "mainwindow.h"
#include "entities/entity.h"

class ImageEntity : public Entity {
private:
    QString path;
    QGraphicsRectItem* selection_item = nullptr;
public:
    ImageEntity(QString path);
    ImageEntity(QDomElement& element);

    QGraphicsPixmapItem* getItem();
    void moveBy(QPointF dp) override;
    QDomElement toDomElement(QDomDocument &doc) override;

    TopologyCheckResult checkTopologyCondition(LineEntity* entity) override { return TopologyCheckResult::Ignore; }
    TopologyCheckResult checkTopologyCondition(PolygonEntity* entity) override { return TopologyCheckResult::Ignore; }
    TopologyCheckResult checkTopologyCondition(PointEntity* entity) override  { return TopologyCheckResult::Ignore; }
    TopologyCheckResult checkTopologyCondition(PolylineEntity* entity) override  { return TopologyCheckResult::Ignore; }

    void selectedEvent() override;
    void deselectedEvent() override;
};
