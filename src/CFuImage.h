/*
 * @Author: USTB.mophy1109
 * @Date: 2018-04-02 11:48:43
 * @Last Modified by: USTB.mophy1109
 * @Last Modified time: 2018-04-02 11:54:15
 */
#ifndef CFUIMAGE_H
#define CFUIMAGE_H
#endif

#include "cv.h"
class CFuImage {
	private:
	cv::Mat m_Img;
	cv::Mat m_Roi;
	int i_Width, i_Height; //大小
	int i_Devx, i_Devy;	//全局偏移
	float descriptors;
	vector<SiftGPU::SiftKeypoint> key;

	public:
	CFuImage(cv::Mat img);
	CFuImage(cv::Mat img, int width, int height);
	bool SetROI(cv::Mat roi);
	bool SetImageSize(int width, int height);
};