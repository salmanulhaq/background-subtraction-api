/*
	Author: Salman Ul Haq <salman@programmerfish.com>
*/

#include "subtract.h"
#include <chrono>

using namespace std;
using namespace cv;

#define DEBUG

#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

size_t curlCallback(char* buffer, size_t size, size_t nmemb, void* input) {
    for (unsigned int c = 0; c<(unsigned int)(size*nmemb); c++) {
        ((string*)input)->push_back(buffer[c]);
    }
    return size*nmemb;
}

BGSubError downloadImage(const char* url, double& contentLength, cv::Mat& image) {
  
  #ifdef DEBUG 
    cout<<"downloadImage() starting download: "<<url<<endl; 
  #endif

  CURL* curl;
  curl = curl_easy_init();

  string downloadedData;
  curl_easy_setopt(curl, CURLOPT_URL, url);
  curl_easy_setopt(curl, CURLOPT_USERAGENT, "Test BackgroundSubtractionCode image fetch");
  curl_easy_setopt(curl, CURLOPT_HTTPGET, 1L);
  curl_easy_setopt(curl, CURLOPT_MAXCONNECTS, 2L);
  curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, &curlCallback);
  curl_easy_setopt(curl, CURLOPT_WRITEDATA, &downloadedData);
  curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0);
  curl_easy_setopt(curl, CURLOPT_CONNECTTIMEOUT, 30L);
  curl_easy_setopt(curl, CURLOPT_TIMEOUT, 300L);
  curl_easy_setopt(curl, CURLOPT_DNS_USE_GLOBAL_CACHE, 1);
  curl_easy_setopt(curl, CURLOPT_DNS_CACHE_TIMEOUT, 3600L);
  curl_easy_setopt(curl, CURLOPT_PROTOCOLS, CURLPROTO_HTTP | CURLPROTO_HTTPS);
  curl_easy_setopt(curl, CURLOPT_REDIR_PROTOCOLS, CURLPROTO_HTTP | CURLPROTO_HTTPS);
  curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
  curl_easy_setopt(curl, CURLOPT_MAXREDIRS, 3L);
  curl_easy_setopt(curl, CURLOPT_FORBID_REUSE, 1L);
  curl_easy_setopt(curl, CURLOPT_FRESH_CONNECT, 1L);

  #ifdef DEBUG 
    curl_easy_setopt(curl, CURLOPT_VERBOSE, 1L); 
  #endif

  CURLcode response = curl_easy_perform(curl);
  
  #ifdef DEBUG 
    cout<<"downloadImage() performed curl_easy_perform()"<<endl; 
  #endif

  curl_easy_cleanup(curl);

  #ifdef DEBUG 
    cout<<"downloadImage() called curl_global_cleanup()"<<endl; 
  #endif

  if(response == CURLE_OK) {
    if(contentLength < 0) contentLength = downloadedData.size();
    cv::Mat *mbuf = new cv::Mat(downloadedData.size(), 1, CV_64FC1, (void*)downloadedData.c_str());
    try {
      #ifdef DEBUG 
        cout<<"downloadImage() calling imdecode()"<<endl; 
      #endif
      image = cv::imdecode(*mbuf, -1).clone();
      delete mbuf;
      downloadedData.clear();
      if(image.empty() || image.cols <= 0 || image.rows <=0) {
        #ifdef DEBUG 
          cout<<"downloadImage() image.empty() true"<<endl; 
        #endif
        return IMAGE_DOWNLOAD_FAILED;
      }
    } 
    catch(cv::Exception& ex) {
        #ifdef DEBUG 
          cout<<"downloadImage() OpenCV exception while decoding image: "<<ex.what()<<endl; 
        #endif
      	return IMAGE_DOWNLOAD_FAILED;
    }

    return NO_ERROR;
  } 
  
  #ifdef DEBUG 
    cout<<"ENGINE: downloadImage() CURL response not OK"<<endl; 
  #endif
  return IMAGE_DOWNLOAD_FAILED;
}

BGSubError subtract(string url, double& contentLength, vector<uchar>& outputEncoded) {

	try{

		double latency = 0.0;
		auto t0 = chrono::high_resolution_clock::now();
		
		Mat input;

		downloadImage(url.c_str(), contentLength, input);

		cout<<"downloaded image: "<<input.size()<<endl;

		Mat mask, fg, bg;
		mask = Scalar(0);

		if(input.empty()) {
			return OPENCV_ERROR;
		}

		if(input.channels() == 4) {
			cvtColor(input, input, CV_BGRA2BGR);
		}

		grabCut(input, mask, Rect(10,10,input.cols-10,input.rows-10), bg, fg, 0, GC_INIT_WITH_RECT);
		grabCut(input, mask, Rect(), bg, fg, 8, GC_EVAL);
		compare(mask, GC_PR_FGD, mask, CMP_EQ);

		int dilation_size = 1;
		Mat element = getStructuringElement(MORPH_ELLIPSE, Size(2*dilation_size+1, 2*dilation_size+1), Point(dilation_size, dilation_size));
	  	dilate(mask, mask, element);

	  	Mat output;
		input.copyTo(output, mask);

		cvtColor(output, output, CV_BGR2BGRA);

		vector<Mat> channels;
		split(output, channels);
		channels[3] = mask;
		merge(channels, output);

		vector<int> encodingParams;
		encodingParams.push_back(cv::IMWRITE_PNG_COMPRESSION);
    	encodingParams.push_back(1);

		imencode(".png", output, outputEncoded, encodingParams);

		auto t1 = chrono::high_resolution_clock::now();

		latency = chrono::duration_cast<chrono::milliseconds>(t1-t0).count();
		
		return SUCCESS;

	} catch(cv::Exception& ex) {
		return OPENCV_ERROR;
	}
}