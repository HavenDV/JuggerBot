#include "debug.h"
#include "utils.h"
#include "gpu.h"
#include <QDebug>
/*
#include <opencv2\gpu\gpu.hpp>
#include <opencv2\ocl\ocl.hpp>

using namespace cv;

void	debugInfo::printOpenCLInfo() {
	ocl::PlatformsInfo platforms;
	ocl::getOpenCLPlatforms( platforms );
	for ( int i = 0; i < platforms.size(); i++ ) {
		qDebug()	<< QString::fromStdString(	platforms[i]->platformName )
					<< QString::fromStdString(	platforms[i]->platformVersion )
					<< "Devices: ";
		for ( int i = 0; i < platforms[i]->devices.size(); i++ ) {
			int type = platforms[i]->devices[i]->deviceType;
			qDebug()	<< QString::fromStdString(	platforms[i]->devices[i]->deviceName );
			qDebug()	<< "Type: " << (	type & ocl::CVCL_DEVICE_TYPE_GPU ?		"GPU" : 
											type & ocl::CVCL_DEVICE_TYPE_CPU ?		"CPU" : 
											type & ocl::CVCL_DEVICE_TYPE_DEFAULT ?	"DEFAULT" : 
																					"UNKNOWN" );
			qDebug()	<< "isUnifiedMemory: " << platforms[i]->devices[i]->isUnifiedMemory;
			qDebug()	<< "Memory size: " << platforms[i]->devices[i]->localMemorySize;
			qDebug()	<< "Threads: " << platforms[i]->devices[i]->maxComputeUnits;
		}
	}	
}

void	debugInfo::printCudaInfo() {
	for ( int i = 0; i < gpu::getCudaEnabledDeviceCount(); i++ ) {
		gpu::DeviceInfo info( i );
		qDebug() << "CUDA Device " << i;
		qDebug() << "Name: " << QString::fromStdString( info.name() );
		qDebug() << "Processor count: " << info.multiProcessorCount();
		qDebug() << "Version: " << info.majorVersion() << info.minorVersion();
		qDebug() << "Compatible: " << info.isCompatible();
		qDebug() << "Total memory: " << info.totalMemory();
		qDebug() << "Free memory: " << info.freeMemory();
		testSpeed( GPU );
		testSpeed( CPU );
	}
}

void	debugInfo::printProcessorsInfo() {
	qDebug()	<< "Number of CPU: " << getNumberOfCPUs();
	qDebug()	<< "Number of threads: " << getNumThreads();
	if ( useOptimized() )
		qDebug()	<< "Optimized code is enabled. Current optimizations: " << endl
					<< ( checkHardwareSupport(CV_CPU_MMX) ? "MMX " : " " )
					<< ( checkHardwareSupport(CV_CPU_SSE) ? "SSE " : " " )
					<< ( checkHardwareSupport(CV_CPU_SSE2) ? "SSE2 " : " " )
					<< ( checkHardwareSupport(CV_CPU_SSE3) ? "SSE3 " : " " )
					<< ( checkHardwareSupport(CV_CPU_SSSE3) ? "SSSE3 " : " " )
					<< ( checkHardwareSupport(CV_CPU_SSE4_1) ? "SSE4_1 " : " " )
					<< ( checkHardwareSupport(CV_CPU_SSE4_2) ? "SSE4_2 " : " " )
					<< ( checkHardwareSupport(CV_CPU_POPCNT) ? "POPCNT " : " " )
					<< ( checkHardwareSupport(CV_CPU_AVX) ? "AVX " : " " )
					<< ( checkHardwareSupport(CV_CPU_AVX2) ? "AVX2 " : " " );
}

void	debugInfo::testSpeed( testSpeedType type ) {
	Mat in = Mat( 6000, 6000, CV_8U, Scalar::all( 200  ) ), out;	
	double t = static_cast<double>( getTickCount() );
	if (type == CPU) {
		out = filter( in );
	}
	else {
		out = filterGPU( in );
	}
	t = 1000.0 * ( static_cast<double>( getTickCount() ) - t ) / getTickFrequency(); //ms
	qDebug()	<< "Time " << ( type == CPU ? "CPU" : "GPU" ) << "(ms): " << t;
}
*/