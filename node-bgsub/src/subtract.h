#ifndef SUBTRACT_H
#define SUBTRACT_H

#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/objdetect/objdetect.hpp>
#include <opencv2/video/background_segm.hpp>

#include <curl/curl.h>

#include "internal.h"

#include <iostream>
#include <vector>
#include <string>

BGSubError subtract(std::string url, double& contentLength, std::vector<uchar>& outputEncoded);

#endif
