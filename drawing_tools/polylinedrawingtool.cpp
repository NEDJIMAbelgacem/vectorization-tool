#include "polylinedrawingtool.h"

PolylineDrawingTool::PolylineDrawingTool() : DrawingTool("polyline drawer", DrawingStyle::Clicking) {
    qDebug() << __FUNCTION__ << "polyline drawer" << " " << this->style;
    clicks_count = 0;
}

void PolylineDrawingTool::MousePressed(QGraphicsSceneMouseEvent* event) {
    qDebug() << __FUNCTION__;
    QPointF p = event->scenePos();
    if (event->button() != Qt::MouseButton::LeftButton) {
        if (polyline_entity != nullptr) {
            QPointF last_point = polyline_entity->getLastPoint();
            polyline_entity->eraseLastPoint();
            TopologyManager::getManager()->setDrawingStage(DrawingStage::Submitting);
            if (!TopologyManager::getManager()->isAcceptedTopology(polyline_entity)) {
                // Alert user error
                QMessageBox msgBox;
                msgBox.setText("Invalid topology");
                msgBox.exec();
                polyline_entity->addPoint(last_point);
                return;
            }
            if (clicks_count >= 2) polyline_entity->sendToManager();
            else delete polyline_entity;
            polyline_entity = nullptr;
        }
        return;
    }
    if (polyline_entity != nullptr) {
        clicks_count++;
        polyline_entity->setLastPoint(p);
        polyline_entity->addPoint(p);
        TopologyManager::getManager()->setDrawingStage(DrawingStage::Drawing);
        if (!TopologyManager::getManager()->isAcceptedTopology(polyline_entity)) {
            // Alert user error
            QMessageBox msgBox;
            msgBox.setText("Invalid topology");
            msgBox.exec();
            polyline_entity->eraseLastPoint();
            return;
        }
    } else {
        if (event->button() != Qt::MouseButton::LeftButton) return;
        clicks_count = 1;
        polyline_entity = new PolylineEntity;
        polyline_entity->setFirstPoint(p);
        polyline_entity->addPoint(p);
        TopologyManager::getManager()->setDrawingStage(DrawingStage::Instansing);
        if (!TopologyManager::getManager()->isAcceptedTopology(polyline_entity)) {
            // Alert user error
            QMessageBox msgBox;
            msgBox.setText("Invalid topology");
            msgBox.exec();
            delete polyline_entity;
            polyline_entity = nullptr;
            return;
        }
        polyline_entity->setConfig(*MainWindow::drawing_config);
    }
}

void PolylineDrawingTool::MouseMoved(QGraphicsSceneMouseEvent* event) {
    if (polyline_entity == nullptr) return;
    QPointF p = event->scenePos();
    polyline_entity->setLastPoint(p);
}

void PolylineDrawingTool::MouseReleased(QGraphicsSceneMouseEvent* event) {
    qDebug() << __FUNCTION__ << " " << style;
}
