#pragma once
#include "entity.h"
#include "mainwindow.h"
#include "entitiesmanager.h"
#include "drawingconfig.h"

class PointEntity;

#include <QRect>
#include <QGraphicsEllipseItem>

class PointEntity : public Entity {
    DrawingConfig config;
private:
    void applyConfig();
    QPen getPen();

    QGraphicsEllipseItem* selection_item = nullptr;
public:
    PointEntity();
    PointEntity(QDomElement& element);

    void setPosition(QPointF pos);
    void setWidth(qreal w) override { this->config.setConfig("width", w); applyConfig(); }
    qreal getWidth() override { return config.getConfig<qreal>("width"); }
    void setColor(QColor color) override { this->config.setConfig("color", color); applyConfig(); }
    QColor getColor() override { return config.getConfig<QColor>("color"); }

    QPointF getPoint();

    void setConfig(DrawingConfig config);

    bool isNeighboursWith(PointEntity* point, qreal threshold);
    bool isNeighboursWith(LineEntity* line, qreal threshold);
    bool isNeighboursWith(PolygonEntity* polygon, qreal threshold);
    bool isNeighboursWith(PolylineEntity* polyline, qreal threshold);

    QGraphicsEllipseItem* getItem();

    void selectedEvent() override;
    void deselectedEvent() override;

    TopologyCheckResult checkTopologyCondition(LineEntity* entity) override;
    TopologyCheckResult checkTopologyCondition(PolygonEntity* entity) override;
    TopologyCheckResult checkTopologyCondition(PointEntity* entity) override;
    TopologyCheckResult checkTopologyCondition(PolylineEntity* entity) override;

    void moveBy(QPointF dp) override;
    QDomElement toDomElement(QDomDocument &doc) override;
};
