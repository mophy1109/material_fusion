/*
 * @Author: USTB.mophy1109
 * @Date: 2018-03-30 12:49:23
 * @Last Modified by:   USTB.mophy1109
 * @Last Modified time: 2018-05-24 17:02:24
 */

#include "CFusion.h"

using namespace std;
using namespace cv;

void FusionImages(cv::Mat roi1, cv::Mat roi2, FusionMethod method) {
	switch (method) {
		case LINEAR: {
			break;
		}
		case MULTIBAND: {
			Mat_<float> f_img1;
			Mat_<float> f_img2;
			Mat_<float> f_Wtmat; // weight Matrix
			roi1.convertTo(f_img1, CV_32F, 1.0 / 255.0);
			roi2.convertTo(f_img2, CV_32F, 1.0 / 255.0);
			vector<Mat_<float>> lap1 = LaplacianPyramid(f_img1);
			vector<Mat_<float>> lap2 = LaplacianPyramid(f_img2);
			// f_Wtmat =
			// Mat imgr = reconstruct(lap);
		}
	}
}

Mat_<float> StretchImage(Mat_<float> region) {
	// region is a (0,1) img, to make it more clear, set pixel-value "1" as 255 (white) for visual demonstration.
	Mat out = region.clone();
	out = out * 255;
	return out;
}

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

// generate gaussian pyramid of weight matrix R
vector<Mat_<float>> GaussianPyramid(const Mat_<float> R) {
	// generate the gaussian pyramid of image R
	int level = (int)log(min(R.cols, R.rows));
	Mat_<float> G = R.clone();
	vector<Mat_<float>> GP;
	GP.push_back(G);
	for (int i = 0; i < level; i++) {
		Mat_<float> down_level;
		pyrDown(GP[i], down_level);
		imshow("gaussian down level:" + to_string(i), StretchImage(down_level));
		GP.push_back(down_level);
	}
	return GP;
}

//待测试内容：GPU pyramid效率对比
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