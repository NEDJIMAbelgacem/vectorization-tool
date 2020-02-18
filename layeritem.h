#pragma once

#include <QObject>
#include <QStandardItem>
#include <QGraphicsItem>
#include <QDebug>
#include <QtXml/QDomElement>

#include "entity.h"

class Entity;

class LayerItem : public QObject {
    Q_OBJECT
private:
    Entity* stored_entity = nullptr;
    QGraphicsItem* stored_item = nullptr;
    QStandardItem* model_item = nullptr;
    QVector<LayerItem*> m_childItems;
    QVector<QVariant> m_itemData;
    LayerItem *m_parentItem;
public:
    explicit LayerItem(const QVector<QVariant> &data, LayerItem *parentItem = nullptr);
    explicit LayerItem(QDomElement& element, LayerItem *parentItem = nullptr);

    ~LayerItem();

    void appendChild(LayerItem *item);
    LayerItem *child(int row);
    int childCount() const;
    int columnCount() const;
    QVariant data(int column) const;
    int row() const;
    LayerItem *parentItem();
    int childIndex(LayerItem* item);

    void setStoredEntity(Entity* entity);
    void setStoredItem(QGraphicsItem* item);

    Entity* getStoredEntity();
    QGraphicsItem* getStoredItem();

    bool isChild(QGraphicsItem* item);
    LayerItem* removeChild(QGraphicsItem* item);

    QStandardItem* getModelItem();
public:
    void fillDocElements(QDomDocument& doc, QDomNode& parent);
signals:
    void changed();
};
