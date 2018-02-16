#include "hkalmanfilter.h"

HKalmanFilter::HKalmanFilter() : filter{ 4, 2, 0, cv::DataType< double >::type } {
	filter.transitionMatrix *= ( cv::Mat_< double >( 4, 4 ) << 1, 0, 1, 0, 0, 1, 0, 1, 0, 0, 1, 0, 0, 0, 0, 1 );
	filter.statePre = 0.0;
	cv::setIdentity( filter.measurementMatrix ); // Инициализация матрицы измерений
	cv::setIdentity( filter.processNoiseCov, cv::Scalar::all( 1e-4 ) ); // Значение ковариации шума процесса
	cv::setIdentity( filter.measurementNoiseCov, cv::Scalar::all( 3 ) ); // Значение ковариации шума измерения
	cv::setIdentity( filter.errorCovPost, cv::Scalar::all( 0.1 ) ); // Обновление ошибки ковариации
}

auto HKalmanFilter::correct( const cv::Point2d & point ) -> cv::Point2d {
	auto predicted = filter.predict();
	auto predictedPoint = cv::Point2d{ predicted.at< double >( 0 ), predicted.at< double >( 1 ) };
	auto estimated = filter.correct( ( cv::Mat_< double >{ 2, 1 } << point.x, point.y ) );
	auto correctedPoint = cv::Point2d{ estimated.at< double >( 0 ), estimated.at< double >( 1 ) };

	return correctedPoint;
}
