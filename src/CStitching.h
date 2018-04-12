/*
 * @Author: USTB.mophy1109
 * @Date: 2018-04-02 11:13:34
 * @Last Modified by: USTB.mophy1109
 * @Last Modified time: 2018-04-08 16:34:45
 */

#ifndef CSTITCHING_H
#define CSTITCHING_H

#endif

#include "CUtil.h"
#include "SiftGPU.h"
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/xfeatures2d.hpp>
#include <vector>

class CFuImage;
class CStitching {
	private:
	cv::Mat m_Img1;
	cv::Mat m_Img2;

	cv::Mat m_Result;
	cv::Mat roi1;
	cv::Mat roi2;
	cv::Rect rec_ROI;
	Offset g_offset = { 0, 0 }; //初始化偏移量0, 0

	public:
	CalMethod CalOffsetMethod = AFFINE; //默认使用放射矩阵求偏移量
	int Stitching(cv::Mat img1, cv::Mat img2, cv::Mat &result);
	Offset CalOffset(std::vector<cv::Point> train_point, std::vector<cv::Point> query_point, CalMethod method);
	std::vector<cv::Mat> CalROI(const cv::Mat &img1, const cv::Mat &img2);
};