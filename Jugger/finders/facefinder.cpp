#include "facefinder.h"
#include "../utils/utils.h"
#include <QDebug>

const auto pathToXml = "haarcascades/"_s;
const auto faceName = "haarcascade_frontalface_alt.xml"_s;
const auto detectedRadius = 100;

FaceFinder::FaceFinder() {}

void	FaceFinder::initialise() {
	auto path = pathToXml + faceName;
	if ( cascade.empty() && !cascade.load( path ) ) {
		qWarning() << "Cascade load error: " << path.c_str();
		return;
	}

	initialised = true;
}

auto	FaceFinder::isInitialised() -> const bool {
	return initialised;
}

auto	FaceFinder::isDetected() -> const bool {
	return !faces.empty();
}

auto	FaceFinder::getFaces() -> const std::vector< cv::Rect > {
	return faces;
}

auto	FaceFinder::find( const cv::Mat & mat ) -> const std::vector< cv::Rect > {
	if ( mat.empty() ) {
		qWarning() << "FaceFinder::find - Mat is empty"; //throw std::invalid_argument( "Mat is empty" );
		return{};
	}

	if ( !isInitialised() ) {
		initialise();
	}
	
	if ( cascade.empty() ) {
		qWarning() << "FaceFinder::find - Cascade is empty";
		return{};
	}
	/*
	auto isFirst = !isDetected();
	auto rect = isFirst ? toRect( mat.size() ) : clamp( cv::Rect{ faces[ 0 ].x - detectedRadius, faces[ 0 ].y - detectedRadius, faces[ 0 ].width + 2 * detectedRadius, faces[ 0 ].height + 2 * detectedRadius }, mat );
	auto gray = isFirst ? toGray( mat ) : toGray( mat( rect ) );
	faces.clear();
	cascade.detectMultiScale( gray, faces, 1.1, 2, cv::CASCADE_SCALE_IMAGE, ( isFirst ? cv::Size{ 20, 20 } : rect.size() / 2 ) );
	for ( auto && face : faces ) {
		face += rect.tl();
	}
	*/

	cascade.detectMultiScale( toGray( mat ), faces, 1.1, 2, cv::CASCADE_SCALE_IMAGE, { 20, 20 } );
	return faces;
}