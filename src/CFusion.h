/*
 * @Author: USTB.mophy1109
 * @Date: 2018-04-02 11:12:04
 * @Last Modified by: USTB.mophy1109
 * @Last Modified time: 2018-04-02 16:18:27
 */

#ifndef CFUSION_H
#define CFUSION_H

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/xfeatures2d.hpp>
#include <string>
#include <vector>

class CFusion {
	private:
	std::vector<cv::Mat> images;

	public:
};
enum FusionMethod {
	LINEAR,
	MULTIBAND,

};
// void fusionImages(std::vector<char*> image_files);
void FusionImages(cv::Mat roi1, cv::Mat roi2, FusionMethod method);

cv::Mat StretchImage(cv::Mat region);
cv::Mat reconstruct(std::vector<cv::Mat> input_pyramid);
#endif