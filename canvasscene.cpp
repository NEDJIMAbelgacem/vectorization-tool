#include "canvasscene.h"

CanvasScene::CanvasScene(QObject* parent) : QGraphicsScene(parent) {
    qDebug() << __FUNCTION__;
}

void CanvasScene::init() {
    qDebug() << __FUNCTION__;
}

void CanvasScene::setDrawingTool(DrawingTool* tool) {
    qDebug() << __FUNCTION__ << " " << tool->tool_name;
    if (used_tool != nullptr) {
        used_tool->release();
    }
    this->used_tool = tool;
    used_tool->use();
}

void CanvasScene::mouseMoveEvent(QGraphicsSceneMouseEvent* event) {
//    qDebug() << __FUNCTION__;
    QPointF pos = event->scenePos();
    QString pos_text = QString("(%1, %2)").arg(pos.rx()).arg(pos.ry());
    MainWindow::mouse_position_label->setText(pos_text);
    if (mouse_pressed) {
        if (!mouse_pressed) return;
        QPointF p = event->scenePos();
        QPointF dp = p - press_pos;
        press_pos = p;
        center += dp;
        for (QGraphicsItem* item : MainWindow::drawing_scene->items()) {
            item->setPos(item->pos() + dp);
        }
        std_cursor = MainWindow::drawing_area->cursor();
        MainWindow::drawing_area->setCursor(QCursor(Qt::CursorShape::ClosedHandCursor));
        return;
    }
    if (used_tool == nullptr) return;
    used_tool->MouseMoved(event);
}

void CanvasScene::mousePressEvent(QGraphicsSceneMouseEvent* event) {
    qDebug() << __FUNCTION__;
    if (event->button() == Qt::MouseButton::MidButton) {
        QPointF p = event->scenePos();
        press_pos = p;
        mouse_pressed = true;
        return;
    }
    if (used_tool == nullptr) return;
    used_tool->MousePressed(event);
}

void CanvasScene::mouseReleaseEvent(QGraphicsSceneMouseEvent* event) {
    qDebug() << __FUNCTION__;
    if (event->button() == Qt::MouseButton::MidButton) {
        qDebug() << "Release";
        mouse_pressed = false;
        MainWindow::drawing_area->setCursor(QCursor(Qt::CursorShape::ArrowCursor));
        return;
    }
    if (used_tool == nullptr) return;
    used_tool->MouseReleased(event);
}

void CanvasScene::keyPressEvent(QKeyEvent *event) {
    qDebug() << __FUNCTION__;
    if (event->key() == Qt::Key::Key_A) {
        qDebug() << TopologyManager::getManager()->getSelectedEntity();
    }
    if (event->key() == Qt::Key_0) {
        MainWindow::window->switchDrawingTool();
    }
}

