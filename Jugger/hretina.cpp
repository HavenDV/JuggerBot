#include "hretina.h"


HRetina::HRetina() {
}

void	HRetina::update( const cv::Mat & mat ) {
	if ( mat.empty() ) {
		return;
	}

	if ( retina.empty() || mat.size() != retina->getInputSize() ) {
		retina = cv::bioinspired::createRetina( mat.size(), true );
		//retina = createRetina( in.size(), true, RETINA_COLOR_BAYER, true, 2.0, 10.0 );
		retina->setup( "RetinaSpecificParameters.xml" );
		//retina->setupOPLandIPLParvoChannel( false, true, 0.7F, 0.5F, 0.53F, 0.5F, 0.5F, 7.0F, 1.0F );
		//retina->setupIPLMagnoChannel( true, 1.0F, 0.5F, 7.0F, 3.0F, 3.0F );

		//retina->clearBuffers();
	}
	if ( retina.empty() ) {
		return;
	}

	retina->setupIPLMagnoChannel( false );
	retina->run( mat );
	retina->getParvo( parvo );
	retina->getMagno( magno );
}

auto	HRetina::getMagno( const cv::Mat & mat ) -> cv::Mat {
	update( mat );
	return magno;
}

auto	HRetina::getParvo( const cv::Mat & mat ) -> cv::Mat {
	update( mat );
	return parvo;
}
