#include "drawingtool.h"


DrawingTool::DrawingTool(QString _tool_name, DrawingStyle _style) : tool_name(_tool_name), style(_style) {
    qDebug() << __FUNCTION__ << " " << _tool_name << " " << _style << " " << style;
}

void DrawingTool::MousePressed(QGraphicsSceneMouseEvent* event) {
    qDebug() << __FUNCTION__;
}

void DrawingTool::MouseMoved(QGraphicsSceneMouseEvent* event) {
//    qDebug() << __FUNCTION__;
}

void DrawingTool::MouseReleased(QGraphicsSceneMouseEvent* event) {
    qDebug() << __FUNCTION__;
}


void DrawingTool::use() {
    qDebug() << __FUNCTION__;
}

void DrawingTool::release() {
    qDebug() << __FUNCTION__;
}
