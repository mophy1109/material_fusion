/*
 * @Author: USTB.mophy1109
 * @Date: 2018-04-02 11:48:41
 * @Last Modified by:   USTB.mophy1109
 * @Last Modified time: 2018-04-23 17:26:19
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
	// cout << "Num of features :" << FeatureNum << endl;

	descriptor = new float[128 * FeatureNum];
	// cout << &descriptor << endl;
	key = vector<SiftGPU::SiftKeypoint>(FeatureNum);
	sift.GetFeatureVector(&key[0], &descriptor[0]);
}

CFuImage::CFuImage(cv::Mat img, int width, int height) {
	this->m_Img = img;
}

CFuImage::~CFuImage() {
	delete[] descriptor;
}

bool CFuImage::SetROI(cv::Mat roi) {
	this->m_Roi = roi;
}

bool CFuImage::SetImageSize(int width, int height) {
	this->i_Width = width;
	this->i_Height = height;
}

void CFuImage::Clone(const CFuImage &image) {
	// mem leak happens when calling : CFuImage A = B, because descriptor is a ptr.
	//直接拷贝时会出现内存泄漏，descriptor需要手动释放
	key.clear();
	delete[] descriptor;
	m_Img.release();

	m_Img = image.m_Img.clone();
	FeatureNum = image.FeatureNum;
	i_Width = m_Img.cols;
	i_Height = m_Img.rows;
	descriptor = (float *)calloc(128 * FeatureNum, sizeof(float));
	memcpy(descriptor, image.descriptor, 128 * FeatureNum * sizeof(float));
	key = image.key; // vector内数据是结构体，默认是深拷贝
}