#pragma once

#include <QMainWindow>
#include <QDebug>
#include <QToolBar>
#include <QGraphicsView>
#include <QGraphicsLineItem>
#include <QGraphicsPolygonItem>
#include <QMouseEvent>
#include <QIcon>
#include <QVector>
#include <QFileDialog>
#include <QDir>
#include <QComboBox>
#include <QColorDialog>
#include <QColor>
#include <QSpinBox>
#include <QLabel>
#include <QRadioButton>

#include "drawingarea.h"
#include "drawingtool.h"
#include "polygondrawingtool.h"
#include "handmovingtool.h"
#include "pointsdrawingtool.h"
#include "drawingconfig.h"
#include "canvasscene.h"
#include "layerstoolbar.h"
#include "polylinedrawingtool.h"
#include "itemsmovingtool.h"
#include "image.h"
#include "framelesswindow/framelesswindow.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class CanvasScene;
class DrawingArea;
class DrawingConfig;
class LayersToolBar;

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    static MainWindow* window;
    static DrawingArea* drawing_area;
    static CanvasScene* drawing_scene;
    static DrawingConfig* drawing_config;
    static QLabel* mouse_position_label;
    static LayersToolBar* layers_tool_bar;
    static FramelessWindow* frameless;

    QMap<QString, DrawingTool*> drawing_tools_map;
    QVector<QString> drawing_tools_vect;
    QString selected_tool;
public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    DrawingTool* getDrawingTool();
    void enableItemDelete(bool activate) {
        this->delete_item_action->setEnabled(activate);
    }
    void switchDrawingTool() {
        int index = this->item_creation_cb->currentIndex();
        this->item_creation_cb->setCurrentIndex((index + 1) % drawing_tools_map.size());
    }

    QPen getItemSelectionPen() {
        QPen pen;
        pen.setColor(QColor(0, 0, 255, 200));
        return pen;
    }
private:
    Ui::MainWindow *ui;

    QAction* delete_item_action = nullptr;
    QComboBox* item_creation_cb = nullptr;
private slots:
    void loadMap();
    void saveXML();
    void loadXML();
    void deleteSelectedItem();
    void exit() {
        if (frameless != nullptr) frameless->close();
    }
protected:
    void keyPressEvent(QKeyEvent* event) {
        qDebug() << __FUNCTION__;
        if (event->key() == Qt::Key::Key_Tab) {
            switchDrawingTool();
        }
    }
};
