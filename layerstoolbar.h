#pragma once

#include <QVector>
#include <QVariant>
#include <QAbstractItemModel>
#include <QToolBar>
#include <QTreeView>
#include <QStandardItemModel>
#include <QStandardItem>

#include "entity.h"
#include "line.h"
#include "entitiesmanager.h"

class LayerItem;

class LayersToolBar : public QToolBar
{
    Q_OBJECT
private:
    QStandardItemModel* model = nullptr;
    QTreeView* tree_view = nullptr;
    QItemSelectionModel* selection_model = nullptr;
public:
    LayersToolBar(QWidget* parent = nullptr);
    void clearSelection() {
        this->selection_model->clear();
    }
    void deleteItem(QStandardItem* item);
public slots:
    void selectedItemsChanged(const QItemSelection &selected, const QItemSelection &deselected);
};

