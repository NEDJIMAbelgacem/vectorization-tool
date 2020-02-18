#include "point.h"


PointEntity::PointEntity() : Entity("Point") {
    qDebug() << __FUNCTION__;
    QGraphicsScene* scene = MainWindow::drawing_area->scene();
    QPointF tl = - QPointF(getWidth() / 2.0f, getWidth() / 2.0f);
    QPointF br = + QPointF(getWidth() / 2.0f, getWidth() / 2.0f);
    this->item = scene->addEllipse(QRectF(tl, br), getPen());
    this->item->setZValue(2 * EntitiesManager::getManager()->getEntitiesCount());
}

PointEntity::PointEntity(QDomElement& element) : Entity("Point") {
    float x = element.attribute("X").toFloat();
    float y = element.attribute("Y").toFloat();
    QGraphicsScene* scene = MainWindow::drawing_area->scene();
    QPointF tl = - QPointF(getWidth() / 2.0f, getWidth() / 2.0f);
    QPointF br = + QPointF(getWidth() / 2.0f, getWidth() / 2.0f);
    this->item = scene->addEllipse(QRectF(tl, br), getPen());
    this->setPosition(QPointF(x, y));
    this->setWidth(element.attribute("width").toFloat());
    int r = element.attribute("red").toInt();
    int g = element.attribute("green").toInt();
    int b = element.attribute("blue").toInt();
    int a = element.attribute("alpha").toInt();
    this->setColor(QColor(r, g, b, a));
}

void PointEntity::applyConfig() {
    qDebug() << __FUNCTION__;
    auto* item = getItem();
    qreal w = this->getWidth();
    QPointF tl = - QPointF(w / 2.0f, w / 2.0f);
    QPointF br = + QPointF(w / 2.0f, w / 2.0f);
    item->setPen(getPen());
    item->setRect(QRectF(tl, br));
    if (selection_item != nullptr) {
        QPen pen = selection_item->pen();
        pen.setWidthF(getWidth() * (1 + std::fminf(2.0f * getWidth(), 10.0f) / getWidth()));
        selection_item->setPen(pen);
    }
}

QPen PointEntity::getPen() {
    QPen p;
    p.setColor(config.getConfig<QColor>("color"));
    p.setWidthF(config.getConfig<qreal>("width"));
    return p;
}

void PointEntity::setPosition(QPointF pos) {
    auto* item =  getItem();
    item->setPos(pos);
}

void PointEntity::setConfig(DrawingConfig conf) {
    this->config = conf;
    applyConfig();
}

QGraphicsEllipseItem* PointEntity::getItem() {
    return qgraphicsitem_cast<QGraphicsEllipseItem*>(this->item);
}

QPointF PointEntity::getPoint() {
    return this->getItem()->pos();
}

bool PointEntity::isNeighboursWith(PointEntity* entity, qreal threshold) {
    QPointF point = entity->getItem()->pos();
    QPointF pt = this->getItem()->pos();
    qreal dist = (point.rx() - pt.rx()) * (point.rx() - pt.rx()) + (point.ry() - pt.ry()) * (point.ry() - pt.ry());
    return dist <= threshold * threshold;
}

bool PointEntity::isNeighboursWith(LineEntity* line, qreal threshold) {
    return line->isNeighboursWith(this, threshold);
}

bool PointEntity::isNeighboursWith(PolygonEntity* polygon, qreal threshold) {
    return polygon->isNeighboursWith(this, threshold);
}

bool PointEntity::isNeighboursWith(PolylineEntity* polyline, qreal threshold) {
    return polyline->isNeighboursWith(this, threshold);
}

TopologyCheckResult PointEntity::checkTopologyCondition(LineEntity* entity) {
    switch (TopologyManager::getManager()->getTopologyCondition()) {
    case TopologyCondition::Inside:
        return TopologyCheckResult::Ignore;
    case TopologyCondition::Outside:
        return TopologyCheckResult::Ignore;
    case TopologyCondition::NoTopology:
        return TopologyCheckResult::Accepted;
    case TopologyCondition::Intersect:
    case TopologyCondition::NoIntersect:
        return TopologyCheckResult::Ignore;
    case TopologyCondition::Neighbours: {
            bool nei = isNeighboursWith(entity, TopologyManager::getManager()->getNeighbourhoodThreshold());
            if (nei) return TopologyCheckResult::Accepted;
            return TopologyCheckResult::Rejected;
        }
    case TopologyCondition::NotNeighbours: {
            bool nei = isNeighboursWith(entity, TopologyManager::getManager()->getNeighbourhoodThreshold());
            if (!nei) return TopologyCheckResult::Accepted;
            return TopologyCheckResult::Rejected;
        }
    }
    return TopologyCheckResult::Ignore;
}

TopologyCheckResult PointEntity::checkTopologyCondition(PolygonEntity* entity) {
    switch (TopologyManager::getManager()->getTopologyCondition()) {
    case TopologyCondition::Inside:
        return TopologyCheckResult::Ignore;
    case TopologyCondition::Outside:
        return TopologyCheckResult::Ignore;
    case TopologyCondition::NoTopology:
        return TopologyCheckResult::Accepted;
    case TopologyCondition::Intersect:
    case TopologyCondition::NoIntersect:
        return TopologyCheckResult::Ignore;
    case TopologyCondition::Neighbours: {
            bool nei = isNeighboursWith(entity, TopologyManager::getManager()->getNeighbourhoodThreshold());
            if (nei) return TopologyCheckResult::Accepted;
            return TopologyCheckResult::Rejected;
        }
    case TopologyCondition::NotNeighbours: {
            bool nei = isNeighboursWith(entity, TopologyManager::getManager()->getNeighbourhoodThreshold());
            if (!nei) return TopologyCheckResult::Accepted;
            return TopologyCheckResult::Rejected;
        }
    }
    return TopologyCheckResult::Ignore;
}

TopologyCheckResult PointEntity::checkTopologyCondition(PointEntity* entity) {
    switch (TopologyManager::getManager()->getTopologyCondition()) {
    case TopologyCondition::Inside:
        return TopologyCheckResult::Ignore;
    case TopologyCondition::Outside:
        return TopologyCheckResult::Ignore;
    case TopologyCondition::NoTopology:
        return TopologyCheckResult::Accepted;
    case TopologyCondition::Intersect:
    case TopologyCondition::NoIntersect:
        return TopologyCheckResult::Ignore;
    case TopologyCondition::Neighbours: {
            bool nei = isNeighboursWith(entity, TopologyManager::getManager()->getNeighbourhoodThreshold());
            if (nei) return TopologyCheckResult::Accepted;
            return TopologyCheckResult::Rejected;
        }
    case TopologyCondition::NotNeighbours: {
            bool nei = isNeighboursWith(entity, TopologyManager::getManager()->getNeighbourhoodThreshold());
            if (!nei) return TopologyCheckResult::Accepted;
            return TopologyCheckResult::Rejected;
        }
    }
    return TopologyCheckResult::Ignore;
}

TopologyCheckResult PointEntity::checkTopologyCondition(PolylineEntity* entity) {
    switch (TopologyManager::getManager()->getTopologyCondition()) {
    case TopologyCondition::Inside:
        return TopologyCheckResult::Ignore;
    case TopologyCondition::Outside:
        return TopologyCheckResult::Ignore;
    case TopologyCondition::NoTopology:
        return TopologyCheckResult::Accepted;
    case TopologyCondition::Intersect:
    case TopologyCondition::NoIntersect:
        return TopologyCheckResult::Ignore;
    case TopologyCondition::Neighbours: {
            bool nei = isNeighboursWith(entity, TopologyManager::getManager()->getNeighbourhoodThreshold());
            if (nei) return TopologyCheckResult::Accepted;
            return TopologyCheckResult::Rejected;
        }
    case TopologyCondition::NotNeighbours: {
            bool nei = isNeighboursWith(entity, TopologyManager::getManager()->getNeighbourhoodThreshold());
            if (!nei) return TopologyCheckResult::Accepted;
            return TopologyCheckResult::Rejected;
        }
    }
    return TopologyCheckResult::Ignore;
}

void PointEntity::moveBy(QPointF dp) {
    qDebug() << __FUNCTION__;
    this->getItem()->setPos(this->getItem()->pos() + dp);
    if (selection_item != nullptr) selection_item->setPos(selection_item->pos() + dp);
}

QDomElement PointEntity::toDomElement(QDomDocument &doc) {
    QDomElement element = doc.createElement(this->getType());
    QPointF p = this->getPoint();
    element.setAttribute("X", p.rx());
    element.setAttribute("Y", p.ry());
    element.setAttribute("width", this->getWidth());
    QColor color = this->getColor();
    element.setAttribute("red", color.red());
    element.setAttribute("green", color.green());
    element.setAttribute("blue", color.blue());
    element.setAttribute("alpha", color.alpha());
    return element;
}

void PointEntity::selectedEvent() {
    QGraphicsScene* scene = MainWindow::drawing_area->scene();
    QPointF tl = - QPointF(getWidth() / 2.0f, getWidth() / 2.0f);
    QPointF br = + QPointF(getWidth() / 2.0f, getWidth() / 2.0f);
    this->selection_item = scene->addEllipse(QRectF(tl, br));
    this->selection_item->setPos(this->getItem()->scenePos());
    QPen pen = this->getPen();
    pen.setWidthF(pen.widthF() * (1 + std::fminf(2.0f * pen.widthF(), 10.0f) / pen.widthF()));
    pen.setColor(QColor(0, 0, 255, 200));
    this->selection_item->setPen(pen);
    this->selection_item->setEnabled(false);
    this->selection_item->setZValue(this->getItem()->zValue() - 1);
}

void PointEntity::deselectedEvent() {
    delete this->selection_item;
    this->selection_item = nullptr;
}
