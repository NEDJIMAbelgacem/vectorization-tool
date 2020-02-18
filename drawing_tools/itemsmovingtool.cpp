#include "itemsmovingtool.h"

ItemsMovingTool::ItemsMovingTool() : DrawingTool("ItemsMovingTool", DrawingStyle::None) { }

void ItemsMovingTool::MousePressed(QGraphicsSceneMouseEvent* event) {
    qDebug() << __FUNCTION__;
    if (event->button() != Qt::MouseButton::LeftButton) return;
    QPointF p = event->scenePos();
    press_pos = p;
    QGraphicsItem* selected_item = MainWindow::drawing_scene->itemAt(p, MainWindow::drawing_area->transform());
    Entity* selected = EntitiesManager::getManager()->getEntity(selected_item);
    TopologyManager::getManager()->setSelectedEntity(selected);
    mouse_pressed = true;
}

void ItemsMovingTool::MouseMoved(QGraphicsSceneMouseEvent* event) {
    qDebug() << __FUNCTION__;
    if (!mouse_pressed) return;
    QPointF p = event->scenePos();
    QPointF dp = p - press_pos;
    press_pos = p;
    Entity* entity = TopologyManager::getManager()->getSelectedEntity();
    if (entity == nullptr) return;
    entity->moveBy(dp);
}

void ItemsMovingTool::MouseReleased(QGraphicsSceneMouseEvent* event) {
    qDebug() << __FUNCTION__;
    if (event->button() != Qt::MouseButton::LeftButton) return;
    mouse_pressed = false;
}
