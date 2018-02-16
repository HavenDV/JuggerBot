#pragma once
#include <opencv2\opencv.hpp>
#include "hthreadparent.h"

enum State {
	Normal = 0,
	Fight
};

class Bot: public HThreadParent {
	Q_OBJECT

public:
	Bot();

	void	init();
	void	prepare();
	void	update();

	bool	check( const std::string & name, double k = 0.001 );
	void	click( const cv::Rect & rect );
	bool	click( const std::string & name, double k = 0.001 );
	bool	startBattle( const std::string & name, double k = 0.001 );
	void	defenceMode();
	void	repeat();
	void	create();
	void	analyse() noexcept;
	void	getScreenshot( const QImage & image );

private:
	
	cv::Mat						screenshot;
	State						state;

signals:
	void	kill();

};
