#pragma once
#include <opencv2\opencv.hpp>

enum testSpeedType {
	CPU,
	GPU
};

class debugInfo {
	public:
		static void		printCudaInfo();
		static void		printOpenCLInfo();
		static void		printProcessorsInfo();
		static void		testSpeed( testSpeedType type );
};