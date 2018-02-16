#include <QTimer>
#include "utils/imageconverter.h"
#include "utils/utils.h"
#include "utils/qt.h"
#include "emulate/hmouse.h"
#include "bot.h"

constexpr auto WIDTH = 1920;		//Ширина
constexpr auto HEIGHT = 1080;		//Высота

Bot::Bot() : state( State::Normal ) {}

void	Bot::prepare() {
	HThreadParent::prepare();
	cv::destroyAllWindows();
}

void Bot::getScreenshot( const QImage & image ) {
	screenshot = ImageConverter{ image };
}

void Bot::init() {
	QTimer::singleShot( 1, this, &Bot::update );
}

void Bot::update() {
	if ( isPrepared() || isStopped() ) {
		return;
	}

	analyse();

	QTimer::singleShot( 1000, this, &Bot::update );
}

std::string path = "D:\\jugger\\";
auto	getImage( const std::string & name )
-> cv::Mat {
	cv::VideoCapture capture( path + name );
	cv::Mat mat;
	capture >> mat;
	return mat;
}

bool	find( const cv::Mat & mat, const cv::Mat & templ, cv::Rect & outRect, double k = 0.001 ) {
	if ( mat.empty() || templ.empty() ) {
		false;
	}

	cv::Mat result;
	cv::matchTemplate( mat, templ, result, cv::TM_SQDIFF_NORMED );
	cv::Point point;
	cv::minMaxLoc( result, 0, 0, &point );
	auto rect = cv::Rect{ point, templ.size() };
	auto dif = cv::abs( mat( rect ) - templ );
	auto mean = cv::mean( dif );
	auto sum = ( mean[ 0 ] + mean[ 1 ] + mean[ 2 ] ) / 3;
	auto procent = sum / 255;
	if ( procent > k ) {
		return false;
	}

	outRect = rect;
	return true;
}

bool	find( const cv::Mat & mat, const std::string & name, cv::Rect & outRect, double k = 0.001 ) {
	return find( mat, getImage( name ), outRect, k );
}

bool	Bot::check( const std::string & name, double k ) {
	cv::Rect rect;
	return find( screenshot, name, rect, k );
}

void	Bot::click( const cv::Rect & rect ) {
	HMouse::leftClick( toQPoint( getCenterPoint( rect ) ) );
}

bool	Bot::click( const std::string & name, double k ) {
	cv::Rect rect;
	if ( find( screenshot, name, rect, k ) ) {
		click( rect );
		return true;
	}

	return false;
}

bool	Bot::startBattle( const std::string & name, double k ) {
	if ( click( "battle.bmp" ) ) {
		return true;
	}

	click( name, k );
	return false;
}

#include <iostream>
void	console( const std::string & string ) {
	std::cout << string << std::endl;
}

void	Bot::defenceMode() {
	static size_t count = 0;

	if ( click( "return.bmp" ) ) {
		console( "Returned" );
		return;
	}

	if ( check( "very_low_hp.bmp" ) || check( "low_hp.bmp" ) ) { //     
		//console( "Need heal: lowHP or veryLowHP" );
		if ( check( "fight/is_fight.bmp" ) ) {
			//console( "Fight Heal" );
			if ( click( "fight/heal.bmp" ) ) {
				console( "Fight Heal is used" );
				return;
			}
		}
		else {
			//console( "No Fight Heal" );
			//if ( click( "hleb.jpg" ) ) { //pirozhok
				//console( "Low HP: Eat is used" );
				//return;
			//}
		}
	}

	if ( check( "fight/is_kazn.bmp" ) && click( "fight/yarost.bmp" ) ) {
		console( "Kazn: Yarost is used" );
		return;
	}

	     if ( click( "fight/cast_active.bmp" ) ) {}
	else if ( click( "fight/cast.bmp" ) ) {}
	else if ( click( "fight/direct_active.bmp" ) ) {
		click( "fight/orb.bmp" );
	}
	else if ( click( "fight/left_active.jpg", 0.05 ) ) {
		click( "fight/orb.bmp" );
	}
	else if ( click( "fight/right_active.bmp" ) ) {
		click( "fight/orb.bmp" );
	}
	else if ( click( "fight/exit.jpg", 0.05 ) ) {
		count++;
		console( "Killed: " + std::to_string( count ) );
	}
	else if ( check( "full_hp.bmp" ) && 
			  startBattle( "gibl1.bmp", 0.2 ) && 
			  startBattle( "gibl2.bmp", 0.2 ) &&
			  startBattle( "gibl3.bmp", 0.2 ) &&
			  startBattle( "gibl4.bmp", 0.2 ) ) {// click( "kratch.bmp" )
		//console( "Mob finded" ); quest.bmp
	}
	
}

void	Bot::repeat() {
	auto temp = HMouse::getPosition();
	if ( click( "repeat.bmp", 0.01 ) ) {
		HMouse::move( temp );
	}
}

void	Bot::create() {
	auto temp = HMouse::getPosition();
	if ( click( "create.bmp", 0.01 ) ) {
		HMouse::move( temp );
	}
}

void Bot::analyse() noexcept {
	try {
		if ( screenshot.empty() ) {
			return;
		}
		//imshow_s( "screenshot", screenshot );
		repeat();
		create();
		defenceMode();
	}
	catch ( const cv::Exception & e ) {
		qDebug() << e.what();
	}
}