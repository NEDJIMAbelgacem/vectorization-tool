#pragma once

#include "drawingtool.h"
#include "mainwindow.h"

class ItemsMovingTool : public DrawingTool {
    QPointF press_pos;
    bool mouse_pressed = false;
public:
    ItemsMovingTool();

    void MousePressed(QGraphicsSceneMouseEvent* event) override;
    void MouseMoved(QGraphicsSceneMouseEvent* event) override;
    void MouseReleased(QGraphicsSceneMouseEvent* event) override;
};
