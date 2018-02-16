#include "qt.h"

auto	operator "" _q( const char* str, size_t size ) -> QString {
	return str;
}

auto	toQColor( const cv::Scalar_<int> & scalar ) -> QColor {
	return{ scalar[ 0 ], scalar[ 1 ], scalar[ 2 ], 255 - scalar[ 3 ] };
}

auto	toQPoint( const cv::Point & point ) -> QPoint {
	return{ point.x, point.y };
}

auto	toQPointF( const cv::Point_< qreal > & point ) -> QPointF {
	return{ point.x, point.y };
}

auto	toQRect( const cv::Rect & rect ) -> QRect {
	return{ toQPoint( rect.tl() ), toQPoint( rect.br() ) };
}

auto	toQRectF( const cv::Rect_< qreal > & rect ) -> QRectF {
	return{ toQPointF( rect.tl() ), toQPointF( rect.br() ) };
}

auto	toCvPoint( const QPoint & point ) -> cv::Point {
	return{ point.x(), point.y() };
}

auto	toCvPointF( const QPointF & point ) -> cv::Point_< qreal > {
	return{ point.x(), point.y() };
}

auto	toCvColor( const QColor & color ) -> cv::Scalar_< int > {
	return{ color.blue(), color.green(), color.red(), 255 - color.alpha() };
}

auto	toCvRect( const QRect & rect ) -> cv::Rect {
	return{ rect.x(), rect.y(), rect.width(), rect.height() };
}

auto	toCvRectF( const QRectF & rect ) -> cv::Rect_< qreal > {
	return{ rect.x(), rect.y(), rect.width(), rect.height() };
}
