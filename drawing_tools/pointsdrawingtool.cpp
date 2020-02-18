#include "pointsdrawingtool.h"

PointsDrawingTool::PointsDrawingTool() : DrawingTool("PointsTool", DrawingStyle::None) { }

void PointsDrawingTool::MousePressed(QGraphicsSceneMouseEvent* event) {
    if (event->button() != Qt::MouseButton::LeftButton) return;
    QPointF p = event->scenePos();
    PointEntity* point = new PointEntity;
    point->setPosition(p);
    TopologyManager::getManager()->setDrawingStage(DrawingStage::Instansing);
    if (!TopologyManager::getManager()->isAcceptedTopology(point)) {
        // Alert user error
        QMessageBox msgBox;
        msgBox.setText("Invalid topology");
        msgBox.exec();
        delete point;
        point = nullptr;
        return;
    }
    point->setConfig(*MainWindow::drawing_config);
    point->sendToManager();
}
