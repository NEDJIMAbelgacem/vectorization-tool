#include "polygondrawingtool.h"

PolygonDrawingTool::PolygonDrawingTool() : DrawingTool("polygon drawer", DrawingStyle::Clicking) {
    qDebug() << __FUNCTION__ << "polygon drawer" << " " << this->style;
    clicks_count = 0;
}

void PolygonDrawingTool::MousePressed(QGraphicsSceneMouseEvent* event) {
    qDebug() << __FUNCTION__;
    QPointF p = event->scenePos();
    if (event->button() != Qt::MouseButton::LeftButton) {
        if (polygon_entity != nullptr) {
            // Ended creation
            QPointF last_point = polygon_entity->getLastPoint();
            polygon_entity->eraseLastPoint();
            TopologyManager::getManager()->setDrawingStage(DrawingStage::Submitting);
            if (!TopologyManager::getManager()->isAcceptedTopology(polygon_entity)) {
                // Alert user error
                QMessageBox msgBox;
                msgBox.setText("Invalid topology");
                msgBox.exec();
                polygon_entity->addPoint(last_point);
                return;
            }
            if (clicks_count >= 3) polygon_entity->sendToManager();
            else delete polygon_entity;
            polygon_entity = nullptr;
        }
        return;
    }
    if (polygon_entity != nullptr) {
        clicks_count++;
        polygon_entity->setLastPoint(p);
        polygon_entity->addPoint(p);
        TopologyManager::getManager()->setDrawingStage(DrawingStage::Drawing);
        if (!TopologyManager::getManager()->isAcceptedTopology(polygon_entity)) {
            // Alert user error
            QMessageBox msgBox;
            msgBox.setText("Invalid topology");
            msgBox.exec();
            polygon_entity->eraseLastPoint();
            return;
        }
    } else {
        if (event->button() != Qt::MouseButton::LeftButton) return;
        clicks_count = 1;
        polygon_entity = new PolygonEntity;
        polygon_entity->addPoint(p);
        polygon_entity->addPoint(p);
        TopologyManager::getManager()->setDrawingStage(DrawingStage::Instansing);
        if (!TopologyManager::getManager()->isAcceptedTopology(polygon_entity)) {
            // Alert user error
            QMessageBox msgBox;
            msgBox.setText("Invalid topology");
            msgBox.exec();
            delete polygon_entity;
            polygon_entity = nullptr;
            return;
        }
        polygon_entity->setConfig(*MainWindow::drawing_config);
    }
}

void PolygonDrawingTool::MouseMoved(QGraphicsSceneMouseEvent* event) {
    if (polygon_entity == nullptr) return;
    QPointF p = event->scenePos();
    polygon_entity->setLastPoint(p);
}

void PolygonDrawingTool::MouseReleased(QGraphicsSceneMouseEvent* event) {
    qDebug() << __FUNCTION__ << " " << style;
}
