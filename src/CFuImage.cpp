/*
 * @Author: USTB.mophy1109
 * @Date: 2018-04-02 11:48:41
 * @Last Modified by: USTB.mophy1109
 * @Last Modified time: 2018-04-02 16:17:37
 */
#include "cv.h"

CFuImage::CFuImage(cv::Mat img) {
	this->img = img;
}
CFuImage::CFuImage(cv::Mat img, int width, int height) {
	this->img = img;
}

bool CFuImage::SetROI(cv::Mat roi) {
	this->roi = roi;
}

bool CFuImage::SetImageSize(int width, int height) {
	this->i_Width = width;
	this->i_Height = height;
}
