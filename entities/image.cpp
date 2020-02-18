#include "image.h"

ImageEntity::ImageEntity(QString path) : Entity("Image") {
    this->path = path;
    QPixmap pixmap(path);
    this->item = MainWindow::drawing_scene->addPixmap(pixmap);
    this->item->setZValue(2 * EntitiesManager::getManager()->getEntitiesCount());
}

ImageEntity::ImageEntity(QDomElement& element) : Entity("Image") {
    this->path = element.attribute("path");
    QPixmap pixmap(path);
    this->item = MainWindow::drawing_scene->addPixmap(pixmap);
}


QGraphicsPixmapItem* ImageEntity::getItem() {
    return qgraphicsitem_cast<QGraphicsPixmapItem*>(this->item);
}

void ImageEntity::moveBy(QPointF dp) {
    this->getItem()->setPos(this->getItem()->pos() + dp);
    if (selection_item != nullptr) selection_item->setPos(selection_item->pos() + dp);
}

QDomElement ImageEntity::toDomElement(QDomDocument &doc) {
    QDomElement element = doc.createElement(this->getType());
    element.setAttribute("path", this->path);
    return element;
}

void ImageEntity::selectedEvent() {
    this->selection_item = MainWindow::drawing_scene->addRect(this->getItem()->pixmap().rect());
    this->selection_item->setPos(this->getItem()->pos());
    QPen pen;
    pen.setWidthF(pen.widthF() * (1 + std::fminf(2.0f * pen.widthF(), 10.0f) / pen.widthF()));
    pen.setColor(QColor(0, 0, 255, 200));
    this->selection_item->setPen(pen);
    this->selection_item->setEnabled(false);
    this->selection_item->setZValue(this->getItem()->zValue() - 1);
}

void ImageEntity::deselectedEvent() {
    delete this->selection_item;
    this->selection_item = nullptr;
}
