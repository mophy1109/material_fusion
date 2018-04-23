/*
 * @Author: USTB.mophy1109
 * @Date: 2018-03-30 12:49:23
 * @Last Modified by: USTB.mophy1109
 * @Last Modified time: 2018-04-04 17:45:10
 */

#include "CFusion.h"
#include <SiftGPU.h>
#include <dirent.h>
#include <iostream>
#include <math.h>
#include <regex>
#include <string>

using namespace std;
using namespace cv;

void FusionImages(cv::Mat roi1, cv::Mat roi2, FusionMethod method) {
	switch (method) {
		case LINEAR: {
			break;
		}
		case MULTIBAND: {
		}
	}
}

Mat StretchImage(Mat region) {
	// region is a (0,1) img, to make it more clear, set pixel-value "1" as 255 (white) for visual demonstration.
	Mat out = region.clone();
	out = out * 255;
	return out;
}

// got some problem
Mat reconstruct(vector<Mat_<float>> input_pyramid) {
	// reconstruct Laplacian pyramid
	Mat currentImg = input_pyramid.back();
	for (int i = input_pyramid.size() - 1; i > 0; i--) {
		Mat up_level;
		pyrUp(currentImg, up_level, input_pyramid[i - 1].size());
		// Mat resize_level;
		// resize(up_level, resize_level, input_pyramid[i].size(), 0, 0 interpolation = INTER_CUBIC);
		currentImg = up_level + input_pyramid[i - 1];
	}
	return currentImg;
}

// vector<Mat> GaussianPyramid(Mat R) {
// 	// generate the gaussian pyramid of image R
// 	int level = (int)log(min(R.cols, R.rows));
// 	Mat G = R.clone();
// 	vector<Mat> GP;
// 	GP.push_back(G);
// 	for (int i = 0; i < level; i++) {
// 		Mat down_level;
// 		pyrDown(GP[i], down_level);
// 		GP.push_back(G);
// 	}
// 	return GP;
// }

vector<Mat_<float>> LaplacianPyramid(const Mat_<float> &img) {
	// generate Laplacian Pyramid of image img
	// to generate Laplacian Pyramid, you need generate GaussianPyramid
	int level = (int)(log10(min(img.cols, img.rows)) / log10(2));
	vector<Mat_<float>> LP; // LaplacianPyramid
	Mat currentImg = img;

	for (int i = 0; i < level; i++) {
		Mat up_level, down_level;
		pyrDown(currentImg, down_level);				// down sample G[i]
		pyrUp(down_level, up_level, currentImg.size()); // up sample
		Mat L = currentImg - up_level;
		// imshow("up" + to_string(i), up_level);
		LP.push_back(L); // add a level in Laplacian pyramid
		// cout << L << endl;
		currentImg = down_level;
	}
	LP.push_back(currentImg);
	return LP;
}