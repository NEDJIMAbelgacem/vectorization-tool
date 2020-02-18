#include "layeritem.h"
#include "mainwindow.h"

LayerItem::LayerItem(const QVector<QVariant> &data, LayerItem *parentItem)
    : m_itemData(data), m_parentItem(parentItem) {
    model_item = new QStandardItem(data.at(0).toString());
}

LayerItem::LayerItem(QDomElement& element, LayerItem *parentItem) {
    this->m_itemData.push_back(element.tagName());
    model_item = new QStandardItem(m_itemData.at(0).toString());
    if (element.tagName() == "Point") {
        PointEntity* point = new PointEntity(element);
        point->sendToManager();
        this->setStoredEntity(point);
    } else if (element.tagName() == "Line") {
        LineEntity* line = new LineEntity(element);
        line->sendToManager();
        this->setStoredEntity(line);
    } else if (element.tagName() == "Polygon") {
        PolygonEntity* polygon = new PolygonEntity(element);
        polygon->sendToManager();
        this->setStoredEntity(polygon);
    } else if (element.tagName() == "Polyline") {
        PolylineEntity* polyline = new PolylineEntity(element);
        polyline->sendToManager();
        this->setStoredEntity(polyline);
    } else if (element.tagName() == "Image") {
        ImageEntity* image = new ImageEntity(element);
        image->sendToManager();
        this->setStoredEntity(image);
    }else {
        QDomNodeList children = element.childNodes();
        for (int i = 0; i < children.size(); ++i) {
            QDomElement e = children.at(i).toElement();
            LayerItem* child = new LayerItem(e);
            this->appendChild(child);
        }
    }
}

LayerItem::~LayerItem() {
    qDebug() << __FUNCTION__;
    if (this->stored_entity != nullptr) delete stored_entity;
    if (this->model_item != nullptr) MainWindow::layers_tool_bar->deleteItem(this->model_item);
    qDeleteAll(m_childItems);
}

void LayerItem::appendChild(LayerItem *item) {
    m_childItems.append(item);
    model_item->appendRow(item->getModelItem());
}

LayerItem* LayerItem::child(int row) {
    if (row < 0 || row >= m_childItems.size())
            return nullptr;
        return m_childItems.at(row);
}

int LayerItem::childCount() const {
    return m_childItems.count();
}

int LayerItem::columnCount() const {
    return m_itemData.count();
}

QVariant LayerItem::data(int column) const {
//    qDebug() << m_itemData.at(column);
    if (column < 0 || column >= m_itemData.size())
        return QVariant();
    return m_itemData.at(column);
}
int LayerItem::row() const {
    if (m_parentItem)
        return m_parentItem->m_childItems.indexOf(const_cast<LayerItem*>(this));
    return 0;
}
LayerItem* LayerItem::parentItem() {
    return m_parentItem;
}

int LayerItem::childIndex(LayerItem* item) {
    return m_childItems.indexOf(item);
}

void LayerItem::setStoredEntity(Entity* entity) {
    this->stored_entity = entity;
    this->stored_item = entity->getItem();
}

void LayerItem::setStoredItem(QGraphicsItem* item) {
    this->stored_item = item;
}

Entity* LayerItem::getStoredEntity() {
    return stored_entity;
}

QGraphicsItem* LayerItem::getStoredItem() {
    return stored_item;
}

bool LayerItem::isChild(QGraphicsItem* item) {
    for (LayerItem* i : m_childItems) {
        if (i->getStoredItem() == item) return true;
    }
    return false;
}

LayerItem* LayerItem::removeChild(QGraphicsItem* item) {
    int item_index = -1;
    for (int i = 0; i < m_childItems.size(); ++i) {
        if (m_childItems[i]->getStoredItem() == item) {
            item_index = i;
            break;
        }
    }
    if (item_index == -1) return nullptr;
    LayerItem* child = m_childItems[item_index];
    m_childItems.remove(item_index);
    return child;
}

QStandardItem* LayerItem::getModelItem() {
    return this->model_item;
}

void LayerItem::fillDocElements(QDomDocument& doc, QDomNode& parent) {
    if (stored_entity != nullptr) {
        QDomElement child = stored_entity->toDomElement(doc);
        parent.appendChild(child);
    } else {
        QDomElement element = doc.createElement(m_itemData[0].toString() + "Layer");
        parent.appendChild(element);
        for (LayerItem* item : m_childItems) {
            item->fillDocElements(doc, element);
        }
    }
}
