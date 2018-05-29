/*
 * @Author: USTB.mophy1109
 * @Date: 2018-04-02 11:13:34
 * @Last Modified by: USTB.mophy1109
 * @Last Modified time: 2018-05-25 11:43:51
 */

#ifndef CSTITCHING_H
#define CSTITCHING_H

#include "CFuImage.h"
#include "CUtil.h"
#include "SiftGPU.h"
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/xfeatures2d.hpp>
#include <vector>

class CStitching {
	private:
	cv::Mat m_Img1;
	cv::Mat m_Img2;

	cv::Mat m_Result;
	cv::Rect rec_ROI;
	Offset g_offset = { 0, 0 }; // init offset to (0, 0)

	public:
	CalMethod CalOffsetMethod = AFFINE; // default: use Affine Matrix to calculate offset;
	int Stitching(CFuImage &img1, CFuImage &img2, cv::Mat &result);
	Offset CalOffset(std::vector<cv::Point> train_point, std::vector<cv::Point> query_point, CalMethod method);
	std::vector<cv::Mat> CalROI(cv::Mat &img1, cv::Mat img2);
};
#endif