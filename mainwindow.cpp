#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow* MainWindow::window = nullptr;
DrawingArea* MainWindow::drawing_area = nullptr;
CanvasScene* MainWindow::drawing_scene = nullptr;
DrawingConfig* MainWindow::drawing_config = nullptr;
QLabel* MainWindow::mouse_position_label = nullptr;
LayersToolBar* MainWindow::layers_tool_bar = nullptr;
FramelessWindow* MainWindow::frameless = nullptr;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    window = this;

    MainWindow::drawing_area = this->ui->drawing_area;
    MainWindow::drawing_scene = new CanvasScene(MainWindow::drawing_area);
    MainWindow::drawing_scene->setSceneRect(-drawing_area->width(), drawing_area->height(), drawing_area->width(), drawing_area->height());
    MainWindow::drawing_area->setScene(MainWindow::drawing_scene);
    MainWindow::drawing_area->setMouseTracking(true);

    MainWindow::drawing_config = new DrawingConfig;

    drawing_tools_map["Line"] = new LineDrawingTool;
    drawing_tools_map["Polygon"] = new PolygonDrawingTool;
    drawing_tools_map["Polyline"] = new PolylineDrawingTool;
    drawing_tools_map["Point"] = new PointsDrawingTool;
    drawing_tools_map["Move"] = new ItemsMovingTool;
    drawing_tools_map["Hand"] = new HandMovingTool;
    selected_tool = "Hand";

    drawing_scene->setDrawingTool(this->getDrawingTool());

    QToolBar* tool_bar = new QToolBar("create", this);
    this->addToolBar(tool_bar);

    item_creation_cb = new QComboBox(tool_bar);
    item_creation_cb->addItem(QString("Hand"));
    item_creation_cb->addItem(QString("Move"));
    item_creation_cb->addItem(QString("Line"));
    item_creation_cb->addItem(QString("Polygon"));
    item_creation_cb->addItem(QString("Polyline"));
    item_creation_cb->addItem(QString("Point"));
    drawing_tools_vect.push_back("Hand");
    drawing_tools_vect.push_back("Move");
    drawing_tools_vect.push_back("Line");
    drawing_tools_vect.push_back("Polygon");
    drawing_tools_vect.push_back("Polyline");
    drawing_tools_vect.push_back("Point");
    tool_bar->addWidget(new QLabel("Tool : ", tool_bar));
    tool_bar->addWidget(item_creation_cb);
    tool_bar->addSeparator();
    QObject::connect(item_creation_cb, QOverload<const QString&>::of(&QComboBox::currentIndexChanged), [=](const QString& selected) {
        this->selected_tool = selected;
        drawing_scene->setDrawingTool(this->getDrawingTool());
    });

    QToolBar* properties_tool_bar = new QToolBar("create", this);
//    properties_tool_bar->addWidget(new QLabel("Properties : ", properties_tool_bar));
    this->addToolBar(properties_tool_bar);

    QDoubleSpinBox* size_spinbox = new QDoubleSpinBox(properties_tool_bar);
    size_spinbox->setValue(1.0);
    drawing_config->setConfig("width", 1.0f);
    QObject::connect(size_spinbox, QOverload<double>::of(&QDoubleSpinBox::valueChanged), [=](double new_val) {
        drawing_config->setConfig("width", new_val);
        Entity* entity = TopologyManager::getManager()->getSelectedEntity();
        if (entity != nullptr) entity->setWidth(new_val);
    });
    QLabel* label =new QLabel("width");
    properties_tool_bar->addWidget(label);
    properties_tool_bar->addWidget(size_spinbox);
    QAction* color_selection = properties_tool_bar->addAction("Change color");
    QObject::connect(color_selection, &QAction::triggered, [=]() {
        static QColorDialog* dialog = nullptr;
        if (dialog == nullptr) dialog = new QColorDialog(this);
        QObject::connect(dialog, QOverload<const QColor&>::of(&QColorDialog::colorSelected), [=](QColor color) {
            drawing_config->setConfig("color", color);
            Entity* entity = TopologyManager::getManager()->getSelectedEntity();
            if (entity != nullptr) entity->setColor(color);
        });
        dialog->open();
    });

    QObject::connect(this->ui->actionLoad, SIGNAL(triggered()), this, SLOT(loadXML()));

    LayersToolBar* layers_tool_bar = new LayersToolBar;
    MainWindow::layers_tool_bar = layers_tool_bar;
    this->addToolBar(Qt::ToolBarArea::LeftToolBarArea, layers_tool_bar);

    QToolBar* topology_toolbar = new QToolBar(this);
    topology_toolbar->addWidget(new QLabel("Topology : "));
    this->addToolBar(topology_toolbar);
    QComboBox* topology_cb = new QComboBox;
    topology_toolbar->addWidget(topology_cb);
    QMap<QString, TopologyCondition> topologies;
    topologies[QString("NoTopology")] = TopologyCondition::NoTopology;
    topologies[QString("Inside")] = TopologyCondition::Inside;
    topologies[QString("Outside")] = TopologyCondition::Outside;
    topologies[QString("Intersects")] = TopologyCondition::Intersect;
    topologies[QString("No intersects")] = TopologyCondition::NoIntersect;
    topologies[QString("Neighbours")] = TopologyCondition::Neighbours;
    topologies[QString("Not neighbours")] = TopologyCondition::NotNeighbours;
    topology_cb->addItem(QString("NoTopology"));
    topology_cb->addItem(QString("Inside"));
    topology_cb->addItem(QString("Outside"));
    topology_cb->addItem(QString("Intersects"));
    topology_cb->addItem(QString("No intersects"));
    topology_cb->addItem(QString("Neighbours"));
    topology_cb->addItem(QString("Not neighbours"));

    QObject::connect(topology_cb, QOverload<const QString&>::of(&QComboBox::currentIndexChanged), [=](QString selected) {
        TopologyManager::getManager()->setTopologyCondition(topologies[selected]);
    });

    QDoubleSpinBox* neighbourhoud_threshold = new QDoubleSpinBox(topology_toolbar);
    neighbourhoud_threshold->setValue(5.0f);
    topology_toolbar->addSeparator();
    topology_toolbar->addWidget(new QLabel("Neighbourhood distance : "));
    topology_toolbar->addWidget(neighbourhoud_threshold);
    QObject::connect(neighbourhoud_threshold, QOverload<double>::of(&QDoubleSpinBox::valueChanged), [=](double new_val) {
        TopologyManager::getManager()->setNeighbourhoodThreshold(new_val);
    });

    mouse_position_label = new QLabel(ui->statusbar);
    ui->statusbar->addWidget(mouse_position_label);

    MainWindow::drawing_area->centerOn(0.0f, 0.0f);

    delete_item_action = layers_tool_bar->addAction("Delete item");
    QObject::connect(delete_item_action, SIGNAL(triggered()), this, SLOT(deleteSelectedItem()));
    delete_item_action->setEnabled(false);

    QObject::connect(ui->actionSave, SIGNAL(triggered()), this, SLOT(saveXML()));

    QAction* load_map_action = tool_bar->addAction("Load map");
    QObject::connect(load_map_action, SIGNAL(triggered()), this, SLOT(loadMap()));

    QObject::connect(ui->actionExit, SIGNAL(triggered()), this, SLOT(exit()));

    QObject::connect(ui->actionAbout, &QAction::triggered, [=]() {
        QMessageBox msgBox;
        msgBox.setText("This project was created by Nedjima Belgacem during SIGA class (geographical information systems course) at ESI-Algiers. \n\n Github project URL :\n \"https://github.com/NEDJIMAbelgacem/vectorization-tool\"\n\nI hope it's useful :)");
        msgBox.exec();
    });
}

MainWindow::~MainWindow() {
    delete ui;
}

void MainWindow::loadMap() {
    QFileDialog dialog;
    dialog.setFileMode(QFileDialog::FileMode::ExistingFile);
    // TODO : set filters
    if (dialog.exec()) {
        QList<QUrl> lst = dialog.selectedUrls();
        QUrl url = lst[0];
        ImageEntity* img = new ImageEntity(url.toLocalFile());
        img->sendToManager();
    }
}

void MainWindow::saveXML() {
    QFileDialog dialog;
    dialog.setFileMode(QFileDialog::FileMode::AnyFile);
    if (dialog.exec()) {
        QList<QUrl> lst = dialog.selectedUrls();
        QUrl url = lst[0];
        QFile file(url.toLocalFile());
        if (file.open(QFile::WriteOnly | QFile::Truncate)) {
            QDomDocument doc("xml");
            EntitiesManager::getManager()->fillDocument(doc);
            QTextStream out(&file);
            doc.save(out, 4);
        }
    }
}

void MainWindow::loadXML() {
    QFileDialog dialog;
    dialog.setFileMode(QFileDialog::FileMode::ExistingFile);
    qDebug() << __FUNCTION__;
    auto search = [&](QDomElement& element, auto& f) -> void {
        qDebug() << __FUNCTION__ << element.tagName();
        if (element.tagName() == "Point") {
            PointEntity* point = new PointEntity(element);
            qDebug() << point;
            point->sendToManager();
        } else if (element.tagName() == "Line") {
            LineEntity* line = new LineEntity(element);
            qDebug() << line;
            line->sendToManager();
        } else if (element.tagName() == "Polygon") {
            PolygonEntity* polygon = new PolygonEntity(element);
            qDebug() << polygon;
            polygon->sendToManager();
        } else if (element.tagName() == "Polyline") {
            PolylineEntity* polyline = new PolylineEntity(element);
            polyline->sendToManager();
        } else if (element.tagName() == "Image") {
            ImageEntity* image = new ImageEntity(element);
            image->sendToManager();
        } else {
            QDomNodeList children = element.childNodes();
            for (int i = 0; i < children.size(); ++i) {
                QDomElement e = children.at(i).toElement();
                f(e, f);
            }
        }
    };
    if (dialog.exec()) {
        QList<QUrl> lst = dialog.selectedUrls();
        QUrl url = lst[0];
        QFile file(url.toLocalFile());
        if (file.open(QFile::ReadOnly)) {
            QDomDocument doc("xml");
            doc.setContent(&file);
            QDomElement e = doc.childNodes().at(0).toElement();
            search(e, search);
        }
    }
}

DrawingTool* MainWindow::getDrawingTool() {
    return this->drawing_tools_map[selected_tool];
}

void MainWindow::deleteSelectedItem() {
    qDebug() << __FUNCTION__;
    Entity* ptr = TopologyManager::getManager()->getSelectedEntity();
    if (ptr == nullptr) return;
    layers_tool_bar->clearSelection();
    EntitiesManager::getManager()->eraseEntity(ptr, ptr->getItem());
}
