/*
 * @Author: USTB.mophy1109
 * @Date: 2018-04-02 11:48:41
 * @Last Modified by: USTB.mophy1109
 * @Last Modified time: 2018-04-02 16:17:37
 */

#include "CFuImage.h"
#include "SiftGPU.h"
#include <GL/gl.h>
#include <iostream>

using namespace std;
using namespace cv;

CFuImage::CFuImage(cv::Mat img) {
	m_Img = img;
	SiftGPU sift;
	sift.ParseParam(4, myargv);
	i_Width = img.cols;
	i_Height = img.rows;
	//运行sift算法
	sift.RunSIFT(i_Width, i_Height, img.data, GL_LUMINANCE, GL_UNSIGNED_BYTE);
	// cout << "runsift() cost time=" << timer.elapsed() << endl;

	//获取关键点和描述子
	FeatureNum = sift.GetFeatureNum();
	cout << "Num of features :" << FeatureNum << endl;

	descriptor = new float[128 * FeatureNum];
	key = vector<SiftGPU::SiftKeypoint>(FeatureNum);
	sift.GetFeatureVector(&key[0], &descriptor[0]);
}
CFuImage::CFuImage(cv::Mat img, int width, int height) {
	this->m_Img = img;
}

bool CFuImage::SetROI(cv::Mat roi) {
	this->m_Roi = roi;
}

bool CFuImage::SetImageSize(int width, int height) {
	this->i_Width = width;
	this->i_Height = height;
}
