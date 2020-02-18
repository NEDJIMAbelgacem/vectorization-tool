#include "highlighteffect.h"

HighlightEffect::HighlightEffect( qreal offset ) :
 QGraphicsEffect(),
 mColor( 255, 255, 0, 128 ), // yellow, semi-transparent
 mOffset( offset, offset ) {}

QRectF HighlightEffect::boundingRectFor( const QRectF& sourceRect) const
{
 return sourceRect.adjusted( -mOffset.x(), -mOffset.y(), mOffset.x(), mOffset.y() );
}

void HighlightEffect::draw( QPainter* painter )
{
 QPoint offset;
 QPixmap pixmap;

 // if ( sourceIsPixmap() ) // doesn't seems to work, return false
 // {
 // No point in drawing in device coordinates (pixmap will be scaled anyways).
 pixmap = sourcePixmap( Qt::LogicalCoordinates, &offset);
 //}

 QRectF bound = boundingRectFor( pixmap.rect() );

 painter->save();
 painter->setPen( Qt::NoPen );
 painter->setBrush( mColor );
 QPointF p( offset.x()-mOffset.x(), offset.y()-mOffset.y() );
 bound.moveTopLeft( p );
 painter->drawRoundedRect( bound, 5, 5, Qt::RelativeSize );
 painter->drawPixmap( offset, pixmap );
 painter->restore();
}
