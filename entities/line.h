#pragma once

#include <QLine>
#include <QGraphicsLineItem>
#include <QColor>
#include <QGraphicsEffect>
#include <QGraphicsColorizeEffect>
#include <QGraphicsDropShadowEffect>

#include "entity.h"
#include "entitiesmanager.h"
#include "drawingconfig.h"
#include "topologymanager.h"
#include "mainwindow.h"

class LineEntity : public Entity {
    DrawingConfig config;
    QGraphicsEffect* original_effect = nullptr;
    QGraphicsDropShadowEffect* selected_effect = nullptr;

    QGraphicsLineItem* selection_item = nullptr;
private:
    void applyConfig();
public:
    LineEntity();
    LineEntity(QDomElement& element);

    QPen getPen();

    void setStart(QPointF start);
    void setEnd(QPointF end);
    QPointF getStart();
    QPointF getEnd();

    void setColor(QColor color) override;
    QColor getColor() override { return config.getConfig<QColor>("color"); }
    void setWidth(qreal width) override;
    qreal getWidth() override {return config.getConfig<qreal>("width"); }

    void setConfig(DrawingConfig config);

    QGraphicsLineItem* getItem();

    QLineF getLine();

    bool intersectsWith(LineEntity* line);
    bool intersectsWith(PolygonEntity* polygon);
    bool intersectsWith(PolylineEntity* polyline);

    bool isNeighboursWith(PointEntity* point, qreal threshold);
    bool isNeighboursWith(LineEntity* line, qreal threshold);
    bool isNeighboursWith(PolygonEntity* polygon, qreal threshold);
    bool isNeighboursWith(PolylineEntity* polyline, qreal threshold);

    TopologyCheckResult checkTopologyCondition(LineEntity* entity) override;
    TopologyCheckResult checkTopologyCondition(PolygonEntity* entity) override;
    TopologyCheckResult checkTopologyCondition(PointEntity* entity) override;
    TopologyCheckResult checkTopologyCondition(PolylineEntity* entity) override;

    void moveBy(QPointF dp) override;
    QDomElement toDomElement(QDomDocument &doc) override;

    void selectedEvent() override;
    void deselectedEvent() override;
};
