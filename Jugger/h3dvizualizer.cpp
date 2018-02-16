#include "h3dvizualizer.h"
#include "utils/utils.h"
#include "utils/imageconverter.h"
#include "utils/qt.h"
#include <QDebug>

constexpr auto operator "" _sm( long double value ) -> double {
	//constexpr auto scale = 1.0; // 1.0 / SantimetrInOne
	return 1.0 * value;
}

constexpr auto EYE_RADIUS = 1.1_sm;
constexpr auto IRIS_RADIUS = 0.6_sm;
constexpr auto EYE_WIDTH = 3.2_sm;

// add normalize as vec / vec.lenght ?
H3DVizualizer::H3DVizualizer() :
	camera1{ {}, { 0, 0, 0 }, cv::Rect3d{ { 0.0, 0.0, -1.0 }, { 0.0, 0.0, 0.0 }, cv::Size2d{ 60.0_sm, 45.0_sm } } },
	screen1{ {}, cv::Rect3d{ { 0.0, 0.0, 1.0 }, { 0.0, 0.0, 0.0 }, cv::Size2d{ 37.7_sm, 21.4_sm } } } {
	camera1.setPosition( { 0.0_sm, screen1.getSize().height / 2 + 1.3_sm, 0.0_sm } );
	camera1.setDistance( 65.0_sm );
}

void	H3DVizualizer::init() {
	window = cv::viz::getWindowByName( "3DVizualizer" );

	QTimer::singleShot( 1, this, &H3DVizualizer::update );
}

void	H3DVizualizer::prepare() {
	HThread::prepare();
	window.close();
	cv::viz::unregisterAllWindows();
}

void	H3DVizualizer::showVizScreen( const QImage & image ) {
	screen1.setMat( ImageConverter{ image } );
}

void	H3DVizualizer::showVizCamera( const QImage & image ) {
	camera1.setMat( ImageConverter{ image } );
}

void	H3DVizualizer::setVizEyeParams( const QRectF & rect, const QPointF & center, double radius ) {
	//camera1.setDistance( 5.0 * 640.0 / image.size().width() );
	eye1 = { toCvRectF( rect ), toCvPointF( center ), radius, camera1 };
}

void	H3DVizualizer::setAlternativeVizEyeParams( const QRectF & rect, const QPointF & center, double radius ) {
	eye2 = { toCvRectF( rect ), toCvPointF( center ), radius, camera1 };
}

void	H3DVizualizer::showLine( const cv::String & name, const cv::Point3d & point1, const cv::Point3d & point2, const cv::viz::Color & color, double width ) {
	auto widget = cv::viz::WLine{ point1, point2, color };
	if ( width != 1.0 ) {
		widget.setRenderingProperty( cv::viz::LINE_WIDTH, width );
	}
	window.showWidget( name, widget );
}

void	H3DVizualizer::showRect( const cv::String & name, const cv::Rect3d & rect, const cv::viz::Color & color, double width ) {
	showLine( name + " Top", rect.tl(), rect.tr(), color, width );
	showLine( name + " Left", rect.tl(), rect.bl(), color, width );
	showLine( name + " Right", rect.tr(), rect.br(), color, width );
	showLine( name + " Bot", rect.bl(), rect.br(), color, width );
}

void	H3DVizualizer::showLinesToRect( const cv::String & name, const cv::Point3d & start, const cv::Rect3d & rect, const cv::viz::Color & color, double width ) {
	showLine( name + " Top Left", start, rect.tl(), color, width );
	showLine( name + " Top Right", start, rect.tr(), color, width );
	showLine( name + " Bot Left", start, rect.bl(), color, width );
	showLine( name + " Bot Right", start, rect.br(), color, width );
}

void	H3DVizualizer::showImage( const cv::String & name, const cv::Rect3d & rect, const cv::Mat & mat ) {
	window.showWidget( name, cv::viz::WImage3D{ mat, rect.getSize() }, rect.getPose() );
}
/*
void	H3DVizualizer::showTest( const HScreen & screen ) {
	showScreen( "Screen Test", screen );
}
*/
//ѕрив€зать HEye к HCamera. HCamera::addEye( params );
void	H3DVizualizer::update() noexcept {
	if ( isPrepared() || isStopped() ) {
		return;
	}

	try {
		window.showWidget( "Coordinate", cv::viz::WCoordinateSystem{ 1.0 } );

		emit setEyePoint( toQPointF( filter.correct( screen1.to2DPoint( eye1 ) ) ) );
		emit setAlternativeEyePoint( toQPointF( screen1.to2DPoint( eye2 ) ) );
		showEye( "Eye 1", eye1, screen1, { 0, 255, 0 }, { 0, 0, 240, 160 } );
		showEye( "Eye 2", eye2, screen1, { 0, 0, 255 } );
		showCamera( "Camera 1", camera1 );
		showScreen( "Screen 1", screen1 );
		window.spinOnce( 1, false );
	} 
	catch ( const cv::Exception & exception ) {
		print( exception );
	}

	QTimer::singleShot( 1, this, &H3DVizualizer::update );
}

void	H3DVizualizer::showScreen( const cv::String & name, const HScreen & screen ) {
	if ( screen.empty() ) {
		return;
	}

	showImage( name, screen, screen.getMat() );
	showRect( name + " Border", screen, cv::viz::Color::black(), 20.0 );
}

void H3DVizualizer::showCamera( const cv::String & name, const HCamera & camera ) {
	if ( camera.empty() ) {
		return;
	}

	showImage( name, camera, camera.getMat() );
	showLinesToRect( name + " Camera Line", camera.getPosition(), camera, cv::viz::Color::red() );
}

//add support for normal
auto	generateMesh( const cv::Mat_< cv::Point3f > & cloud, const cv::Mat & texture, const cv::Vec3d & normal ) -> cv::viz::Mesh {
	if ( cloud.cols < 4 && cloud.rows < 2 ) {
		return{};
	}

	std::vector< cv::Vec3d > points;
	std::vector< cv::Vec2d > coords;
	std::vector< int > polygons;

	auto width = cloud.cols;
	auto height = cloud.rows;
	auto center = getCenterPoint< int >( cloud );
	auto delta = cloud.at< cv::Point3f >( center ).x - cloud.at< cv::Point3f >( center + cv::Point{ 1, 0 } ).x;
	
	for ( int x = 0; x < width; ++x ) {
		for ( int y = 0; y < height; ++y ) {
			auto && point = cloud.at< cv::Point3f >( { x, y } );
			points.push_back( { point.x, point.y, point.z } );
			points.push_back( { point.x, point.y + delta, point.z } );
			points.push_back( { point.x + delta, point.y, point.z } );
			points.push_back( { point.x + delta, point.y + delta, point.z } );

			points.push_back( { point.x, point.y, point.z + delta } );
			points.push_back( { point.x, point.y + delta, point.z + delta } );
			points.push_back( { point.x + delta, point.y, point.z + delta } );
			points.push_back( { point.x + delta, point.y + delta, point.z + delta } );

			auto texturePoint = cv::Vec2d{ ( 1.0 * x ) / height, ( 1.0 * y ) / width };
			for ( auto i = 0; i < 8; ++i ) {
				coords.push_back( texturePoint );
			}
		}
	}

	for ( int x = 0; x < width; ++x ) {
		for ( int y = 0; y < height; ++y ) {
			polygons.push_back( 4 );
			for ( int i = 0; i < 4; ++i ) {
				polygons.push_back( 8 * ( x * height + y ) + i );
			}

			polygons.push_back( 4 );
			for ( int i = 0; i < 2; ++i ) {
				polygons.push_back( 8 * ( x * height + y ) + i );
				polygons.push_back( 8 * ( x * height + y ) + i + 4 );
			}

			polygons.push_back( 4 );
			for ( int i = 2; i < 4; ++i ) {
				polygons.push_back( 8 * ( x * height + y ) + i );
				polygons.push_back( 8 * ( x * height + y ) + i + 4 );
			}

			polygons.push_back( 4 );
			polygons.push_back( 8 * ( x * height + y ) + 1 );
			polygons.push_back( 8 * ( x * height + y ) + 4 + 1 );
			polygons.push_back( 8 * ( x * height + y ) + 3 );
			polygons.push_back( 8 * ( x * height + y ) + 4 + 3 );

			polygons.push_back( 4 );
			polygons.push_back( 8 * ( x * height + y ) + 0 );
			polygons.push_back( 8 * ( x * height + y ) + 4 + 0 );
			polygons.push_back( 8 * ( x * height + y ) + 2 );
			polygons.push_back( 8 * ( x * height + y ) + 4 + 2 );
		}
	}

	auto mesh = cv::viz::Mesh{};
	mesh.cloud = cv::Mat( points, true ).reshape( 3, 1 );
	mesh.tcoords = cv::Mat( coords, true ).reshape( 2, 1 );
	mesh.polygons = cv::Mat( polygons, true ).reshape( 1, 1 );
	mesh.normals = normal;
	mesh.texture = texture;
	
	return mesh;
}

// –ассто€ние между двум€ позами можно вычислить так
//double distance = cv::norm( ( cam2_to_global.inv() * cam1_to_global ).translation() );
//double rotation_angle = cv::norm( ( cam2_to_global.inv() * cam1_to_global ).rvec() );

// копируем облако точек и выставл€ем часть точек в NAN - такие точки будут проигнорированы
//float qnan = std::numeric_limits< float >::quiet_NaN();
//cv::Mat masked_cloud = cloud.clone();
//for ( int i = 0; i < cloud.total(); ++i )
//	if ( i % 16 != 0 )
//		masked_cloud.at<Vec3f>( i ) = Vec3f( qnan, qnan, qnan );

// Aвтоматическа€ раскраска, полезно если у нас нет цветов
// WPaintedCloud( cloud )

//viz.showWidget( "image", WImageOverlay( display, Rect( 0, 0, 240, 160 ) ) );
//Affine3d camera1_to_camera2 = camera2_to_global.inv() * camera1_to_global
//cv::viz::makeCameraPose( pos, view_dir, y )

void	H3DVizualizer::showEyeMesh( const cv::String & name, const cv::Mat & mat, const cv::Point3d & pos, const cv::Vec3d & normal, double radius ) {
	auto cloud = cv::Mat_< cv::Point3f >{ mat.size() };
	auto center = getCenterPoint< double >( mat );
	cloud.forEach( [ &radius, &pos, &center, &normal ] ( auto && value, auto && position ) {
		auto point = cv::Point{ position[ 1 ], position[ 0 ] };
		value = pos + radius * toSpherePoint( point, center, normal );
	} );
	
	window.showWidget( name, cv::viz::WMesh{ generateMesh( cloud, mat, normal ) } );
}

void	H3DVizualizer::showScreenCloud( const cv::String & name, const HEye & eye, const cv::Rect3d & screen, const cv::viz::Color & color ) {
	auto && mat = eye.getMat();
	auto && pos = eye.getIrisCenter3D();
	auto && normal = eye.getRectNormal();
	auto cloud = cv::Mat_< cv::Point3f >{ mat.size(), { 0, 0, 0 } };
	auto center = getCenterPoint< double >( mat );
	cloud.forEach( [ &pos, &mat, &screen, &center, &normal, &color ] ( auto && value, auto && position ) { //&scale, 
		auto point = cv::Point{ position[ 1 ], position[ 0 ] };
		auto spherePoint = toSpherePoint( point, center, normal );
		auto && matColor = mat.at< cv::Vec3b >( point );
		if ( matColor[ 1 ] < 25 ) {//|| color == cv::viz::Color::green() ) { // Red > Green > Blue
			value = screen.project( pos, spherePoint );
		}
	} );
	//mat - project eye
	auto colors = asColor( !( 10 * toGray( mat ) ), color == cv::viz::Color::green() ? Colors::Green : Colors::Red );
	auto widget = cv::viz::WCloud{ cloud, colors };
	widget.setRenderingProperty( cv::viz::POINT_SIZE, 20.0 );
	window.showWidget( name, widget );
}

void	H3DVizualizer::showScreenEyeCircle( const cv::String & name, const cv::Point3d & point, const cv::Vec3d & normal, const cv::viz::Color & color ) {
	window.showWidget( name, cv::viz::WCircle{ 5.0, point, normal, 1.00, color } );
}

void	H3DVizualizer::showEyePlane( const cv::String & name, const cv::Mat & mat, double radius, const cv::Point3d & position, const cv::Vec3d & normal, const cv::viz::Color & color ) {
	auto k = 0.94;
	auto kr = std::sqrt( 1 - k * k );
	auto center = position + radius * cv::Point3d{ normal };
	 
	window.showWidget( name, cv::viz::WCircle{ kr * radius, center, normal, 0.1, color } );
}

void	H3DVizualizer::showEye( const cv::String & name, const HEye & eye, const HScreen & screen, const cv::viz::Color & color, const cv::Rect & previewRect ) {
	if ( eye.empty() ) {
		return;
	}
	
	//get center change to eyecenter
	showEyeMesh( name + " Mesh", eye.getMat(), eye.getCenter(), eye.getRectNormal(), eye.getEyeRadius() );
	showEyePlane( name + " Plane", eye.getMat(), eye.getEyeRadius(), eye.getCenter(), eye.getNormal(), color );

	if ( !empty( previewRect ) ) {
		window.showWidget( name + " Preview", cv::viz::WImageOverlay{ eye.getMat(), previewRect } );
	}

	if ( screen.empty() ) {
		return;
	}

	showScreenCloud( name + " Screen Cloud", eye, screen, color );
	showLinesToRect( name + " Eye Line", eye.getIrisCenter3D(), screen, color );
	showScreenEyeCircle( name + " Screen Circle", screen.project( eye ), screen.getNormal(), color );
}