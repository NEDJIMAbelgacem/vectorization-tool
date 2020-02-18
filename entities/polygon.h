#pragma once

#include <QGraphicsPolygonItem>

#include "entity.h"
#include "mainwindow.h"
#include "drawingconfig.h"
#include "utilities.h"

class PolygonEntity : public Entity {
    DrawingConfig config;
    QGraphicsPolygonItem* selection_item = nullptr;
public:
    PolygonEntity();
    PolygonEntity(QDomElement& element);
    ~PolygonEntity();

    bool intersectsWith(LineEntity* line);
    bool intersectsWith(PolygonEntity* polygon);
    bool intersectsWith(PolylineEntity* polyline);
    bool contains(PointEntity* point);
    bool contains(LineEntity* line);
    bool contains(PolygonEntity* polygon);
    bool contains(PolylineEntity* polyline);

    void setConfig(DrawingConfig conf);
    DrawingConfig getConfig();
    QPen getPen();
    void setWidth(qreal w) override { this->config.setConfig("width", w); applyConfig(); }
    qreal getWidth() override { return config.getConfig<qreal>("width"); }
    void setColor(QColor color) override { this->config.setConfig("color", color); applyConfig(); }
    QColor getColor() override { return config.getConfig<QColor>("color"); }

    void applyConfig();

    bool isNeighboursWith(PointEntity* point, qreal threshold);
    bool isNeighboursWith(LineEntity* line, qreal threshold);
    bool isNeighboursWith(PolygonEntity* polygon, qreal threshold);
    bool isNeighboursWith(PolylineEntity* polyline, qreal threshold);

    void addPoint(QPointF point);
    void setLastPoint(QPointF point);
    QPointF getLastPoint();
    void eraseLastPoint();
    QGraphicsPolygonItem* getItem();

    QPolygonF getPolygon() {
        QPolygonF polygon = this->getItem()->polygon();
        polygon = this->getItem()->mapToScene(polygon);
        return polygon;
    }

    QVector<QPointF> getPointsVector();
    QVector<QLineF> getLinesVector();

    TopologyCheckResult checkTopologyCondition(LineEntity* entity) override;
    TopologyCheckResult checkTopologyCondition(PolygonEntity* entity) override;
    TopologyCheckResult checkTopologyCondition(PointEntity* entity) override;
    TopologyCheckResult checkTopologyCondition(PolylineEntity* entity) override;

    void moveBy(QPointF dp) override;
    QDomElement toDomElement(QDomDocument &doc);

    void selectedEvent() override;
    void deselectedEvent() override;
};
