#include "defaults.h"


template<typename T>
T get_default(QString name) {
    Q_ASSERT(false);
    return T();
}

template<>
qreal get_default(QString name) {
    if (name == QString("width")) {
        return 1.0;
    }
    Q_ASSERT(false);
    return 0.0;
}

template<>
QColor get_default(QString name) {
    if (name == QString("color")) {
        return QColor(Qt::GlobalColor::black);
    }
    Q_ASSERT(false);
    return QColor(Qt::GlobalColor::black);;
}
