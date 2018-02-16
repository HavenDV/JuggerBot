#pragma once
#include <QVariant>
#include <QSharedPointer>
#include <opencv2\opencv.hpp>
#include "loaders\himageloader.h"

typedef const std::initializer_list<QVariant> Args;
typedef const QMap<QString, QVariant> Vars;
typedef QSharedPointer<HImageLoader> HImageLoaderPtr;

void	roundedRect( cv::Mat & mat, const cv::Rect & rect, const cv::Scalar & color, int offset = 10, double opacity = 1.0 );

void	selectCircle( cv::Mat & mat, const cv::Rect & rect, const cv::Scalar & color, int animate = 0 );
void	selectRhomb( cv::Mat & mat, const cv::Rect & rect, const cv::Scalar & color, int animate = 0 );
void	selectEllipse( cv::Mat & mat, const cv::Rect & rect, const cv::Scalar & color, int animate = 0 );
void	selectLines( cv::Mat & mat, const cv::Rect & rect, const cv::Scalar & color, int animate = 0 );
void	selectRect( cv::Mat & mat, const cv::Rect & rect, const cv::Scalar & color );
void	select( cv::Mat & mat, const cv::Rect & rect, const cv::Scalar & color, int id = -1 );

class Print {

public:
	static const cv::Point		startPointDefault;
	static const cv::Scalar		rectColorDefault;
	static const cv::Scalar		rectErrorColorDefault;
	static const int			rectOffsetDefault;
	static const double			rectOpacityDefault;

	static char* const 			fontNameDefault;
	static const cv::Scalar		fontColorDefault;
	static const int			fontSizeDefault;

	static const QSize			maxImageSizeDefault;
	static const int			emoteScaleDefault;
	static const bool			emoteDefault;

	static const cv::Rect2f		rectModDefault;

	static cv::Point	startPoint;
	static cv::Scalar	rectColor;
	static cv::Scalar	rectErrorColor;
	static int			rectOffset;
	static double		rectOpacity;

	static char*		fontName;
	static cv::Scalar	fontColor;
	static int			fontSize;

	static QSize		maxImageSize;
	static int			emoteScale;
	static bool			emote;

	static cv::Rect2f	rectMod;

	static QMap<QUrl, HImageLoaderPtr> cache;

	static auto setDefault() -> void;

	static auto getTextRect( cv::Mat & in, QString text, int size, const char * fontName )->QRect;
	static auto getImageRect( cv::Mat & in, QImage & image )->QRect;
	static auto getEmoteRect( int emoteScale = Print::emoteScale )->QRect;

	static auto drawPrintRect( cv::Mat & in, cv::Rect rect, cv::Scalar color, int offset );
	static auto drawText( cv::Mat & mat, QString text, QPoint pos, int size, cv::Scalar color );
	static auto drawImage( cv::Mat & mat, QImage & image, QPoint pos );
	static auto drawEmote( cv::Mat & mat, QString emote, QPoint pos, int emoteScale = Print::emoteScale );

	static auto clearCache();
	static auto getSmile( QString text ) -> QStringList;
	static void print( cv::Mat& in, const Args &data, cv::Point startPoint = Print::startPoint, int fontSize = Print::fontSize, const char * fontName = Print::fontName, int rectOffset = Print::rectOffset, const cv::Scalar &fontColor = Print::fontColor, const bool emote = false, const cv::Rect2f & rectMod = Print::rectMod );
	static void printError( cv::Mat& in, const Args &data, cv::Point startPoint = Print::startPoint, int fontSize = Print::fontSize, const char * fontName = Print::fontName, int rectOffset = Print::rectOffset, const cv::Scalar &fontColor = Print::fontColor, const bool emote = false, const cv::Rect2f & rectMod = Print::rectMod );

	static void print( cv::Mat & in, const Args & data, const Vars & vars );

	Print( cv::Mat & in, const Args & data, cv::Point startPoint = Print::startPoint, int fontSize = Print::fontSize, const char * fontName = Print::fontName, int rectOffset = Print::rectOffset, const cv::Scalar &fontColor = Print::fontColor, const bool emote = false, const cv::Rect2f & rectMod = Print::rectMod );
};