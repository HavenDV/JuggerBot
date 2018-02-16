#include "../utils/utils.h"
#include "detector.h"
#include <QDebug>

const auto pathToXml = "haarcascades/"_s;
const auto faceName = "haarcascade_frontalface_alt.xml"_s;
const auto eyesName = "haarcascade_eye_tree_eyeglasses.xml"_s;
const auto smileName = "haarcascade_smile.xml"_s;

//Радиус повторного поиска глаза. Чем меньше,
//тем алгоритм быстрее, но менее чувствителен
//к резким сдвигам.
const auto eyeRadius = 100;
//Время(в кадрах), в течении которого данные
//по глазу считаются актуальными.
const auto lifetime = 15;
//Автоматически корректировать полученное изображение
//относительно центра глаза.
const auto autoCorrect = false;	

bool	operator < ( cv::Scalar a, cv::Scalar b ) {
	auto aSum = a[ 0 ] + a[ 1 ] + a[ 2 ];
	auto bSum = b[ 0 ] + b[ 1 ] + b[ 2 ];
	return aSum < bSum;
}

bool	operator > ( cv::Scalar a, cv::Scalar b ) {
	return b < a;
}

Detector::Detector() : time{ 0 } {

}

void	Detector::loadCascade( cv::CascadeClassifier & cascade, const std::string & path ) {
	if ( !cascade.empty() ) {
		return;
	}

	if ( !cascade.load( path ) ) {
		qWarning() << "Cascade load error: " << path.c_str();
	}
}

void	Detector::initialise( bool reload ) {
	if ( !reload && isInitialised() ) {
		//return;
	}

	loadCascade( faceCascade, pathToXml + faceName );
	loadCascade( eyesCascade, pathToXml + eyesName );
	loadCascade( smileCascade, pathToXml + smileName );

	initialised = true;
}

auto	Detector::isInitialised() -> const bool {
	return initialised;
}

auto	Detector::isDetected() -> const bool {
	return time > 0 ? true : false;
}

auto	Detector::getEyeRect() -> const cv::Rect {
	return rect;
}

auto	Detector::getEyePoint() -> const cv::Point {
	return point;
}

auto	Detector::getInfo() -> const std::vector< std::pair< cv::Rect, std::vector< cv::Rect > > > {
	return answer;
}

auto	Detector::getEyes() -> const std::vector< cv::Rect > {
	std::vector< cv::Rect > eyes;
	for ( auto && face : answer ) {
		for ( auto && eye : face.second ) {
			eyes.push_back( eye ); // + face.first.tl()
		}
	}
	return eyes;
}

auto	Detector::getFaces() -> const std::vector< cv::Rect > {
	std::vector< cv::Rect > faces;
	for ( auto && face : answer ) {
		faces.push_back( face.first );
	}
	return faces;
}

auto	toMatCoord( const std::vector< cv::Rect > & rects, const cv::Rect & roi, const cv::Size & expandedSize = {} )
-> std::remove_reference_t< decltype( rects ) > {
	auto out = rects;
	for ( auto && rect : out ) {
		rect = toMatCoord( rect, roi, expandedSize );
	}

	return out;
}

auto	Detector::detectSmiles( const cv::Mat & gray, const cv::Rect & roi ) -> const std::vector< cv::Rect > {
	if ( gray.empty() ) {
		qWarning() << "detectSmiles grayMat empty";
		return{};
	}
	initialise();
	if ( smileCascade.empty() ) {
		qWarning() << "smileCascade empty";
		return{};
	}

	std::vector< cv::Rect > smiles;
	smileCascade.detectMultiScale( empty( roi ) ? gray : gray( roi ), smiles, 1.1, 2, cv::CASCADE_SCALE_IMAGE );
	smiles = toMatCoord( smiles, roi );
	return smiles;
}

auto	Detector::detectFaces( const cv::Mat & gray, const cv::Rect & roi ) -> const std::vector< cv::Rect > {
	if ( gray.empty() ) {	
		qWarning() << "detectFaces grayMat empty";
		return{};
	}

	initialise();
	if ( faceCascade.empty() ) {
		qWarning() << "faceCascade empty";
		return{};
	}

	std::vector< cv::Rect > faces;
	faceCascade.detectMultiScale( empty( roi ) ? gray : gray( roi ), faces, 1.1, 2, cv::CASCADE_SCALE_IMAGE, { 20, 20 } );
	faces = toMatCoord( faces, roi );
	return faces;
}

auto	Detector::detectEyes( const cv::Mat & gray, const cv::Rect & roi ) -> const std::vector< cv::Rect > {
	if ( gray.empty() ) {
		qWarning() << "getEyes grayMat empty";
		return{};
	}

	initialise();
	if ( eyesCascade.empty() ) {
		qWarning() << "eyesCascade empty";
		return{};
	}
	
	std::vector< cv::Rect > eyes;
	eyesCascade.detectMultiScale( empty( roi ) ? gray : gray( roi ), eyes, 1.1, 2, cv::CASCADE_SCALE_IMAGE );
	eyes = toMatCoord( eyes, roi );
	return eyes;
}


auto	Detector::getColorfulEye( const cv::Mat & mat, const cv::Rect & roi ) -> const cv::Rect {
	auto gray = toGray( mat, true );

	if ( time > 0 ) {
		--time;

		if ( gray.empty() || !detectEyes( gray, rect ).empty() ) {
			return rect;
		}

		auto findRect = expand( rect, eyeRadius );

		//Возможно перекрестное определение. Можно немного улучшить алгоритм авто корректировки, добавив поиск минимального отличия от прошлой точки.
		for ( auto && eye : detectEyes( gray, findRect ) ) {
			time = lifetime;
			if ( autoCorrect ) {
				//Need update answer
				for ( auto && face : answer ) {
					for ( auto && eyeAnswer : face.second ) {
						if ( eyeAnswer == rect ) {
							eyeAnswer = eye;
						}
					}
				}
				point = getCenterPoint( eye );// +findRect.tl();
				rect = eye;// +findRect.tl();
			}
		}

		return rect;
	}
	
	answer.clear();
	for ( auto && face : detectFaces( gray, roi ) ) {
		auto maxMean = cv::Scalar::all( 0 );

		auto eyes = detectEyes( gray, face );
		for ( auto && eye : eyes ) {
			auto mean = cv::mean( gray( eye ) );

			if ( mean > maxMean ) {
				maxMean = mean;
				time = lifetime;
				point = getCenterPoint( eye ); //face.tl() + 
				rect = eye;// +face.tl();
			}
		}
		answer.push_back( { face, eyes } );
	}
	if ( !answer.empty() ) {
		return rect;
	}

	return toRect( mat );
}