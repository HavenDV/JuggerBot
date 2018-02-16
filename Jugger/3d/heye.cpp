#include "../utils/utils.h"
#include <opencv2\viz.hpp>
#include "heye.h"

// Возвращает нормальный вектор искомой плоскости. + D
auto	points2Plane( const std::vector< cv::Point3d > & points )
-> cv::Vec4d {
	// Ax + By + Cz + D = 0  -  Уравнение плоскости
	auto A = 0.0, B = 0.0, C = 0.0, D = 0.0;

	if ( points.size() < 3 ) {
		return{ A, B, C, D };
	}

	A = ( points[ 1 ].y - points[ 0 ].y ) * ( points[ 2 ].z - points[ 0 ].z )
		- ( points[ 1 ].z - points[ 0 ].z ) * ( points[ 2 ].y - points[ 0 ].y );
	B = ( points[ 1 ].x - points[ 0 ].x ) * ( points[ 2 ].z - points[ 0 ].z )
		- ( points[ 1 ].z - points[ 0 ].z ) * ( points[ 2 ].x - points[ 0 ].x );
	C = ( points[ 1 ].x - points[ 0 ].x ) * ( points[ 2 ].y - points[ 0 ].y )
		- ( points[ 1 ].y - points[ 0 ].y ) * ( points[ 2 ].x - points[ 0 ].x );
	D = -A * points[ 0 ].x - B * points[ 0 ].y - C * points[ 0 ].z;

	//qDebug() << A << B << C << D;

	return{ A, B, C, D };
}
/*
auto	getPlanePoint( const cv::Vec4d & plane ) -> cv::Point3d {
auto x = plane[ 0 ] > 0.0 ? -plane[ 3 ] / plane[ 0 ] : 0.0;
auto y = plane[ 1 ] > 0.0 ? -plane[ 3 ] / plane[ 1 ] : 0.0;
auto z = plane[ 2 ] > 0.0 ? -plane[ 3 ] / plane[ 2 ] : 0.0;
return{ x, y, z };
}
*/

//Normal??
auto	toSpherePoint( const cv::Point2d & point, const cv::Point2d & center )
-> cv::Point3d {
	auto vec = point - center;
	auto l2 = vec.ddot( vec );
	auto r2 = std::pow( std::max( center.x, center.y ), 2.0 );
	if ( l2 < r2 ) {
		return cv::normalize( cv::Vec3d{ vec.x, vec.y, std::sqrt( r2 - l2 ) } );
	}

	return{ 0.0, 0.0, 0.0 };
}

auto	toSpherePoint( const cv::Point2d & point, const cv::Point2d & center, const cv::Vec3d & normal )
-> cv::Point3d {
	auto z = normal;
	auto y = -cv::Vec3d{ 0, 1, 0 };
	auto x = z.cross( y );
	auto pose = cv::viz::makeTransformToGlobal( x, y, z );
	return pose * toSpherePoint( point, center );
}

HEye::HEye() {}

//CHange position( as center rect ) to position in 3d;
HEye::HEye( const cv::Rect2d & _rect, const cv::Point2d & _irisCenter, double _radius, const HCamera & camera ) {
	irisCenter = _irisCenter;
	radius = _radius;

	mat = camera.getMat()( _rect );
	cv::circle( mat, irisCenter - _rect.tl(), radius, { 0, 0, 0 }, 1, cv::LINE_AA );
	auto _center = getCenterPoint( _rect );

	rectNormal = camera.getNormal();
	center = computeEyePosition( _center, camera );
	//center3d = computeCenter3D( camera );
	irisCenter3D = computeEyePosition( irisCenter, camera );
	plane = computePlane( irisCenter - _rect.tl(), radius, rectNormal );
	normal = computePlaneNormal( plane );
	pixelSize = camera.getSize().width / camera.getMat().size().width;
	size = pixelSize * mat.size();
	eyeRadius = std::max( size.width, size.height ) / 2;
}

auto HEye::empty() const -> const bool {
	return mat.empty();
}

auto	HEye::getMat() const -> const cv::Mat {
	return mat;
}

auto	HEye::getCenter() const -> const cv::Point3d {
	return center;
}

auto	HEye::getIrisCenter() const -> const cv::Point2d {
	return irisCenter;
}

auto	HEye::getRadius() const -> const double {
	return radius;
}

auto	HEye::getPlane() const -> const cv::Vec4d {
	return plane;
}

auto	HEye::getNormal() const -> const cv::Vec3d {
	return normal;
}

auto	HEye::getRectNormal() const -> const cv::Vec3d {
	return rectNormal;
}

auto	HEye::getIrisCenter3D() const -> const cv::Point3d {
	return irisCenter3D;
}

auto	HEye::getPixelSize() const -> const double {
	return pixelSize;
}

auto	HEye::getSize() const -> const cv::Size2d {
	return size;
}

//Change to fixed eye radius. Как половина длины разреза глаз + немножко еще.
auto	HEye::getEyeRadius() const -> const double {
	return eyeRadius;
}

auto HEye::getRect() const -> const cv::Rect3d {
	return{ getRectNormal(), getCenter(), getSize() };
}

//DEPRECATED
auto	HEye::computeCenter3D( const HCamera & camera ) -> cv::Point3d {
	auto && center2d = getIrisCenter();
	auto && size = camera.getMat().size();
	auto point2d = camera.to2DPoint( getCenter() );
	return camera.to3DPoint( { point2d.x + center2d.x / size.width, point2d.y + center2d.y / size.height } );
}

//replace to QPointF 0.0..1.0 position
auto	HEye::computeEyePosition( const cv::Point2d & point, const HCamera & camera )
-> cv::Point3d {
	auto && size = camera.getMat().size();
	//auto k = camera.getSize().width / std::max( camera.getMat().size().width, 1 );
	//return camera.getCenter() + cv::Point3d{ k * ( -point.x + size.width / 2 ), k * ( -point.y + size.height / 2 ), 0.0 };

	return camera.to3DPoint( { point.x / size.width, point.y / size.height } );
}

auto	HEye::computePlane( const cv::Point2d center, double radius, const cv::Vec3d & normal )
-> cv::Vec4d {
	auto leftPoint = center - cv::Point2d{ radius, 0.0 };
	auto bottomPoint = center + cv::Point2d{ 0.0, radius };
	auto rightPoint = center + cv::Point2d{ radius, 0.0 };
	auto matCenter = getCenterPoint< double >( mat );
	return points2Plane( {
		toSpherePoint( leftPoint, matCenter, normal ),
		toSpherePoint( bottomPoint, matCenter, normal ),
		toSpherePoint( rightPoint, matCenter, normal )
	} );
}

auto	HEye::computePlaneNormal( const cv::Vec4d & plane )
-> cv::Vec3d {
	return cv::normalize( cv::Vec3d{ plane[ 0 ], -plane[ 1 ], plane[ 2 ] } );
}

/*
auto	testEyePoint( const cv::Vec3b & color ) -> bool {
if ( color[ 0 ] < 10 && color[ 1 ] < 10 && color[ 2 ] < 10 ) {
return true;
}
return false;
}

auto	firstEyePoint( const cv::Mat & mat, const cv::Point & point ) -> cv::Point {
auto j = point.y;
for ( auto i = 0; i < point.x; ++i ) {
if ( testEyePoint( mat.at< cv::Vec3b >( j, i ) ) ) {
return { i, j };
}
}

return clamp( cv::Point{ point.x - 1, j }, mat );
}

auto	twoEyePoint( const cv::Mat & mat, const cv::Point & point ) -> cv::Point {
auto j = point.y;
for ( auto i = mat.cols - 1; i > point.x; --i ) {
if ( testEyePoint( mat.at< cv::Vec3b >( j, i ) ) ) {
return{ i, j };
}
}

return clamp( cv::Point{ point.x + 1, j }, mat );
}

auto	threeEyePoint( const cv::Mat & mat, const cv::Point & point ) -> cv::Point {
auto i = point.x;
for ( auto j = mat.rows - 1; j > point.y; --j ) {
if ( testEyePoint( mat.at<cv::Vec3b>( j, i ) ) ) {
return{ i, j };
}
}

return clamp( cv::Point{ i, point.y + 1 }, mat );
}
*/