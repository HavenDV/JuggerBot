#include "heyewidget.h"
#include "loaders/himageloader.h"
#include <QTimer>
#include <QPainter>
#include <QApplication>
#include <QScreen>

HEyeWidget::HEyeWidget( QWidget *parent )
	: QWidget{ parent }, label{ this }, size{ 200, 200 }, 
	position{ 0, 0 }, color{ 0, 255, 0, 55 } {
	setWindowFlags( Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint );
	setAttribute( Qt::WA_TranslucentBackground );
	QTimer::singleShot( 1, this, &HEyeWidget::update );
}

auto	HEyeWidget::normalizePosition( const QPointF & point ) -> QPoint {
	auto screen = QApplication::primaryScreen();
	auto screenSize = screen->size();

	auto x = static_cast< int >( point.x() * screenSize.width() );
	auto y = static_cast< int >( point.y() * screenSize.height() );
	x = std::min( std::max( x, 0 ), screenSize.width() ); // - size.width()
	y = std::min( std::max( y, 0 ), screenSize.height() ); // - size.height()

	return{ x, y };
}

void	HEyeWidget::update() {
	if ( position.isNull() ) {
		QTimer::singleShot( 1000, this, &HEyeWidget::update );
		return;
	}

	QPixmap pixmap{ size };
	pixmap.fill( Qt::transparent );
	QPainter painter{ &pixmap };
	painter.setFont( QFont{ "", 32 } );
	painter.setBrush( color );
	painter.drawEllipse( pixmap.rect() );
	painter.drawText( pixmap.rect(), u8"Привет", { Qt::Alignment::enum_type::AlignCenter } );
	painter.end();

	label.resize( pixmap.size() );
	label.setPixmap( pixmap );
	resize( pixmap.size() );
	show();
	move( position );
	
	QTimer::singleShot( 1, this, &HEyeWidget::update );
}

void	HEyeWidget::setPosition( const QPointF & point ) noexcept {
	position = normalizePosition( point );
	position -= { size.width() / 2, size.height() / 2 };
}

void	HEyeWidget::setColor( const QColor & _color ) noexcept {
	color = _color;
}

auto	HEyeWidget::getPosition() const noexcept -> const QPoint {
	return position;
}

auto	HEyeWidget::getColor() const noexcept -> const QColor {
	return color;
}