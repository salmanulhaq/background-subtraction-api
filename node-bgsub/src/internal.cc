#include "internal.h"
#include <string>

using namespace std;
using namespace cv;

string getErrorString(enum BGSubError error) {
	switch(error) {
		case SUCCESS:
			return "SUCCESS";
			break;
		case ERROR:
			return "ERROR";
			break;
		case OPENCV_ERROR:
			return "OPENCV_ERROR";
			break;
		case IMAGE_DOWNLOAD_FAILED:
			return "IMAGE_DOWNLOAD_FAILED";
			break;
		default:
			return "GENERAL ERROR";
			break;
	}
}