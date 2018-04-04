/*
 * @Author: USTB.mophy1109
 * @Date: 2018-04-02 11:12:04
 * @Last Modified by: USTB.mophy1109
 * @Last Modified time: 2018-04-02 16:18:27
 */

#ifndef CFUSION_H
#define CFUSION_H

#endif

#include <opencv2/core/core.hpp>
#include <string>
#include <vector>

class CFusion {
	private:
	std::vector<cv::Mat> images;

	public:
	void FusionImages(std::string dir);
	std::vector<std::string> GetAllFiles(std::string path, std::string suffix);
};
// void fusionImages(std::vector<char*> image_files);