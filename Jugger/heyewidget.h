#pragma once
#include <QWidget>
#include <QLabel>
#include <QPointF>

class HEyeWidget : public QWidget {
	Q_OBJECT

public:
	HEyeWidget( QWidget * parent = nullptr );

	auto	normalizePosition( const QPointF & point ) -> QPoint;

	void	update();

	void	setPosition( const QPointF & point ) noexcept;
	void	setColor( const QColor & _color ) noexcept;
	auto	getPosition() const noexcept -> const QPoint;
	auto	getColor() const noexcept -> const QColor;

private:
	QLabel			label;
	QPoint			position;
	QSize			size;
	QColor			color;

};
