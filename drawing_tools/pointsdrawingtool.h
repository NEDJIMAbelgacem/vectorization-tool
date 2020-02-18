#pragma once

#include "drawingtool.h"
#include "mainwindow.h"
#include "point.h"

struct PointConfig;

class PointsDrawingTool : public DrawingTool
{
public:
    PointsDrawingTool();

    void MousePressed(QGraphicsSceneMouseEvent* event) override;
};
