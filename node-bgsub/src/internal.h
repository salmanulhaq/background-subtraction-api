#include "subtract.h"

#ifndef INTERNAL_H
#define INTERNAL_H

#include <string>

enum BGSubError {
	SUCCESS = 0,
	NO_ERROR = SUCCESS,
	ERROR = -1,
	OPENCV_ERROR = -2,
	IMAGE_DOWNLOAD_FAILED = -3
};

#endif