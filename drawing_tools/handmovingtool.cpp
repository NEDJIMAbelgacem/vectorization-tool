#include "handmovingtool.h"

HandMovingTool::HandMovingTool() : DrawingTool("HandTool", DrawingStyle::None) { }

void HandMovingTool::MousePressed(QGraphicsSceneMouseEvent* event) {
    if (event->button() != Qt::MouseButton::LeftButton) return;
    QPointF p = event->scenePos();
    press_pos = p;
    mouse_pressed = true;
    MainWindow::drawing_area->setCursor(QCursor(Qt::CursorShape::ClosedHandCursor));
}

void HandMovingTool::MouseMoved(QGraphicsSceneMouseEvent* event) {
    if (!mouse_pressed) return;
    QPointF p = event->scenePos();
    QPointF dp = p - press_pos;
    press_pos = p;
    center += dp;
    for (QGraphicsItem* item : MainWindow::drawing_scene->items()) {
        item->setPos(item->pos() + dp);
    }
}

void HandMovingTool::MouseReleased(QGraphicsSceneMouseEvent* event) {
    if (event->button() != Qt::MouseButton::LeftButton) return;
    mouse_pressed = false;
    MainWindow::drawing_area->setCursor(QCursor(Qt::CursorShape::OpenHandCursor));
}

void HandMovingTool::use() {
    std_cursor = MainWindow::drawing_area->cursor();
    MainWindow::drawing_area->setCursor(QCursor(Qt::CursorShape::OpenHandCursor));
}

void HandMovingTool::release() {
    MainWindow::drawing_area->setCursor(std_cursor);
}
