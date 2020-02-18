#include "polygon.h"

PolygonEntity::PolygonEntity() : Entity("Polygon") {
    qDebug() << __FUNCTION__;
    QGraphicsScene* scene = MainWindow::drawing_area->scene();
    this->item = scene->addPolygon(QPolygonF());
    this->item->setZValue(2 * EntitiesManager::getManager()->getEntitiesCount());
    qDebug() << this->item->zValue();
}

PolygonEntity::PolygonEntity(QDomElement& element) : Entity("Polygon") {
    Q_ASSERT(element.tagName() == this->getType());
    QGraphicsScene* scene = MainWindow::drawing_area->scene();
    this->item = scene->addPolygon(QPolygonF());
    QDomNodeList lst = element.childNodes();
    for (int i = 0; i < lst.size(); ++i) {
        QDomElement node = lst.at(i).toElement();
        if (node.tagName() != "prim_point") continue;
        float x = node.attribute("X").toFloat();
        float y = node.attribute("Y").toFloat();
        QPointF point(x, y);
        this->addPoint(point);
    }
    this->setWidth(element.attribute("width").toFloat());
    int r = element.attribute("red").toInt();
    int g = element.attribute("green").toInt();
    int b = element.attribute("blue").toInt();
    int a = element.attribute("alpha").toInt();
    this->setColor(QColor(r, g, b, a));
}

PolygonEntity::~PolygonEntity() { }

QPen PolygonEntity::getPen() {
    QPen p;
    p.setColor(config.getConfig<QColor>("color"));
    p.setWidthF(config.getConfig<qreal>("width"));
    return p;
}

void PolygonEntity::addPoint(QPointF point) {
    qDebug() << __FUNCTION__;
    auto* item = getItem();
    QPolygonF p = item->polygon();
    p.push_back(point);
    item->setPolygon(p);
}

void PolygonEntity::setLastPoint(QPointF point) {
//        qDebug() << __FUNCTION__;
    auto* item = getItem();
    QPolygonF p = item->polygon();
    assert(!p.empty());
    p.pop_back();
    p.push_back(point);
    item->setPolygon(p);
}

QPointF PolygonEntity::getLastPoint() {
    return this->getItem()->polygon().last();
}

void PolygonEntity::eraseLastPoint() {
    qDebug() << __FUNCTION__;
    auto* item = getItem();
    QPolygonF p = item->polygon();
    assert(!p.empty());
    p.pop_back();
    item->setPolygon(p);
}

void PolygonEntity::applyConfig() {
    qDebug() << __FUNCTION__;
    QGraphicsPolygonItem* polygon_item = getItem();
    polygon_item->setPen(getPen());
    polygon_item->setBrush(QBrush(this->getColor()));
    if (selection_item != nullptr) {
        QPen pen = selection_item->pen();
        pen.setWidthF(getWidth() * (1 + std::fminf(2.0f * getWidth(), 10.0f) / getWidth()));
        selection_item->setPen(pen);
    }
}

QGraphicsPolygonItem* PolygonEntity::getItem() {
    return qgraphicsitem_cast<QGraphicsPolygonItem*>(this->item);
}

void PolygonEntity::setConfig(DrawingConfig conf) {
    this->config = conf;
    applyConfig();
}

DrawingConfig PolygonEntity::getConfig() {
    return config;
}

bool PolygonEntity::intersectsWith(LineEntity* line_entity) {
    QVector<QLineF> polygon_lines = this->getLinesVector();
    QLineF line1 = line_entity->getLine();
    for (QLineF line2 : polygon_lines) {
        if (doIntersect(line1, line2)) return true;
    }
    return false;
}

bool PolygonEntity::intersectsWith(PolygonEntity* polygon) {
    QVector<QLineF> p1_lines = this->getLinesVector(), p2_lines = polygon->getLinesVector();
    for (QLineF l1 : p1_lines) {
        for (QLineF l2 : p2_lines) {
            if (doIntersect(l1, l2)) return true;
        }
    }
    return false;
}

bool PolygonEntity::intersectsWith(PolylineEntity* polyline_entity) {
    QVector<QLineF> p1_lines = this->getLinesVector(), p2_lines = polyline_entity->getLinesVector();
    for (QLineF l1 : p1_lines) {
        for (QLineF l2 : p2_lines) {
            if (doIntersect(l1, l2)) return true;
        }
    }
    return false;
}

bool PolygonEntity::contains(PointEntity* point) {
    QPolygonF polygon = this->getPolygon();
    return polygon.containsPoint(point->getPoint(), this->getItem()->fillRule());
}

bool PolygonEntity::contains(LineEntity* line) {
    QPolygonF polygon = this->getPolygon();
    return polygon.containsPoint(line->getStart(), this->getItem()->fillRule())
            &&
            polygon.containsPoint(line->getEnd(), this->getItem()->fillRule());
}

bool PolygonEntity::contains(PolygonEntity* polygon) {
    QPolygonF pol = this->getPolygon();
    for (QPointF p : polygon->getPolygon()) {
        if (!pol.containsPoint(p, this->getItem()->fillRule())) return false;
    }
    return true;
}

bool PolygonEntity::contains(PolylineEntity* polyline) {
    for (QPointF p : polyline->getPointsVector()) {
        if (!this->getPolygon().containsPoint(p, this->getItem()->fillRule())) return false;
    }
    return true;
}

QVector<QPointF> PolygonEntity::getPointsVector() {
    QVector<QPointF> points;
    QPolygonF polygon = this->getPolygon();
    for (int i = 0; i < polygon.size(); ++i) points.push_back(polygon[i]);
    return points;
}

QVector<QLineF> PolygonEntity::getLinesVector() {
    QVector<QLineF> lines;
    QPolygonF polygon = this->getPolygon();
    for (int i = 0; i < polygon.size(); ++i) lines.push_back(QLineF(polygon[i], polygon[(i + 1) % polygon.size()]));
    return lines;
}

bool PolygonEntity::isNeighboursWith(PointEntity* point, qreal threshold) {
    return areNeighbours(this->getPolygon(), point->getPoint(), threshold);
}

bool PolygonEntity::isNeighboursWith(LineEntity* line, qreal threshold) {
    QLineF l = line->getLine();
    for (QLineF pline : this->getLinesVector()) {
        if (areNeighbours(pline, l.p1(), threshold) && areNeighbours(pline, l.p2(), threshold)) {
            return true;
        }
    }
    return false;
}

bool PolygonEntity::isNeighboursWith(PolygonEntity* polygon, qreal threshold) {
    for (QPointF p : polygon->getPointsVector()) {
        if (!areNeighbours(this->getPolygon(), p, threshold)) return false;
    }
    return true;
}

bool PolygonEntity::isNeighboursWith(PolylineEntity* polyline, qreal threshold) {
    qDebug() << __FUNCTION__;
    for (QPointF p : polyline->getPointsVector()) {
        if (!areNeighbours(this->getPolygon(), p, threshold)) return false;
    }
    return true;
}

TopologyCheckResult PolygonEntity::checkTopologyCondition(LineEntity* entity) {
    qDebug() << __FUNCTION__ << "(LineEntity)";
    switch (TopologyManager::getManager()->getTopologyCondition()) {
    case TopologyCondition::Inside:
        if (this->contains(entity)) return TopologyCheckResult::Accepted;
        return TopologyCheckResult::Rejected;
    case TopologyCondition::Outside:
        if (!this->contains(entity)) return TopologyCheckResult::Rejected;
        return TopologyCheckResult::Accepted;
    case TopologyCondition::NoTopology:
        return TopologyCheckResult::Accepted;
    case TopologyCondition::Intersect: {
            if (TopologyManager::getManager()->getDrawingStage() != DrawingStage::Submitting) {
                return TopologyCheckResult::Accepted;
            }
            if (intersectsWith(entity)) return TopologyCheckResult::Accepted;
            return TopologyCheckResult::Rejected;
        }
    case TopologyCondition::NoIntersect: {
            if (TopologyManager::getManager()->getDrawingStage() != DrawingStage::Submitting) {
                return TopologyCheckResult::Accepted;
            }
            if (!intersectsWith(entity)) return TopologyCheckResult::Accepted;
            return TopologyCheckResult::Rejected;
        }
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

TopologyCheckResult PolygonEntity::checkTopologyCondition(PolygonEntity* entity) {
    qDebug() << __FUNCTION__ << "(PolygonEntity)";
    switch (TopologyManager::getManager()->getTopologyCondition()) {
    case TopologyCondition::Inside:
        if (contains(entity)) return TopologyCheckResult::Accepted;
        return TopologyCheckResult::Rejected;
    case TopologyCondition::Outside:
        if (!contains(entity)) return TopologyCheckResult::Rejected;
        return TopologyCheckResult::Accepted;
    case TopologyCondition::NoTopology:
        return TopologyCheckResult::Accepted;
    case TopologyCondition::Intersect: {
            if (TopologyManager::getManager()->getDrawingStage() != DrawingStage::Submitting)
                return TopologyCheckResult::Accepted;
            if (intersectsWith(entity)) return TopologyCheckResult::Accepted;
            return TopologyCheckResult::Rejected;
        }
    case TopologyCondition::NoIntersect: {
            if (TopologyManager::getManager()->getDrawingStage() != DrawingStage::Submitting)
                return TopologyCheckResult::Accepted;
            if (!intersectsWith(entity)) return TopologyCheckResult::Accepted;
            return TopologyCheckResult::Rejected;
        }
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

TopologyCheckResult PolygonEntity::checkTopologyCondition(PointEntity* entity) {
    qDebug() << __FUNCTION__ << "(Point)";
    switch (TopologyManager::getManager()->getTopologyCondition()) {
    case TopologyCondition::Inside:
        if (contains(entity)) return TopologyCheckResult::Accepted;
        return TopologyCheckResult::Rejected;
    case TopologyCondition::Outside:
        if (!contains(entity)) return TopologyCheckResult::Rejected;
        return TopologyCheckResult::Accepted;
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

TopologyCheckResult PolygonEntity::checkTopologyCondition(PolylineEntity* entity) {
    qDebug() << __FUNCTION__ << "(Polyline)";
    switch (TopologyManager::getManager()->getTopologyCondition()) {
    case TopologyCondition::Inside:
        if (contains(entity)) return TopologyCheckResult::Accepted;
        return TopologyCheckResult::Rejected;
    case TopologyCondition::Outside:
        if (!contains(entity)) return TopologyCheckResult::Rejected;
        return TopologyCheckResult::Accepted;
    case TopologyCondition::NoTopology:
        return TopologyCheckResult::Accepted;
    case TopologyCondition::Intersect: {
            if (TopologyManager::getManager()->getDrawingStage() != DrawingStage::Submitting)
                return TopologyCheckResult::Accepted;
            if (intersectsWith(entity)) return TopologyCheckResult::Accepted;
            return TopologyCheckResult::Rejected;
        }
    case TopologyCondition::NoIntersect: {
            if (TopologyManager::getManager()->getDrawingStage() != DrawingStage::Submitting)
                return TopologyCheckResult::Accepted;
            if (!intersectsWith(entity)) return TopologyCheckResult::Accepted;
            return TopologyCheckResult::Rejected;
        }
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

void PolygonEntity::moveBy(QPointF dp) {
    QPolygonF polygon = this->getItem()->polygon();
    for (int i = 0; i < polygon.size(); ++i) polygon[i] += dp;
    this->getItem()->setPolygon(polygon);
    if (selection_item != nullptr) selection_item->setPos(selection_item->pos() + dp);
}

QDomElement PolygonEntity::toDomElement(QDomDocument &doc) {
    QVector<QPointF> points = this->getPointsVector();
    QDomElement element = doc.createElement(this->getType());
    for (QPointF pt : points) {
        QDomElement child = doc.createElement("prim_point");
        child.setAttribute("X", pt.rx());
        child.setAttribute("Y", pt.ry());
        element.appendChild(child);
    }
    element.setAttribute("width", this->getWidth());
    QColor color = this->getColor();
    element.setAttribute("red", color.red());
    element.setAttribute("green", color.green());
    element.setAttribute("blue", color.blue());
    element.setAttribute("alpha", color.alpha());
    return element;
}

void PolygonEntity::selectedEvent() {
    this->selection_item = MainWindow::drawing_scene->addPolygon(this->getPolygon());
    QPen pen = this->getPen();
    pen.setWidthF(getWidth() * (1 + std::fminf(2.0f * getWidth(), 10.0f) / getWidth()));
    pen.setColor(QColor(0, 0, 255, 200));
    this->selection_item->setPen(pen);
    this->selection_item->setEnabled(false);
    this->selection_item->setZValue(this->getItem()->zValue() - 1);
}

void PolygonEntity::deselectedEvent() {
    delete this->selection_item;
    this->selection_item = nullptr;
}
