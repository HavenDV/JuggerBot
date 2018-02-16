#pragma once
#include "../utils/utils.h"
#include <opencv2/opencv.hpp>

namespace cv {
	template < typename T1 >
	class Rect3_ : cv::Rect_< T1 > {

	public:

		Rect3_() : cv::Rect_< T1 >() {}

		Rect3_( const cv::Point3_< T1 > & point1, const cv::Point3_< T1 > & point2, cv::Vec< T1, 3 > _upVector = { 0, 1, 0 } ) :
			cv::Rect_< T1 >( cv::Point_< T1 >{ point1.x, point1.y }, cv::Point_< T1 >{ point2.x, point2.y } ) {
			m_tl = { point1.x, point1.y, point1.z };
			m_tr = { point2.x, point1.y, point1.z };
			m_bl = { point1.x, point2.y, point2.z };
			m_br = { point2.x, point2.y, point2.z };
			upVector = cv::normalize( _upVector );
			
			computeParameters();
		}

		Rect3_( const cv::Rect_< T1 > & rect, T1 _z1, T1 _z2, cv::Vec< T1, 3 > _upVector = { 0, 1, 0 } ) :
			cv::Rect3_< T1 >( cv::Point_< T1 >( rect.tl(), z1 ), cv::Point_< T1 >( rect.br(), z2 ), _upVector ) {}

		Rect3_( const cv::Vec< T1, 3 > & _normal, const cv::Point3_< T1 > & _center, const cv::Size_< T1 > & _size, cv::Vec< T1, 3 > _upVector = { 0, 1, 0 } ) {
			normal = cv::normalize( _normal );
			center = _center;
			size = _size;
			upVector = cv::normalize( _upVector );

			computePoints();
		}

		void	computePoints() {
			auto ny = upVector;
			auto nx = - normal.cross( ny );
			auto px = cv::Point3_< T1 >{ nx * size.width / 2 };
			auto py = cv::Point3_< T1 >{ ny * size.height / 2 };

			m_tl = center - px + py;
			m_tr = center + px + py;
			m_bl = center - px - py;
			m_br = center + px - py;
		}

		void	computeParameters() {
			auto vec1 = tr() - tl();
			auto vec2 = bl() - tl();
			normal = cv::normalize( cv::Vec< T1, 3 >( vec1.cross( vec2 ) ) );
			center = ( tl() + br() ) / 2;
			size = { std::sqrt( vec1.ddot( vec1 ) ), std::sqrt( vec2.ddot( vec2 ) ) };
		}
 
		auto	tl() const -> const cv::Point3_< T1 > {
			return m_tl;
		}

		auto	tr() const -> const cv::Point3_< T1 > {
			return m_tr;
		}

		auto	bl() const -> const cv::Point3_< T1 > {
			return m_bl;
		}

		auto	br() const -> const cv::Point3_< T1 > {
			return m_br;
		}

		auto	points() const -> const std::vector< cv::Point3_< T1 > > {
			return{ tl(), tr(), bl(), br() };
		}

		auto	getNormal() const -> const cv::Vec< T1, 3 > {
			return normal;
		}

		auto	getUpVector() const -> const cv::Vec< T1, 3 > {
			return upVector;
		}

		auto	getDirectionVector() const -> const cv::Vec< T1, 3 > {
			return getNormal().cross( getUpVector() );
		}

		auto	getCenter() const -> const cv::Point3_< T1 > {
			return center;
		}

		auto	getSize() const -> const cv::Size_< T1 > {
			return size;
		}

		auto	getPose() const -> const cv::Affine3d {
			auto && z = getNormal();
			auto && y = -getUpVector();
			auto x = z.cross( y );
			return cv::viz::makeTransformToGlobal( x, y, z, getCenter() );
		}

		auto	getAngleVector() const -> const cv::Vec< T1, 3 > {
			return getPose().rvec();;
		} 

		auto	getRotationMatrix() const -> cv::Matx< double, 3, 3 > {
			return getPose().rotation();
		}

		void	setCenter( const cv::Point3_< T1 > & _center ) {
			center = _center;

			computePoints();
		}

		//return 3d point from { 0.0..1.0, 0.0..1.0 } point;
		auto	to3DPoint( const cv::Point2d & point ) const -> cv::Point3_< T1 > {
			auto vecx = tr() - tl();
			auto vecy = bl() - tl();
			auto vecxy = point.x * vecx + point.y * vecy;
			return tl() + vecxy;
		}

		auto	project( const cv::Point3_< T1 > & point, const cv::Vec< T1, 3 > & normal ) const -> cv::Point3_< T1 > {
			auto n = cv::Point3d( cv::normalize( getNormal() ) );
			auto s = getCenter();
			auto np = cv::Point3d( cv::normalize( normal ) );
			auto p = point;

			auto vs = p - s;
			auto l = std::abs( n.ddot( vs ) );
			auto vp = -l * n;
			auto cosa = np.ddot( vp ) / std::sqrt( np.ddot( np ) ) / std::sqrt( vp.ddot( vp ) );
			auto lp = l / cosa;
			auto v = lp * np;

			return p + v;
		}

		auto	project( const cv::Point3_< T1 > & point ) const -> cv::Point3_< T1 > {
			return project( point, -getNormal() );
		}

		//return { 0.0..1.0, 0.0..1.0 } projected point for screen
		auto	to2DPoint( const cv::Point3_< T1 > & point, const cv::Vec< T1, 3 > & normal ) const -> cv::Point2d {
			auto v = project( point, normal ); 
			auto z = cv::Point3d( getNormal() );
			auto y = cv::Point3d{ -getUpVector() };
			auto x = z.cross( y );
			auto xp = v.ddot( x );
			auto yp = v.ddot( y );
			//auto xp = v.ddot( { n.y * y.z - n.z * y.y, n.z * y.x - n.x * y.z, n.x * y.y - n.y * y.x } );
			
			auto && size = getSize();
			return clamp( cv::Point2d{ 0.5 + xp / size.width, 0.5 + yp / size.height }, cv::Rect2d{ 0, 0, 1, 1 } );
		}

		//return { 0.0..1.0, 0.0..1.0 } projected point for screen
		auto	to2DPoint( const cv::Point3_< T1 > & point ) const -> cv::Point2d {
			return to2DPoint( point, -getNormal() );
		}

	private:

		Vec< T1, 3 >		normal, upVector;
		cv::Size_< T1 >		size;
		cv::Point3_< T1 >	center;
		cv::Point3_< T1 >	m_tl, m_tr, m_bl, m_br;

	};

	typedef Rect3_< double > Rect3d;
	typedef Rect3_< float > Rect3f;
	typedef Rect3_< int > Rect3i;
}