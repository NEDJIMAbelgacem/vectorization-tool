#include "entity.h"
#include "mainwindow.h"


Entity::Entity(QString _type) : type(_type) { }

Entity::~Entity() {
    if (item != nullptr) delete item;
}

void Entity::sendToManager() {
    EntitiesManager* man = EntitiesManager::getManager();
    man->saveEntity(this, item);
}

QString Entity::getType() {
    return this->type;
}

QGraphicsItem* Entity::getItem() {
    return item;
}

void Entity::selectedEvent() {
    auto* eff = item->graphicsEffect();
    if (eff != nullptr) delete eff;
    item->setGraphicsEffect(MainWindow::drawing_area->getItemSelectionEffect());
}

void Entity::deselectedEvent() {
    QGraphicsDropShadowEffect* eff = new QGraphicsDropShadowEffect;
    eff->setEnabled(false);
    this->getItem()->setGraphicsEffect(eff);
}

QDomElement Entity::toDomElement(QDomDocument& doc) {
    return doc.createElement(this->getType());
}
