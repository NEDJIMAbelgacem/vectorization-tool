#pragma once
#include <QtGlobal>
#include <QString>
#include <QColor>

template<typename T>
T get_default(QString name);

template<>
qreal get_default(QString name);

template<>
QColor get_default(QString name);
