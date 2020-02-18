#pragma once

#include "drawingtool.h"
#include "polyline.h"

class PolylineEntity;

class PolylineDrawingTool : public DrawingTool
{
public:
    PolylineEntity* polyline_entity = nullptr;
    int clicks_count;
public:
    PolylineDrawingTool();

    void MousePressed(QGraphicsSceneMouseEvent* event) override;
    void MouseMoved(QGraphicsSceneMouseEvent* event) override;
    void MouseReleased(QGraphicsSceneMouseEvent* event) override;
};
