#pragma once

#include "drawingtool.h"
#include "mainwindow.h"

class HandMovingTool : public DrawingTool
{
    QPointF press_pos;
    bool mouse_pressed = false;
    QPointF center = QPointF(0.0, 0.0);
    QCursor std_cursor;
public:
    HandMovingTool();

    void MousePressed(QGraphicsSceneMouseEvent* event) override;
    void MouseMoved(QGraphicsSceneMouseEvent* event) override;
    void MouseReleased(QGraphicsSceneMouseEvent* event) override;

    void use() override;

    void release() override;
};
