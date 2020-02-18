#pragma once

#include "drawingtool.h"
#include "polygon.h"

class PolygonEntity;

class PolygonDrawingTool : public DrawingTool
{
public:
    PolygonEntity* polygon_entity = nullptr;
    int clicks_count;
public:
    PolygonDrawingTool();

    void MousePressed(QGraphicsSceneMouseEvent* event) override;

    void MouseMoved(QGraphicsSceneMouseEvent* event) override;

    void MouseReleased(QGraphicsSceneMouseEvent* event) override;
};
