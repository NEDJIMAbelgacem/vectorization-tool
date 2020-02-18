#pragma once

#include <QGraphicsPolygonItem>

#include "entity.h"
#include "mainwindow.h"
#include "drawingconfig.h"

class PolylineEntity : public Entity {
    DrawingConfig config;
    QPointF start_point;
    QVector<QPointF> polyline_points;
    QPainterPath constructPainterPath();

    QGraphicsPathItem* selection_item = nullptr;
public:
    PolylineEntity();
    PolylineEntity(QDomElement& element);

    bool intersectsWith(LineEntity* line);
    bool intersectsWith(PolygonEntity* polygon);
    bool intersectsWith(PolylineEntity* polyline);

    bool isNeighboursWith(PointEntity* point, qreal threshold);
    bool isNeighboursWith(LineEntity* line, qreal threshold);
    bool isNeighboursWith(PolygonEntity* polygon, qreal threshold);
    bool isNeighboursWith(PolylineEntity* polyline, qreal threshold);

    void setConfig(DrawingConfig conf);
    DrawingConfig getConfig();
    QPen getPen();
    void setWidth(qreal w) override { this->config.setConfig("width", w); applyConfig(); }
    qreal getWidth() override { return config.getConfig<qreal>("width"); }
    void setColor(QColor color) override { this->config.setConfig("color", color); applyConfig(); }
    QColor getColor() override { return config.getConfig<QColor>("color"); }

    QVector<QPointF> getPointsVector();
    QVector<QLineF> getLinesVector();

    void applyConfig();

    void setFirstPoint(QPointF p);
    void addPoint(QPointF point);
    void setLastPoint(QPointF point);
    QPointF getLastPoint();
    void eraseLastPoint();
    QGraphicsPathItem* getItem();

    TopologyCheckResult checkTopologyCondition(LineEntity* entity) override;
    TopologyCheckResult checkTopologyCondition(PolygonEntity* entity) override;
    TopologyCheckResult checkTopologyCondition(PointEntity* entity) override;
    TopologyCheckResult checkTopologyCondition(PolylineEntity* entity) override;

    void moveBy(QPointF dp) override;
    QDomElement toDomElement(QDomDocument &doc) override;

    void selectedEvent() override;
    void deselectedEvent() override;
};
