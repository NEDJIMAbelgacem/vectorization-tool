#include "polyline.h"

PolylineEntity::PolylineEntity() : Entity("Polyline") {
    qDebug() << __FUNCTION__;
    QGraphicsScene* scene = MainWindow::drawing_area->scene();
    this->item = scene->addPath(QPainterPath());
    this->item->setZValue(2 * EntitiesManager::getManager()->getEntitiesCount());
}

PolylineEntity::PolylineEntity(QDomElement& element) : Entity("Polyline") {
    Q_ASSERT(element.tagName() == this->getType());
    QGraphicsScene* scene = MainWindow::drawing_area->scene();
    this->item = scene->addPath(QPainterPath());
    QDomNodeList lst = element.childNodes();
    for (int i = 0; i < lst.size(); ++i) {
        QDomElement node = lst.at(i).toElement();
        if (node.tagName() != "prim_point") continue;
        float x = node.attribute("X").toFloat();
        float y = node.attribute("Y").toFloat();
        QPointF point(x, y);
        if (i == 0) this->setFirstPoint(point);
        else this->addPoint(point);
    }
    this->setWidth(element.attribute("width").toFloat());
    int r = element.attribute("red").toInt();
    int g = element.attribute("green").toInt();
    int b = element.attribute("blue").toInt();
    int a = element.attribute("alpha").toInt();
    this->setColor(QColor(r, g, b, a));
}

bool PolylineEntity::intersectsWith(LineEntity* line_entity) {
    qDebug() << __FUNCTION__ << "(LineEntity)";
    QLineF line1 = line_entity->getLine();
    for (QLineF line2 : this->getLinesVector()) {
        if (doIntersect(line1, line2)) return true;
    }
    return false;
}

bool PolylineEntity::intersectsWith(PolygonEntity* polygon) {
    return polygon->intersectsWith(this);
}

bool PolylineEntity::intersectsWith(PolylineEntity* polyline) {
    qDebug() << __FUNCTION__ << "(PolylineEntity)";
    QVector<QLineF> lines1 = this->getLinesVector();
    QVector<QLineF> lines2 = polyline->getLinesVector();
    for (QLineF line1 : lines1) {
        for (QLineF line2 : lines2) {
            if (doIntersect(line1, line2)) return true;
        }
    }
    return false;
}

bool PolylineEntity::isNeighboursWith(PointEntity* point, qreal threshold) {
    for (QLineF line : this->getLinesVector()) {
        if (areNeighbours(line, point->getPoint(), threshold)) return true;
    }
    return false;
}

bool PolylineEntity::isNeighboursWith(LineEntity* line, qreal threshold) {
    QLineF lline = line->getLine();
    for (QLineF pline : this->getLinesVector()) {
        if (areNeighbours(pline, lline, threshold)) return true;
    }
    return false;
}

bool PolylineEntity::isNeighboursWith(PolygonEntity* polygon, qreal threshold) {
    for (QPointF point : polygon->getPointsVector()) {
        bool is_neighbour = false;
        for (QLineF line : this->getLinesVector()) {
            if (areNeighbours(line, point, threshold)) {
                is_neighbour = true;
                break;
            }
        }
        if (!is_neighbour) return false;
    }
    return true;
}

bool PolylineEntity::isNeighboursWith(PolylineEntity* polyline, qreal threshold) {
    for (QPointF p : polyline->getPointsVector()) {
        bool is_neighbour = false;
        for (QLineF line : this->getLinesVector()) {
            if (areNeighbours(line, p, threshold)) {
                is_neighbour = true;
                break;
            }
        }
        if (!is_neighbour) return false;
    }
    return true;
}

QPen PolylineEntity::getPen() {
    QPen p;
    p.setColor(config.getConfig<QColor>("color"));
    p.setWidthF(config.getConfig<qreal>("width"));
    return p;
}

QPainterPath PolylineEntity::constructPainterPath() {
    QPainterPath path;
    path.moveTo(start_point);
    for (QPointF p : polyline_points) path.lineTo(p);
    return path;
}

void PolylineEntity::setFirstPoint(QPointF p) {
    this->start_point = p;
}

void PolylineEntity::addPoint(QPointF point) {
    auto* item = this->getItem();
    polyline_points.push_back(point);
    item->setPath(constructPainterPath());
}

void PolylineEntity::setLastPoint(QPointF point) {
    auto* item = this->getItem();
    polyline_points[polyline_points.size() - 1] = point;
    item->setPath(constructPainterPath());
}

QPointF PolylineEntity::getLastPoint() {
    return polyline_points[polyline_points.size() - 1];
}

void PolylineEntity::eraseLastPoint() {
    auto* item = this->getItem();
    polyline_points.pop_back();
    item->setPath(constructPainterPath());
}

void PolylineEntity::applyConfig() {
    auto* item = getItem();
    item->setPen(getPen());
    if (selection_item != nullptr) {
        QPen pen = selection_item->pen();
        pen.setWidthF(getWidth() * (1 + std::fminf(2.0f * getWidth(), 10.0f) / getWidth()));
        selection_item->setPen(pen);
    }
}

QGraphicsPathItem* PolylineEntity::getItem() {
    return qgraphicsitem_cast<QGraphicsPathItem*>(this->item);
}

QVector<QPointF> PolylineEntity::getPointsVector() {
    QVector<QPointF> points = this->polyline_points;
    points.push_front(start_point);
    return points;
}

QVector<QLineF> PolylineEntity::getLinesVector() {
    QVector<QPointF> points = this->getPointsVector();
    QVector<QLineF> lines;
    for (int i = 0; i < points.size() - 1; ++i) lines.push_back(QLineF(points[i], points[i + 1]));
    return lines;
}

void PolylineEntity::setConfig(DrawingConfig conf) {
    this->config = conf;
    applyConfig();
}

DrawingConfig PolylineEntity::getConfig() {
    return config;
}

TopologyCheckResult PolylineEntity::checkTopologyCondition(LineEntity* entity) {
    qDebug() << __FUNCTION__ << "(Line)";
    switch (TopologyManager::getManager()->getTopologyCondition()) {
    case TopologyCondition::Inside:
        return TopologyCheckResult::Ignore;
    case TopologyCondition::Outside:
        return TopologyCheckResult::Ignore;
    case TopologyCondition::NoTopology:
        return TopologyCheckResult::Accepted;
    case TopologyCondition::Intersect:{
            if (TopologyManager::getManager()->getDrawingStage() != DrawingStage::Submitting) {
                return TopologyCheckResult::Accepted;
            }
            if (this->intersectsWith(entity)) return TopologyCheckResult::Accepted;
            return TopologyCheckResult::Rejected;
        }
    case TopologyCondition::NoIntersect:{
            if (TopologyManager::getManager()->getDrawingStage() != DrawingStage::Submitting) {
                return TopologyCheckResult::Accepted;
            }
            if (!this->intersectsWith(entity)) return TopologyCheckResult::Accepted;
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

TopologyCheckResult PolylineEntity::checkTopologyCondition(PolygonEntity* entity) {
    qDebug() << __FUNCTION__ << "(Polygon)";
    switch (TopologyManager::getManager()->getTopologyCondition()) {
    case TopologyCondition::Inside:
        return TopologyCheckResult::Ignore;
    case TopologyCondition::Outside:
        return TopologyCheckResult::Ignore;
    case TopologyCondition::NoTopology:
        return TopologyCheckResult::Accepted;
    case TopologyCondition::Intersect:{
            if (TopologyManager::getManager()->getDrawingStage() != DrawingStage::Submitting) {
                return TopologyCheckResult::Accepted;
            }
            if (this->intersectsWith(entity)) return TopologyCheckResult::Accepted;
            return TopologyCheckResult::Rejected;
        }
    case TopologyCondition::NoIntersect:{
            if (TopologyManager::getManager()->getDrawingStage() != DrawingStage::Submitting) {
                return TopologyCheckResult::Accepted;
            }
            if (!this->intersectsWith(entity)) return TopologyCheckResult::Accepted;
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

TopologyCheckResult PolylineEntity::checkTopologyCondition(PointEntity* entity) {
    qDebug() << __FUNCTION__ << "(Point)";
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

TopologyCheckResult PolylineEntity::checkTopologyCondition(PolylineEntity* entity) {
    qDebug() << __FUNCTION__ << "(Polyline)";
    switch (TopologyManager::getManager()->getTopologyCondition()) {
    case TopologyCondition::Inside:
        return TopologyCheckResult::Ignore;
    case TopologyCondition::Outside:
        return TopologyCheckResult::Ignore;
    case TopologyCondition::NoTopology:
        return TopologyCheckResult::Accepted;
    case TopologyCondition::Intersect:{
            if (TopologyManager::getManager()->getDrawingStage() != DrawingStage::Submitting) {
                return TopologyCheckResult::Accepted;
            }
            if (this->intersectsWith(entity)) return TopologyCheckResult::Accepted;
            return TopologyCheckResult::Rejected;
        }
    case TopologyCondition::NoIntersect:{
            if (TopologyManager::getManager()->getDrawingStage() != DrawingStage::Submitting) {
                return TopologyCheckResult::Accepted;
            }
            if (!this->intersectsWith(entity)) return TopologyCheckResult::Accepted;
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

void PolylineEntity::moveBy(QPointF dp) {
    this->start_point += dp;
    for (int i = 0; i < this->polyline_points.size(); ++i) polyline_points[i] += dp;
    this->getItem()->setPath(constructPainterPath());
    if (selection_item != nullptr) selection_item->setPos(selection_item->pos() + dp);
}

QDomElement PolylineEntity::toDomElement(QDomDocument &doc) {
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

void PolylineEntity::selectedEvent() {
    this->selection_item = MainWindow::drawing_scene->addPath(this->getItem()->path());
    QPen pen = this->getPen();
    pen.setWidthF(pen.widthF() * (1 + std::fminf(2.0f * pen.widthF(), 10.0f) / pen.widthF()));
    pen.setColor(QColor(0, 0, 255, 200));
    this->selection_item->setPen(pen);
    this->selection_item->setEnabled(false);
    this->selection_item->setZValue(this->getItem()->zValue() - 1);
}

void PolylineEntity::deselectedEvent() {
    delete this->selection_item;
    this->selection_item = nullptr;
}
