#include "layerstoolbar.h"

LayersToolBar::LayersToolBar(QWidget* parent) : QToolBar(parent)
{
    tree_view = new QTreeView(this);
    model = EntitiesManager::getManager()->getModel();
    selection_model = new QItemSelectionModel(model, tree_view);
    model->setHeaderData(0, Qt::Orientation::Horizontal, QString("Layers"));

    QObject::connect(selection_model, SIGNAL(selectionChanged(const QItemSelection &, const QItemSelection &)),
                     this, SLOT(selectedItemsChanged(const QItemSelection &, const QItemSelection &)));

    tree_view->setModel(model);
    tree_view->setSelectionModel(selection_model);
    this->addWidget(tree_view);
}

void LayersToolBar::selectedItemsChanged(const QItemSelection &selected, const QItemSelection &deselected) {
    qDebug() << __FUNCTION__;
    TopologyManager::getManager()->setSelectedEntity(nullptr);
    for (QModelIndex index : deselected.indexes()) {
        QStandardItem* std_item = model->itemFromIndex(index);
        LayerItem* layer_item = EntitiesManager::getManager()->getLayerItem(std_item);
        qDebug() << layer_item;
        Entity* entity = layer_item->getStoredEntity();
        qDebug() << entity;
        if (entity == nullptr) continue;
        entity->deselectedEvent();
    }
    bool item_selected = false;
    for (QModelIndex index : selected.indexes()) {
        QStandardItem* std_item = model->itemFromIndex(index);
        qDebug() << std_item;
        LayerItem* layer_item = EntitiesManager::getManager()->getLayerItem(std_item);
        qDebug() << layer_item;
        Entity* entity = layer_item->getStoredEntity();
        qDebug() << entity;
        if (entity == nullptr) continue;
        qDebug() << entity->getType();
        entity->selectedEvent();
        item_selected = true;
        TopologyManager::getManager()->setSelectedEntity(entity);
    }
    MainWindow::window->enableItemDelete(item_selected);
}

void LayersToolBar::deleteItem(QStandardItem* item) {
    model->removeRow(item->row(), item->parent()->index());
    qDebug() << "Row removed";
}
