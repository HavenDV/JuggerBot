#include <QRegion>
#include "entersections.h"
#include <QDebug>

auto	ellipsesEntersection( const cv::Rect & rect1, const cv::Rect & rect2 )
-> std::vector< cv::Point > {
	QRegion region1{ rect1.x, rect1.y, rect1.width, rect1.height, QRegion::RegionType::Ellipse };
	QRegion region2{ rect2.x, rect2.y, rect2.width, rect2.height, QRegion::RegionType::Ellipse };


	auto region3 = region1.intersected( region2 );
	//qDebug() << region3;
	if ( region3.isEmpty() ) {
		return{};
	}

	auto rect3 = region3.boundingRect();
	if ( rect3.width() == 0 && rect3.height() == 0 ) {
		return{ { rect3.x(), rect3.y() },{ rect3.x(), rect3.y() } };
	}

	if ( rect3.width() > rect3.height() ) {
		return{ { rect3.x(), rect3.y() },{ rect3.x() + rect3.width(), rect3.y() } };
	}
	else {
		return{ { rect3.x(), rect3.y() },{ rect3.x(), rect3.y() + rect3.height() } };
	}

	return{ { rect3.x(), rect3.y() },{ rect3.x() + rect3.width(), rect3.y() },{ rect3.x(), rect3.y() },{ rect3.x(), rect3.y() + rect3.height() } };
}

auto	ellipsesEntersection( const cv::RotatedRect & ellipse1, const cv::RotatedRect & ellipse2 )
-> std::vector< cv::Point > {
	return ellipsesEntersection( ellipse1.boundingRect(), ellipse2.boundingRect() );
}