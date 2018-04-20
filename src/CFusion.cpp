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
Mat reconstruct(vector<Mat> input_pyramid) {
	// reconstruct Laplacian pyramid
	Mat out = input_pyramid[0];
	for (int i = input_pyramid.size() - 1; i >= 0; i--) {
		Mat up_level;
		pyrUp(input_pyramid[i], up_level, input_pyramid[i - 1].size(), BORDER_DEFAULT);
		// Mat resize_level;
		// resize(up_level, resize_level, input_pyramid[i].size(), 0, 0 interpolation = INTER_CUBIC);
		out.release();
		out = up_level + input_pyramid[i];
	}
	return out;
}

vector<Mat> GaussianPyramid(Mat R) {
	// generate the gaussian pyramid of image R
	int level = (int)log(min(R.cols, R.rows));
	Mat G = R.clone();
	vector<Mat> GP;
	GP.push_back(G);
	for (int i = 0; i < level; i++) {
		Mat down_level;
		pyrDown(GP[i], down_level);
		GP.push_back(G);
	}
	return GP;
}

vector<Mat> LaplacianPyramid(Mat img) {
	// generate Laplacian Pyramid of image img
	// to generate Laplacian Pyramid, you need generate GaussianPyramid
	int level = (int)log(min(img.cols, img.rows));
	vector<Mat> LP; // LaplacianPyramid
	vector<Mat> GP; // GaussianPyramid
	Mat G = img.clone();
	GP.push_back(G);

	for (int i = 0; i < level; i++) {
		Mat up_level, down_level;
		pyrDown(GP[i], down_level); // down sample G[i]
		GP.push_back(down_level);   // add a level in Gaussian pyramid

		pyrUp(down_level, up_level, GP[i].size(), BORDER_DEFAULT); // up sample
		Mat L = GP[i] - up_level;
		LP.push_back(L); // add a level in Laplacian pyramid
	}
	return LP;
}