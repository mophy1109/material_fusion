/*
 * @Author: USTB.mophy1109
 * @Date: 2018-04-02 11:13:34
 * @Last Modified by: USTB.mophy1109
 * @Last Modified time: 2018-04-04 18:31:09
 */

#ifndef CSTITCHING_H
#define CSTITCHING_H

#endif

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/xfeatures2d.hpp>

class CFuImage;
class CStitching {
	private:
	cv::Mat m_Img1;
	cv::Mat m_Img2;

	cv::Mat m_Result;
	cv::Rect rec_ROI;
	enum string {
		AVERAGE = 0,
		MODE = 1,
		// OTHERS....

	} CAL;

	public:
	int Stitching(cv::Mat img1, cv::Mat img2, cv::Mat result);
	std::vector<cv::DMatch> FindGoodMatches(std::vector<cv::DMatch> matches);
};