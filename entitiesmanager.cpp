#include "entitiesmanager.h"

EntitiesManager* EntitiesManager::manager = nullptr;

EntitiesManager::EntitiesManager()
{
    model = new QStandardItemModel;
    tree_root = new LayerItem({QString("ROOT")});
    modelitem_mapping[tree_root->getModelItem()] = tree_root;
    model->appendRow(tree_root->getModelItem());
}

void EntitiesManager::clear() {
    model->clear();
    for (auto* key : drawn_entities.keys()) {
        Entity* e = drawn_entities[key];
        delete e;
    }
    drawn_entities.clear();
    modelitem_mapping.clear();
}

EntitiesManager* EntitiesManager::getManager() {
    if (EntitiesManager::manager == nullptr) EntitiesManager::manager = new EntitiesManager;
    return EntitiesManager::manager;
}

int EntitiesManager::getEntitiesCount() {
    EntitiesManager* m = getManager();
    return m->drawn_entities.size();
}

void EntitiesManager::saveEntity(Entity* entity, QGraphicsItem* item) {
    if (drawn_entities.contains(item)) qDebug() << "WARNING : item already exists";
    drawn_entities[item] = entity;
    QString entity_type = entity->getType();
    if (!shapes.contains(entity_type)) {
        shapes[entity_type] = new LayerItem({entity_type}, tree_root);
        modelitem_mapping[shapes[entity_type]->getModelItem()] = shapes[entity_type];
        tree_root->appendChild(shapes[entity_type]);
    }
    if (!shapes[entity_type]->isChild(item)) {
        LayerItem* layer_item = new LayerItem({QString("%1").arg(shapes[entity_type]->childCount())}, shapes[entity_type]);
        shapes[entity_type]->appendChild(layer_item);
        modelitem_mapping[layer_item->getModelItem()] = layer_item;
        layer_item->setStoredEntity(entity);
    }
}

void EntitiesManager::eraseEntity(Entity* entity, QGraphicsItem* item) {
    if (entity == nullptr || item == nullptr || !drawn_entities.contains(item)) return;
    drawn_entities.remove(item);
    QString entity_type = entity->getType();
    LayerItem* layer_item = shapes[entity_type]->removeChild(item);
    if (layer_item != nullptr) {
        modelitem_mapping.remove(layer_item->getModelItem());
    }
    delete layer_item;
}

QStandardItemModel* EntitiesManager::getModel() {
    return this->model;
}

LayerItem* EntitiesManager::getLayerItem(QStandardItem* stditem) {
    if (!modelitem_mapping.contains(stditem)) return nullptr;
    return modelitem_mapping[stditem];
}

QStandardItem* EntitiesManager::getLayerItemStdItem(LayerItem* layer_item) {
    for (QStandardItem* std_item : modelitem_mapping.keys()) {
        if (modelitem_mapping[std_item] == layer_item) return std_item;
    }
    return nullptr;
}

Entity* EntitiesManager::getEntity(QGraphicsItem* item) {
    if (!drawn_entities.contains(item)) return nullptr;
    return drawn_entities[item];
}

void EntitiesManager::fillDocument(QDomDocument& doc) {
    this->tree_root->fillDocElements(doc, doc);
}

void EntitiesManager::loadDocument(QDomDocument& doc) {
    this->clear();
    QDomElement e = doc.toElement();
    tree_root = new LayerItem(e);
    modelitem_mapping[tree_root->getModelItem()] = tree_root;
    model->appendRow(tree_root->getModelItem());
}
