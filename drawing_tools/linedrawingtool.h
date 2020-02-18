#pragma once

#include <QDebug>
#include <QMessageBox>

#include "drawingtool.h"
#include "line.h"
#include "mainwindow.h"

class LineEntity;

class LineDrawingTool : public DrawingTool {
private:
    LineEntity* line_entity = nullptr;
    bool drawing = false;
public:
    LineDrawingTool();

    void MousePressed(QGraphicsSceneMouseEvent* event) override;
    void MouseMoved(QGraphicsSceneMouseEvent* event) override;
    void MouseReleased(QGraphicsSceneMouseEvent* event) override;

    void use() override;
    void release() override;
};
