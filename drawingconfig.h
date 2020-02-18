#pragma once
#include <QMap>
#include <QString>
#include <QVariant>
#include <QDebug>

#include "defaults.h"

class DrawingConfig {
private:
    QMap<QString, QVariant> data;
public:
    void setConfig(QString key, QVariant val) {
        data[key] = val;
    }
    template<class T>
    T getConfig(QString key) {
        if (!data.contains(key)) {
            return get_default<T>(key);
        }
        QVariant val = data[key];
        QString t1 = QString(val.type());
//        QString t2 = QString(val.nameToType(typeid(T).name()));
        // TODO : look for another way to do this
        QString t2 = QString(QVariant::nameToType(typeid(T).name()));
//        qDebug() << t1 << " " << t2;
        if (t1 ==  t2) {
            return val.value<T>();
        }
        return get_default<T>(key);
    }
};
