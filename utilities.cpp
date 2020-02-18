#include "utilities.h"

bool doIntersect(QLineF line1, QLineF line2) {
    QPointF pt;
    return line1.intersects(line2, &pt) == QLineF::IntersectionType::BoundedIntersection;
}

bool areNeighbours(QPointF p1, QPointF p2, qreal threshold) {
    QLineF line(p1, p2);
    return line.length() <= threshold;
}

bool areNeighbours(QLineF line, QPointF point, qreal threshold) {
    qDebug() << __FUNCTION__;
    QPointF dir = line.p2() - line.p1();
    QPointF p1 = line.p1();
    qreal a = point.rx() - p1.rx();
    qreal b = point.ry() - p1.ry();
    if (line.dx() == 0 && line.dy() == 0) return false;
    qreal sqr_len_l = line.dx() * line.dx() + line.dy() * line.dy();
    qDebug() << sqr_len_l;
    qreal optimal_t = (a * line.dx() + b * line.dy()) / sqr_len_l;
    optimal_t = optimal_t < 0 ? 0 : optimal_t > 1 ? 1 : optimal_t;
    QPointF closest_point = p1 + optimal_t * dir;
    qreal sqr_distance = powf(closest_point.rx() - point.rx(), 2) + powf(closest_point.ry() - point.ry(), 2);
    qDebug() << line << point;
    qDebug() << closest_point << optimal_t;
    qDebug() << "Distance : " << sqrtf(sqr_distance);
    return sqr_distance < threshold * threshold;
//    qreal a_x = - point.rx() + p1.rx();
//    qreal a_y = - point.ry() + p1.ry();
////    qDebug() << point;
//    if (dir.rx() == 0 && dir.ry() == 0) return false;
//    qreal optimal_t = -(a_x * dir.rx() + a_y * dir.ry()) / (dir.rx() * dir.rx() + dir.ry() * dir.ry());
//    if (optimal_t > 1.0) closest_point = line.p2();
//    else if (optimal_t < 0.0) closest_point = line.p1();
//    else {
//        closest_point.setX(p1.rx() + optimal_t * dir.rx());
//        closest_point.setY(p1.ry() + optimal_t * dir.ry());
//    }

////    qDebug() << line << point << closest_point;
////    qDebug() << optimal_t;
//    qreal distance = (closest_point.rx() - point.rx()) * (closest_point.rx() - point.rx()) + (closest_point.ry() - point.ry()) * (closest_point.ry() - point.ry());
////    qDebug() << "distance " << distance;
//    return distance <= threshold * threshold;
}

bool areNeighbours(QPointF point, QLineF line, qreal threshold) {
    return areNeighbours(line, point, threshold);
}

bool areNeighbours(QLineF line1, QLineF line2, qreal threshold) {
    if (line1.dx() == 0 && line1.dy() == 0) {
        if (line2.dx() == 0 && line2.dy() == 0) {
            qDebug() << "Warning : 2 unvalid lines";
            return false;
        }
        return areNeighbours(line2, line1, threshold);
    }
    return areNeighbours(line1, line2.p1(), threshold) && areNeighbours(line1, line2.p2(), threshold);
}

bool areNeighbours(QLineF line, QPolygonF polygon, qreal threshold) {
    for (int i = 0; i < polygon.size(); ++i) {
        QLineF line2(polygon.at(i), polygon.at((i + 1) % polygon.size()));
        if (!areNeighbours(line, line2, threshold)) return false;
    }
    return true;
}

bool areNeighbours(QPolygonF polygon, QLineF line, qreal threshold) {
    return areNeighbours(line, polygon, threshold);
}

bool areNeighbours(QPointF point, QPolygonF polygon, qreal threshold) {
    for (int i = 0; i < polygon.size(); ++i) {
        QLineF line(polygon.at(i), polygon.at((i + 1) % polygon.size()));
        if (areNeighbours(line, point, threshold)) return true;
    }
    return false;
}

bool areNeighbours(QPolygonF polygon, QPointF point, qreal threshold) {
    return areNeighbours(point, polygon, threshold);
}

bool areNeighbours(QPolygonF polygon1, QPolygonF polygon2, qreal threshold) {
    for (int i = 0; i < polygon1.size(); ++i) {
        QLineF line(polygon1[i], polygon1[(i + 1) % polygon1.size()]);
        if (areNeighbours(polygon2, line, threshold)) return true;
    }
    return false;
}
