/*
 * @Author: USTB.mophy1109
 * @Date: 2018-04-02 11:48:43
 * @Last Modified by:   USTB.mophy1109
 * @Last Modified time: 2018-04-23 17:25:42
 */

#ifndef CFUIMAGE_H
#define CFUIMAGE_H

#include "SiftGPU.h"
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/xfeatures2d.hpp>

class CFuImage {
	private:
	cv::Mat m_Roi;
	int i_Width, i_Height; //大小
	int i_Devx, i_Devy;	//全局偏移

	public:
	cv::Mat m_Img;
	int FeatureNum;
	float *descriptor;
	std::vector<SiftGPU::SiftKeypoint> key;
	char *myargv[4] = { "-cuda", "0", "-v", "0" }; // siftGPU 默认参数

	CFuImage();
	~CFuImage();
	CFuImage(cv::Mat img);
	CFuImage(cv::Mat img, int width, int height);
	void Clone(const CFuImage &cfuimage);
	bool SetROI(cv::Mat roi);
	bool SetImageSize(int width, int height);
};

#endif