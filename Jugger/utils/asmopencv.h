#pragma once
/*
   Functions to convert between OpenCV's cv::Mat and Qt's QImage and QPixmap.

   Andy Maloney
   23 November 2013
   http://asmaloney.com/2013/11/code/converting-between-cvmat-and-qimage-or-qpixmap
 */

#include <QImage>
#include <QPixmap>
#include <QDebug>

#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/imgproc/types_c.h>

namespace ASM {

	inline QImage cvMatToQImage( const cv::Mat &mat ) {
		if ( mat.empty() ) {
			return{};
		}
		switch ( mat.type() ) {
			// 8-bit, 4 channel
			case CV_8UC4:
				return{ mat.data, mat.cols, mat.rows, mat.step, QImage::Format_RGB32 };

				// 8-bit, 3 channel
			case CV_8UC3:
			{
				QImage image{ mat.data, mat.cols, mat.rows, mat.step, QImage::Format_RGB888 };
				return image.rgbSwapped();
			}
			// 8-bit, 1 channel
			case CV_8UC1:
			{
				//static QVector<QRgb>  sColorTable;
				// only create our color table once
				//if (sColorTable.isEmpty()) {
				//   for (int i = 0; i < 256; ++i)
				//      sColorTable.push_back(qRgb(i, i, i));
				//}
				//QImage image(mat.data, mat.cols, mat.rows, mat.step, QImage::Format_Indexed8);
				//image.setColorTable(sColorTable);
				//return image;
				return{ mat.data, mat.cols, mat.rows, mat.step, QImage::Format_Grayscale8 };
			}

			default:
				qWarning() << "ASM::cvMatToQImage() - cv::Mat image type not handled in switch:" << mat.type();
				break;
		}
		return{};
	}

	inline QPixmap cvMatToQPixmap( const cv::Mat &mat ) {
		return QPixmap::fromImage( cvMatToQImage( mat ) );
	}

	inline cv::Mat QImageToCvMat( const QImage &inImage, bool inCloneImageData = true ) {
		if ( inImage.isNull() ) {
			return{};
		}
		switch ( inImage.format() ) {
			// 8-bit, 4 channel
			case QImage::Format_RGB32:
			{
				cv::Mat  mat{ inImage.height(), inImage.width(), CV_8UC4, const_cast< uchar* >( inImage.bits() ), static_cast< size_t >( inImage.bytesPerLine() ) };
				cv::Mat	 out;
				cvtColor( mat, out, CV_BGRA2BGR );
				return ( inCloneImageData ? out.clone() :
						 out );
			}
			// 8-bit, 3 channel
			case QImage::Format_RGB888:
			{
				if ( !inCloneImageData ) {
					qWarning() << "ASM::QImageToCvMat() - Conversion requires cloning since we use a temporary QImage";
				}
				auto swapped = inImage.rgbSwapped();
				cv::Mat mat{ swapped.height(), swapped.width(), CV_8UC3, const_cast< uchar* >( swapped.bits() ), static_cast< size_t >( swapped.bytesPerLine() ) };
				return ( inCloneImageData ? mat.clone() :
						 mat );
			}
			// 8-bit, 1 channel
			case QImage::Format_Grayscale8:
			{
				cv::Mat mat{ inImage.height(), inImage.width(), CV_8UC1, const_cast< uchar* >( inImage.bits() ), static_cast< size_t >( inImage.bytesPerLine() ) };
				return ( inCloneImageData ? mat.clone() :
						 mat );
			}

			default:
			{
				qWarning() << "ASM::QImageToCvMat() - QImage format not handled in switch:" << inImage.format();
				break;
			}
		}
		return{};
	}

	inline cv::Mat QPixmapToCvMat( const QPixmap &inPixmap, bool inCloneImageData = true ) {
		return QImageToCvMat( inPixmap.toImage(), inCloneImageData );
	}

}