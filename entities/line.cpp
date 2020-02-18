#include "line.h"

LineEntity::LineEntity() : Entity("Line") {
    qDebug() << __FUNCTION__;
    QGraphicsScene* scene = MainWindow::drawing_area->scene();
    this->item = scene->addLine(QLine());
    this->item->setZValue(2 * EntitiesManager::getManager()->getEntitiesCount());
}

LineEntity::LineEntity(QDomElement& element) : Entity("Line") {
    Q_ASSERT(element.tagName() == this->getType());
    float start_x = element.attribute("start_x").toFloat();
    float start_y = element.attribute("start_y").toFloat();
    float end_x = element.attribute("end_x").toFloat();
    float end_y = element.attribute("end_y").toFloat();
    QLineF line(QPointF(start_x, start_y), QPointF(end_x, end_y));
    this->item = MainWindow::drawing_scene->addLine(line);
    this->setWidth(element.attribute("width").toFloat());
    int r = element.attribute("red").toInt();
    int g = element.attribute("green").toInt();
    int b = element.attribute("blue").toInt();
    int a = element.attribute("alpha").toInt();
    this->setColor(QColor(r, g, b, a));
}

QPen LineEntity::getPen() {
    QPen p;
    p.setColor(config.getConfig<QColor>("color"));
    p.setWidthF(config.getConfig<qreal>("width"));
    qDebug() << p;
    return p;
}

void LineEntity::applyConfig() {
    qDebug() << __FUNCTION__;
    QGraphicsLineItem* line_item = getItem();
    line_item->setPen(getPen());
    if (selection_item != nullptr) {
        QPen pen = selection_item->pen();
        pen.setWidthF(getWidth() * (1 + std::fminf(2.0f * getWidth(), 10.0f) / getWidth()));
        selection_item->setPen(pen);
    }
}

void LineEntity::setStart(QPointF start) {
    auto* item =  getItem();
    QLineF l = item->line();
    l.setP1(start);
    item->setLine(l);
}

void LineEntity::setEnd(QPointF end) {
    auto* item = getItem();
    QLineF l = item->line();
    l.setP2(end);
    item->setLine(l);
}

QPointF LineEntity::getStart() {
    return this->getItem()->line().p1();
}

QPointF LineEntity::getEnd() {
    return this->getItem()->line().p2();
}

void LineEntity::setColor(QColor color) {
    this->config.setConfig("color", color);
    applyConfig();
}

void LineEntity::setWidth(qreal width) {
    this->config.setConfig("width", width);
    applyConfig();
}

void LineEntity::setConfig(DrawingConfig conf) {
    this->config = conf;
    applyConfig();
}

QGraphicsLineItem* LineEntity::getItem() {
    return qgraphicsitem_cast<QGraphicsLineItem*>(this->item);
}

QLineF LineEntity::getLine() {
    QLineF line = this->getItem()->line();
    line.setP1(this->getItem()->mapToScene(line.p1()));
    line.setP2(this->getItem()->mapToScene(line.p2()));
    return line;
}

bool LineEntity::intersectsWith(LineEntity* line) {
    return doIntersect(this->getLine(), line->getLine());
}

bool LineEntity::intersectsWith(PolygonEntity* polygon) {
    return polygon->intersectsWith(this);
}

bool LineEntity::intersectsWith(PolylineEntity* polyline) {
    return polyline->intersectsWith(this);
}

bool LineEntity::isNeighboursWith(PointEntity* point, qreal threshold) {
    return areNeighbours(point->getPoint(), this->getLine(), threshold);
}

bool LineEntity::isNeighboursWith(LineEntity* line, qreal threshold) {
    QLineF l = line->getLine();
    return areNeighbours(this->getLine(), l.p1(), threshold) && areNeighbours(this->getLine(), l.p2(), threshold);
}

bool LineEntity::isNeighboursWith(PolygonEntity* polygon, qreal threshold) {
    QVector<QPointF> polygon_points = polygon->getPointsVector();
    for (int i = 0; i < polygon_points.size(); ++i) {
        QPointF point = polygon_points[i];
        if (!areNeighbours(this->getLine(), point, threshold)) return false;
    }
    return true;
}

bool LineEntity::isNeighboursWith(PolylineEntity* polyline, qreal threshold) {
    QVector<QPointF> polyline_points = polyline->getPointsVector();
    for (int i = 0; i < polyline_points.size(); ++i) {
        QPointF point = polyline_points[i];
        if (!areNeighbours(this->getLine(), point, threshold)) return false;
    }
    return true;
}

TopologyCheckResult LineEntity::checkTopologyCondition(LineEntity* entity) {
    qDebug() << __FUNCTION__ << "(Line)";
    switch (TopologyManager::getManager()->getTopologyCondition()) {
    case TopologyCondition::Inside:
        return TopologyCheckResult::Ignore;
    case TopologyCondition::Outside:
        return TopologyCheckResult::Ignore;
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

TopologyCheckResult LineEntity::checkTopologyCondition(PolygonEntity* entity) {
    qDebug() << __FUNCTION__ << "(Polygon)";
    switch (TopologyManager::getManager()->getTopologyCondition()) {
    case TopologyCondition::Inside:
        return TopologyCheckResult::Ignore;
    case TopologyCondition::Outside:
        return TopologyCheckResult::Ignore;
    case TopologyCondition::NoTopology:
        return TopologyCheckResult::Accepted;
    case TopologyCondition::Intersect: {
            if (TopologyManager::getManager()->getDrawingStage() != DrawingStage::Submitting) {
                return TopologyCheckResult::Accepted;
            }
            if (this->intersectsWith(entity)) return TopologyCheckResult::Accepted;
            return TopologyCheckResult::Rejected;
        }
    case TopologyCondition::NoIntersect: {
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

TopologyCheckResult LineEntity::checkTopologyCondition(PointEntity* entity) {
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

TopologyCheckResult LineEntity::checkTopologyCondition(PolylineEntity* entity) {
    qDebug() << __FUNCTION__ << "(Polyline)";
    switch (TopologyManager::getManager()->getTopologyCondition()) {
    case TopologyCondition::Inside:
        return TopologyCheckResult::Ignore;
    case TopologyCondition::Outside:
        return TopologyCheckResult::Ignore;
    case TopologyCondition::NoTopology:
        return TopologyCheckResult::Accepted;
    case TopologyCondition::Intersect: {
            if (TopologyManager::getManager()->getDrawingStage() != DrawingStage::Submitting) {
                return TopologyCheckResult::Accepted;
            }
            if (this->intersectsWith(entity)) return TopologyCheckResult::Accepted;
            return TopologyCheckResult::Rejected;
        }
    case TopologyCondition::NoIntersect: {
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

void LineEntity::moveBy(QPointF dp) {
    this->setStart(this->getStart() + dp);
    this->setEnd(this->getEnd() + dp);
    if (this->selection_item != nullptr) this->selection_item->setLine(this->getLine());
}

QDomElement LineEntity::toDomElement(QDomDocument &doc) {
    QDomElement element = doc.createElement(this->getType());
    QPointF start = this->getStart(), end = this->getEnd();
    element.setAttribute("start_x", start.rx());
    element.setAttribute("start_y", start.ry());
    element.setAttribute("end_x", end.rx());
    element.setAttribute("end_y", end.ry());
    QColor color = this->getColor();
    element.setAttribute("red", color.red());
    element.setAttribute("green", color.green());
    element.setAttribute("blue", color.blue());
    element.setAttribute("alpha", color.alpha());
    element.setAttribute("width", getWidth());
    return element;
}

void LineEntity::selectedEvent() {
    this->selection_item = MainWindow::drawing_scene->addLine(this->getLine());
    QPen pen = this->getPen();
    pen.setWidthF(getWidth() * (1 + std::fminf(2.0f * getWidth(), 10.0f) / getWidth()));
    pen.setColor(QColor(0, 0, 255, 200));
    this->selection_item->setPen(pen);
    this->selection_item->setEnabled(false);
    this->selection_item->setZValue(this->getItem()->zValue() - 1);
}

void LineEntity::deselectedEvent() {
    delete this->selection_item;
    this->selection_item = nullptr;
}
