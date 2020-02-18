#include "drawingarea.h"

DrawingArea::DrawingArea(QWidget* parent) : QGraphicsView(parent) {
    item_selection_effect = new QGraphicsDropShadowEffect;
    item_selection_effect->setBlurRadius(10.0f);
    item_selection_effect->setColor(Qt::blue);
//    item_selection_effect->setOffset(0.0f, 0.0f);
}

void DrawingArea::wheelEvent(QWheelEvent *event) {
    if (event->angleDelta().y() > 0) {
        scale(1.25, 1.25);
    } else {
        scale(0.8, 0.8);
    }
}

QGraphicsEffect* DrawingArea::getItemSelectionEffect() {
    QGraphicsDropShadowEffect* eff = new QGraphicsDropShadowEffect;
    eff->setBlurRadius(item_selection_effect->blurRadius());
    eff->setOffset(item_selection_effect->offset());
    eff->setColor(item_selection_effect->color());
    return eff;
}
