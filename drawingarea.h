#pragma once

#include <QObject>
#include <QWidget>
#include <QGraphicsView>
#include <QMouseEvent>
#include <QDebug>
#include <iostream>
#include <QWheelEvent>

#include "drawingtool.h"
#include "linedrawingtool.h"
#include "highlighteffect.h"

class DrawingArea : public QGraphicsView
{
    Q_OBJECT
private:
    QGraphicsDropShadowEffect* item_selection_effect = nullptr;
    bool mouse_pressed = false;
    QPointF press_pos;
public:
    DrawingArea(QWidget* parent);

    QGraphicsEffect* getItemSelectionEffect();
protected slots:
    void wheelEvent(QWheelEvent *event) override;
};
