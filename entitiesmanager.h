#pragma once
#include <QMap>
#include <QDebug>
#include <QGraphicsItem>
#include <QStandardItemModel>
#include <QStandardItem>

#include "entity.h"
#include "layeritem.h"

class Entity;
class LayerItem;

class EntitiesManager {
private:
    EntitiesManager();
    static EntitiesManager* manager;
    void clear();
public:
    static EntitiesManager* getManager();
    static int getEntitiesCount();
private:
    QMap<QGraphicsItem*, Entity*> drawn_entities;
    QMap<QString, LayerItem*> shapes;
    LayerItem* tree_root = nullptr;
    QStandardItemModel* model = nullptr;
    QMap<QStandardItem*, LayerItem*> modelitem_mapping;
public:
    void saveEntity(Entity* entity, QGraphicsItem* item);
    void eraseEntity(Entity* entity, QGraphicsItem* item);

    void fillDocument(QDomDocument& doc);
    void loadDocument(QDomDocument& doc);

    QStandardItemModel* getModel();

    LayerItem* getLayerItem(QStandardItem* stditem);
    QStandardItem* getLayerItemStdItem(LayerItem* layer_item);
    Entity* getEntity(QGraphicsItem* item);
};
