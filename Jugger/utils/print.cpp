#include <QDebug>
#include <QFontMetrics>
#include <QRegularExpression>
#include <QRegularExpressionMatch>
#include <QPainter>
#include <QLineF> // For rotatePoint
#include "print.h"
#include "utils.h"
#include "qt.h"
#include "imageviewer.h"

QMap<QUrl, HImageLoaderPtr>	Print::cache;

const cv::Point		Print::startPointDefault = { -10, 10 };
const cv::Scalar	Print::rectColorDefault = { 255, 177, 122 };
const cv::Scalar	Print::rectErrorColorDefault = { 0, 0, 255 };
const int			Print::rectOffsetDefault = 10;
const double		Print::rectOpacityDefault = 0.7;

char* const 		Print::fontNameDefault = "Georgia";
const cv::Scalar	Print::fontColorDefault = { 255, 255, 255 };
const int			Print::fontSizeDefault = 16;

const QSize			Print::maxImageSizeDefault = { 200, 200 };
const int			Print::emoteScaleDefault = 5;
const bool			Print::emoteDefault = false;

const cv::Rect2f	Print::rectModDefault = { 0.0F, 0.0F, 0.0F, 0.0F };

cv::Point	Print::startPoint = startPointDefault;
cv::Scalar	Print::rectColor = rectColorDefault;
cv::Scalar	Print::rectErrorColor = rectErrorColorDefault;
int			Print::rectOffset = rectOffsetDefault;
double		Print::rectOpacity = rectOpacityDefault;

char*		Print::fontName = fontNameDefault;
cv::Scalar	Print::fontColor = fontColorDefault;
int			Print::fontSize = fontSizeDefault;

QSize		Print::maxImageSize = maxImageSizeDefault;
int			Print::emoteScale = emoteScaleDefault;
bool		Print::emote = emoteDefault;

cv::Rect2f	Print::rectMod = rectModDefault;

void Print::setDefault() {
	startPoint = startPointDefault;
	rectColor = rectColorDefault;
	rectOffset = rectOffsetDefault;

	fontName = fontNameDefault;
	fontColor = fontColorDefault;
	fontSize = fontSizeDefault;

	maxImageSize = maxImageSizeDefault;
	emoteScale = emoteScaleDefault;
	emote = emoteDefault;

	rectMod = rectModDefault;
}

void	roundedRect( cv::Mat & mat, const cv::Rect & rect, const cv::Scalar & color, int offset, double opacity ) {
	QImage qimg{ mat.ptr(), mat.cols, mat.rows, mat.step, QImage::Format_RGB888 };

	QPainter qp{ &qimg };
	qp.setRenderHint( QPainter::Antialiasing, true );
	qp.setPen( toQColor( color ) );
	auto brushColor = color;
	brushColor[ 3 ] = opacity * 255;
	qp.setBrush( toQColor( brushColor ) );
	qp.drawRoundedRect( toQRect( rect ), offset, offset );
}

auto	rotatePoint( const cv::Point2d & point, const cv::Point2d & center, int angle ) -> cv::Point {
	auto normal = QLineF{ center.x, center.y, point.x, point.y };
	normal.setAngle( normal.angle() + angle );
	return cv::Point2d{ normal.x2(), normal.y2() };
}

void	circleRhomb( cv::Mat & mat, const cv::Size & size, const cv::Scalar & color, const cv::Point & center, double raduis, int angle = 0 ) {
	auto angleRadian = CV_PI * angle / 180;
	auto circlePoint = center + cv::Point( raduis * cv::Point2d{ 
		std::cos( angleRadian ), std::sin( angleRadian ) 
	} );
	auto centerRhomb = cv::Point2d{ size.width / 2.0, size.height / 4.0 };
	auto points = std::vector< cv::Point >{ 4, circlePoint - cv::Point( centerRhomb ) };
	points[ 0 ] += rotatePoint( { size.width / 2.0, 0.0 }, centerRhomb, -90 - angle );
	points[ 1 ] += rotatePoint( { size.width - 0.0, size.height / 4.0 }, centerRhomb, -90 - angle );
	points[ 2 ] += rotatePoint( { size.width / 2.0, size.height - 0.0 }, centerRhomb, -90 - angle );
	points[ 3 ] += rotatePoint( { 0.0, size.height / 4.0 }, centerRhomb, -90 - angle );
	for ( auto & point : points ) {
		point = clamp( point, mat );
	}
	cv::fillConvexPoly( mat, points, color, cv::LINE_AA );
}

auto	rectTr( const cv::Rect & rect ) -> cv::Point {
	return{ rect.br().x, rect.tl().y };
}

auto	rectBl( const cv::Rect & rect ) -> cv::Point {
	return{ rect.tl().x, rect.br().y };
}

auto	getEllipseRect( const cv::Rect & rect ) {
	return cv::fitEllipse( std::vector< cv::Point >{ 
		rect.tl(), rect.tl(), rect.br(), rectTr( rect ), rectBl( rect ) 
	} ).boundingRect();
}

auto	getAnimateValue( int animate, int size = 20 ) -> int {
	if ( animate > 0 ) {
		auto animateValue = animate % size;
		animateValue = animateValue > size / 2 ? size - animateValue : animateValue;
		animateValue -= size / 4;
		return animateValue;
	}
	return 0;
}

void	selectCircle( cv::Mat & mat, const cv::Rect & rect, const cv::Scalar & color, int animate ) {
	auto ellipseRect = getEllipseRect( rect );
	auto ellipseCenter = clamp( getCenterPoint( ellipseRect ), mat );
	auto animateValue = getAnimateValue( animate, 20 );
	auto dx = std::max( rect.width / 12, 6 ) + animateValue / 4;
	auto sizeValue = std::max( ellipseRect.width, ellipseRect.height ) / 2;
	auto linePoint1 = ellipseRect.tl() + cv::Point{ ellipseRect.width / 2, 0 };
	auto linePoint2 = ellipseRect.tl() + cv::Point{ ellipseRect.width / 2, dx };
	auto point = ellipseRect.tl() + cv::Point{ ellipseRect.width / 2, dx + 8 };

	cv::circle( mat, ellipseCenter, sizeValue - 2,
				color + cv::Scalar{ 111, 111, 111 }, 1, cv::LINE_AA );
	cv::circle( mat, ellipseCenter, sizeValue - dx, 
				color + cv::Scalar{ 111, 111, 111 }, 1, cv::LINE_AA );
	for ( auto i = 0; i < 4; ++i ) {
		if ( i & 1 ) { // 0, 2, 4, 6 etc
			cv::ellipse( mat, ellipseCenter, { sizeValue, sizeValue },
					 0, 90 * i - 30 + animateValue, 90 * i + 120 + animateValue,
					 color, 2, cv::LINE_AA );
		}
		cv::line( mat, clamp( rotatePoint( linePoint1, ellipseCenter, 90 * i ), mat ),
				  rotatePoint( linePoint2, ellipseCenter, 90 * i ), color, 2, cv::LINE_AA );
		cv::line( mat, clamp( rotatePoint( point, ellipseCenter, 90 * i ), mat ),
				  rotatePoint( point, ellipseCenter, 90 * i ), color, 2, cv::LINE_AA );
	}
}

void	selectRhomb( cv::Mat & mat, const cv::Rect & rect, const cv::Scalar & color, int animate ) {
	auto ellipseRect = getEllipseRect( rect );
	auto rhombSize = cv::Size{	clamp( rect.width / 25, 2, rect.width ), 
								clamp( rect.width / 10, 5, rect.width ) };

	auto animateValue = getAnimateValue( animate, 20 );
	for ( auto i = 0; i < 4; ++i ) { 
		circleRhomb( mat, rhombSize, color, getCenterPoint( ellipseRect ),
					 0.5 * std::max( ellipseRect.width, ellipseRect.height ), 
					 90 * i + 45 + animateValue );
	}
}

void	selectEllipse( cv::Mat & mat, const cv::Rect & rect, const cv::Scalar & color, int animate ) {
	auto ellipseRect = getEllipseRect( rect );
	auto centerEllipse = getCenterPoint( ellipseRect );
	auto sizeEllipse = 0.5 * ellipseRect.size();

	auto animateValue = getAnimateValue( animate, 20 );
	for ( auto i = 0; i < 4; ++i ) {
		cv::ellipse( mat, centerEllipse, sizeEllipse, 0, 
					 90 * i + 30 + animateValue, 
					 90 * i + 60 + animateValue, 
					 color, 2, cv::LINE_AA );
	}
}

void	selectLines( cv::Mat & mat, const cv::Rect & rect, const cv::Scalar & color, int animate ) {
	auto animateValue = getAnimateValue( animate, 20 );
	auto dx = rect.width / 16 + animateValue;
	auto dy = rect.height / 16 + animateValue;

	auto tr = rectTr( rect );
	auto bl = rectBl( rect );

	line( mat, rect.tl(), rect.tl() + cv::Point{ dx, 0 }, color, 2, cv::LINE_AA );
	line( mat, rect.tl(), rect.tl() + cv::Point{ 0, dy }, color, 2, cv::LINE_AA );
	line( mat, rect.br(), rect.br() - cv::Point{ dx, 0 }, color, 2, cv::LINE_AA );
	line( mat, rect.br(), rect.br() - cv::Point{ 0, dy }, color, 2, cv::LINE_AA );

	line( mat, tr, tr - cv::Point{ dx, 0 }, color, 2, cv::LINE_AA );
	line( mat, tr, tr + cv::Point{ 0, dy }, color, 2, cv::LINE_AA );
	line( mat, bl, bl + cv::Point{ dx, 0 }, color, 2, cv::LINE_AA );
	line( mat, bl, bl - cv::Point{ 0, dy }, color, 2, cv::LINE_AA );
}

void	selectRect( cv::Mat & mat, const cv::Rect & rect, const cv::Scalar & color ) {
	rectangle( mat, rect, color );
	mat( rect ) += color / 5;
}

void	select( cv::Mat & mat, const cv::Rect & rect, const cv::Scalar & color, int id ) {
	static auto animates = std::map< int, long long >{};
	auto animate = 0;
	if ( id > 0 ) {
		if ( animates.count( id ) > 0 && animates[ id ] < 1000 ) {
			animate = animates[ id ]++;
		}
		else {
			animates[ id ] = 0;
		}
	}

	selectLines( mat, rect, color, animate );
}

auto	operator+ ( const QRect &rect1, const QRect &rect2 ) -> QRect {
	return{ 0, 0, std::max( rect1.width(), rect2.width() ), rect1.height() + rect2.height() };
}

auto operator+= ( QRect & rect1, const QRect & rect2 ) -> QRect {
	rect1 = rect1 + rect2;
	return rect1;
}

auto Print::getTextRect( cv::Mat& in, QString text, int fontSize, const char* fontName ) -> QRect {
	if ( text == "" ) {
		return{ 0, 0, 0, 0 };
	}
	QFontMetrics metrics( { fontName, fontSize } );

	if ( in.empty() ) {
		return metrics.boundingRect( 
			{ { 0, 0 }, 
			metrics.size( Qt::TextWordWrap, text ) }, 
			Qt::TextWordWrap, text );
	}
	return metrics.boundingRect( 
		{ rectOffset, rectOffset, in.cols - 2 * rectOffset, in.rows - 2 * rectOffset },
		Qt::TextWordWrap, text );
}

auto Print::drawText( cv::Mat& mat, QString text, QPoint pos, int fontSize, cv::Scalar fontColor ) {
	QImage qimg{ mat.ptr(), mat.cols, mat.rows, mat.step, QImage::Format_RGB888 };
	// fontName external
	auto rect = getTextRect( mat, text, fontSize, fontName );

	QPainter painter{ &qimg };
	QFont font{ fontName, fontSize };
	painter.setRenderHint( QPainter::TextAntialiasing );
	painter.setPen( toQColor( fontColor ) );
	painter.setFont( font );

	rect.moveTopLeft( pos );

	painter.drawText( rect, Qt::TextWordWrap, text );
	painter.end();

	return rect.height();
}

auto Print::getImageRect( cv::Mat& in, QImage& image ) -> QRect {
	auto rect = image.rect();
	rect.setWidth( cv::min( rect.width(), maxImageSize.width() ) );
	rect.setHeight( cv::min( rect.height(), maxImageSize.height() ) );
	return rect;
}

auto Print::drawImage( cv::Mat& mat, QImage& image, QPoint pos ) {
	QImage qimg{ mat.ptr(), mat.cols, mat.rows, mat.step, QImage::Format_RGB888 };

	auto rect = getImageRect( mat, image );
	rect.moveTopLeft( pos );

	QPainter qp{ &qimg };
	qp.drawImage( rect, image.rgbSwapped() );

	return rect.height();
}

auto Print::getEmoteRect( int emoteScale ) -> QRect {
	return{ 0, 0, emoteScale * 20, emoteScale * 20 };
}

auto Print::drawEmote( cv::Mat& mat, QString emote, QPoint pos, int scale ) {
	bool isOpenEyes = emote[ 0 ] != '=';
	bool isWink = emote[ 0 ] == ';';
	double positive = emote[ 1 ] == ')' ? 1.0 : emote[ 1 ] == '(' ? -1.0 : 0.0;

	auto center = cv::Point{ pos.x() + scale * 10, pos.y() + scale * 10 };
	auto color = cv::Scalar::all( 0 );

	cv::circle( mat, center, scale * 10, { 150, 255, 255 }, cv::FILLED, cv::LINE_AA );

	if ( isOpenEyes ) {
		if ( isWink ) {
			cv::rectangle( mat, { center + scale * cv::Point{ -5, -3 }, cv::Size{ scale * 3, scale * 1 } }, color, cv::FILLED, cv::LINE_AA );
		}
		else {
			cv::circle( mat, center + scale * cv::Point{ -3, -3 }, scale, color, cv::FILLED, cv::LINE_AA );
		}
		cv::circle( mat, center + scale * cv::Point{ 3, -3 }, scale, color, cv::FILLED, cv::LINE_AA );
	}
	else {
		cv::rectangle( mat, { center + scale * cv::Point{ -5, -3 }, cv::Size{ scale * 3, scale * 1 } }, color, cv::FILLED, cv::LINE_AA );
		cv::rectangle( mat, { center + scale * cv::Point{ 3, -3 }, cv::Size{ scale * 3, scale * 1 } }, color, cv::FILLED, cv::LINE_AA );
	}

	auto isPositive = positive > 0.0 ? true : false;
	auto startAngle = isPositive ? 90 : 270;

	cv::ellipse( mat, center + scale * cv::Point{ 0, 5 + ( isPositive ? -5 : 5 ) },
	{ int( abs( positive ) * scale * 5 ), scale * 5 },
			 0, startAngle - 30, startAngle + 30, color, scale / 2, cv::LINE_AA );

	if ( positive > 0.8 ) {
		ellipse( mat, center,
		{ int( abs( positive ) * scale * 5 ), int( scale * 5.5 ) },
				 0, startAngle - 10, startAngle + 10, color, scale, cv::LINE_AA );
	}

	return getEmoteRect( scale ).height();
}

auto Print::drawPrintRect( cv::Mat & in, cv::Rect rect, cv::Scalar color, int offset ) {
	rect += cv::Size{ 2 * offset, 2 * offset };
	rect -= cv::Point{ offset, offset };

	if ( in.empty() ) {
		in = cv::Mat::zeros( rect.size(), CV_8UC3 );
	}

	rect.width = clamp( rect.width, 0, in.cols );
	rect.height = clamp( rect.height, 0, in.rows );
	rect.x = clamp( rect.x, 0, in.cols - rect.width );
	rect.y = clamp( rect.y, 0, in.rows - rect.height );

	roundedRect( in, rect, color, offset, rectOpacity );
}

auto Print::clearCache() {
	cache.clear();
}

auto Print::getSmile( QString text ) -> QStringList {
	//":)" "=(" ";|"s
	auto pattern = "(\\:|\\;|\\=)(\\)|\\(|\\|)*"_q;
	QRegularExpression expression{ pattern };
	auto match = expression.match( text );

	if ( match.hasMatch() ) {
		//qDebug() << match.capturedTexts();
		auto index = text.lastIndexOf( match.captured( 0 ) );
		QStringList texts;
		texts += text.left( index );
		texts += text.mid( index, 2 );
		texts += text.mid( index + 2 );

		return texts;
	}
	return{};
}

void Print::print( cv::Mat& in, const Args &data, cv::Point pos, int fontSize, const char * fontName, int rectOffset, const cv::Scalar &color, const bool emote, const cv::Rect2f & rectMod ) {
	cv::Rect printRect{ pos + cv::Point{ rectOffset, rectOffset }, cv::Size{ 0, 0 } };

	for ( auto item : data ) {
		QRect boundingRect;
		if ( item.type() == QVariant::String ) {
			auto text = item.toString();
			auto smile = getSmile( text );
			if ( emote && !smile.empty() ) {
				boundingRect += getTextRect( in, smile[ 0 ], fontSize, fontName );
				boundingRect += getEmoteRect( emoteScale );
				boundingRect += getTextRect( in, smile[ 2 ], fontSize, fontName );
			}
			else {
				boundingRect = getTextRect( in, text, fontSize, fontName );
			}
		}
		else if ( item.type() == QVariant::Image ) {
			boundingRect = getImageRect( in, item.value<QImage>() );
		}
		else if ( item.type() == QVariant::Url ) {
			auto url = item.toUrl();
			if ( !cache.contains( url ) ) {
				cache[ url ] = { new HImageLoader{ url }, &QObject::deleteLater };
			}
			auto image = cache[ url ]->getImage();
			boundingRect = getImageRect( in, image );
		}

		if ( boundingRect.width() > printRect.width ) {
			printRect.width = boundingRect.width();
		}
		printRect.height += boundingRect.height();
	}

	if ( !in.empty() ) {
		pos.x += pos.x < 0 ? in.cols - printRect.width - rectOffset : rectOffset;
		pos.y += pos.y < 0 ? in.rows - printRect.height - rectOffset : rectOffset;
		printRect.x = clamp( pos.x, rectOffset, in.cols - printRect.width - rectOffset );
		printRect.y = clamp( pos.y, rectOffset, in.rows - printRect.height - rectOffset );
	}
	else {
		printRect.x = rectOffset;
		printRect.y = rectOffset;
	}
	if ( printRect.height > 0 && printRect.width > 0 ) {
		drawPrintRect( in, printRect, rectColor, rectOffset );
	}

	auto y = 0;
	for ( auto item : data ) {
		auto pos = toQPoint( printRect.tl() + cv::Point{ 0, y } );
		if ( item.type() == QVariant::String ) {
			auto text = item.toString();
			auto smile = getSmile( text );
			if ( emote && !smile.empty() ) {
				y += drawText( in, smile[ 0 ], pos, fontSize, color );
				pos = toQPoint( printRect.tl() + cv::Point{ 0, y } );
				y += drawEmote( in, smile[ 1 ], pos, emoteScale );
				pos = toQPoint( printRect.tl() + cv::Point{ 0, y } );
				y += drawText( in, smile[ 2 ], pos, fontSize, color );
			}
			else {
				y += drawText( in, text, pos, fontSize, color );
			}
		}
		else if ( item.type() == QVariant::Image ) {
			y += drawImage( in, item.value<QImage>(), pos );
		}
		else if ( item.type() == QVariant::Url ) {
			auto url = item.toUrl();
			if ( cache.contains( url ) ) {
				auto image = cache[ url ]->getImage();
				y += drawImage( in, image, pos );
			}
		}
	}
}

Print::Print( cv::Mat & in, const Args & data, cv::Point pos, int fontSize, const char * fontName, int rectOffset, const cv::Scalar & color, const bool emote, const cv::Rect2f & rectMod ) {
	print( in, data, pos, fontSize, fontName, rectOffset, color, emote );
}

void Print::printError( cv::Mat & in, const Args & data, cv::Point pos, int fontSize, const char * fontName, int rectOffset, const cv::Scalar & color, const bool emote, const cv::Rect2f & rectMod ) {
	rectColor = rectErrorColor;
	//print( in, { u8"”гроза" }, { 10, 10 } );
	print( in, data, pos, fontSize, fontName, rectOffset, color, emote );
	rectColor = rectColorDefault;
}

void Print::print( cv::Mat & in, const Args & data, const Vars & vars ) {
	print( 
		in, data,
		vars.contains( "pos" ) ? toCvPoint( vars[ "pos" ].toPoint() ) : startPoint,
		vars.contains( "fontSize" ) ? vars[ "emote" ].toInt() : fontSize,
		vars.contains( "fontName" ) ? vars[ "fontName" ].toString().toStdString().c_str() : fontName,
		vars.contains( "rectOffset" ) ? vars[ "rectOffset" ].toInt() : rectOffset,
		vars.contains( "fontColor" ) ? toCvColor( vars[ "fontColor" ].value<QColor>() ) : fontColor,
		vars.contains( "emote" ) ? vars[ "emote" ].toBool() : emote,
		vars.contains( "rectMod" ) ? toCvRectF( vars[ "rectMod" ].toRectF() ) : rectMod
	);
}