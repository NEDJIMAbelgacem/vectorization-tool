#pragma once

#include <QPointF>
#include <QLineF>
#include <QPolygonF>
#include <QDebug>

bool doIntersect(QLineF line1, QLineF line2);

bool areNeighbours(QPointF p1, QPointF p2, qreal threshold);

bool areNeighbours(QPointF point, QLineF line, qreal threshold);
bool areNeighbours(QLineF line, QPointF point, qreal threshold);

bool areNeighbours(QPointF point, QPolygonF polygon, qreal threshold);
bool areNeighbours(QPolygonF polygon, QPointF point, qreal threshold);

bool areNeighbours(QLineF line1, QLineF line2, qreal threshold);

bool areNeighbours(QLineF line, QPolygonF polygon, qreal threshold);
bool areNeighbours(QPolygonF polygon, QLineF line, qreal threshold);

bool areNeighbours(QPolygonF polygon1, QPolygonF polygon2, qreal threshold);
