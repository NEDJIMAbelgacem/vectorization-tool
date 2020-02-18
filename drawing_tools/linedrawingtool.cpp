#include "linedrawingtool.h"

LineDrawingTool::LineDrawingTool() : DrawingTool("line drawer", DrawingStyle::DragNDrop) {
    qDebug() << __FUNCTION__ << "line drawer DrawingStyle::DragNDrop";
}

void LineDrawingTool::MousePressed(QGraphicsSceneMouseEvent* event) {
    qDebug() << __FUNCTION__;
    QPointF p = event->scenePos();
    TopologyManager* manager = TopologyManager::getManager();
    if (event->button() != Qt::MouseButton::LeftButton && line_entity != nullptr) {
        delete line_entity;
        line_entity = nullptr;
        return;
    }
    if (line_entity != nullptr) {
        line_entity->setEnd(p);
        TopologyManager::getManager()->setDrawingStage(DrawingStage::Submitting);
        if (!manager->isAcceptedTopology(line_entity)) {
            // Alert user error
            QMessageBox msgBox;
            msgBox.setText("Invalid topology");
            msgBox.exec();
            return;
        }
        line_entity->sendToManager();
        line_entity = nullptr;
    } else {
        if (event->button() != Qt::MouseButton::LeftButton) return;
        line_entity = new LineEntity;
        line_entity->setStart(p);
        line_entity->setEnd(p);
        TopologyManager::getManager()->setDrawingStage(DrawingStage::Instansing);
        if (!manager->isAcceptedTopology(line_entity)) {
            // Alert user error
            QMessageBox msgBox;
            msgBox.setText("Invalid topology");
            msgBox.exec();
            delete line_entity;
            line_entity = nullptr;
            return;
        }
        line_entity->setConfig(*MainWindow::drawing_config);
    }
}

void LineDrawingTool::MouseMoved(QGraphicsSceneMouseEvent* event) {
    if (line_entity == nullptr) return;
    QPointF p = event->scenePos();
    line_entity->setEnd(p);
}

void LineDrawingTool::MouseReleased(QGraphicsSceneMouseEvent* event) {
    qDebug() << __FUNCTION__ << " " << style;
}

void LineDrawingTool::use() {
    qDebug() << __FUNCTION__;
}

void LineDrawingTool::release() {
    qDebug() << __FUNCTION__;
}
