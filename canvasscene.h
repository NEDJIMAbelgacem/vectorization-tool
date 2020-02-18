#pragma once

#include <QGraphicsScene>
#include <QMouseEvent>

#include "drawingtool.h"
#include "entitiesmanager.h"
#include "mainwindow.h"

class CanvasScene : public QGraphicsScene
{
private:
    bool mouse_pressed = false;
    QPointF press_pos;
    QPointF center = QPointF(0.0, 0.0);
    QCursor std_cursor;
public:
    CanvasScene();
    CanvasScene(QObject* parent);
    void init();

    void mouseMoveEvent(QGraphicsSceneMouseEvent* event) override;
    void mousePressEvent(QGraphicsSceneMouseEvent* event) override;
    void mouseReleaseEvent(QGraphicsSceneMouseEvent* event) override;

    void keyPressEvent(QKeyEvent *event) override;

    void setDrawingTool(DrawingTool* tool);
private:
    DrawingTool* used_tool = nullptr;
    bool mouse_activated = false;
};

