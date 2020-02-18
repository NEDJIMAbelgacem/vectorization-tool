#pragma once

#include <QMouseEvent>
#include <QGraphicsScene>
#include <QDebug>
#include <QString>
#include <QGraphicsSceneMouseEvent>

#include "topologymanager.h"

enum DrawingStyle {
    DragNDrop = 1,
    Clicking = 2,
    None = 3
};

class DrawingTool
{
public:
    QString tool_name;
    DrawingStyle style = DrawingStyle::DragNDrop;
protected:
    DrawingTool(QString _tool_name, DrawingStyle _style);
public:
    virtual void MousePressed(QGraphicsSceneMouseEvent* event);
    virtual void MouseMoved(QGraphicsSceneMouseEvent* event);
    virtual void MouseReleased(QGraphicsSceneMouseEvent* event);

    virtual void use();
    virtual void release();
};
