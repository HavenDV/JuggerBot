#pragma once
#include <QColor>
#include <QPoint>
#include <QPointF>
#include <QRect>
#include <opencv2/opencv.hpp>

auto	operator "" _q( const char* str, size_t size ) -> QString;

auto	toQColor( const cv::Scalar_<int> & scalar ) -> QColor;
auto	toQPoint( const cv::Point & point ) -> QPoint;
auto	toQPointF( const cv::Point_< qreal > & point ) -> QPointF;
auto	toQRect( const cv::Rect & rect ) -> QRect;
auto	toQRectF( const cv::Rect_< qreal > & rect ) -> QRectF;

auto	toCvPoint( const QPoint & point ) -> cv::Point;
auto	toCvPointF( const QPointF & point ) -> cv::Point_< qreal >;
auto	toCvColor( const QColor & color ) -> cv::Scalar_< int >;
auto	toCvRect( const QRect & rect ) -> cv::Rect;
auto	toCvRectF( const QRectF & rect ) -> cv::Rect_< qreal >;
