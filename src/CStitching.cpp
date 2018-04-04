/*
 * @Author: USTB.mophy1109
 * @Date: 2018-04-02 11:13:23
 * @Last Modified by: USTB.mophy1109
 * @Last Modified time: 2018-04-04 18:35:29
 */

#include "CStitching.h"
#include "SiftGPU.h"
// third party libs
#include <GL/gl.h>
#include <boost/timer.hpp> //timer
#include <iostream>

using namespace cv;
using namespace std;

// std::vector<DMatch> FindGoodMatches(std::vector<DMatch> matches) {
// 	std::vector<DMatch> goodMatches;
// 	float dist1 = 0.0, dist2 = 0.0;
// 	float nndr = 0.7;
// 	for (size_t i = 0; i < matches.size(); i++) {
// 		DMatch dmatch = matches[i][0];
// 		dist1 = matches[i][0].distance;
// 		dist2 = matches[i][1].distance;

// 		if (dist1 < nndr * dist2) {
// 			// goodMatches.push_back(train[dmatch.queryIdx].pt);
// 			// goodMatches.push_back(query[dmatch.trainIdx].pt);
// 			goodMatches.push_back(dmatch);
// 		}
// 	}
// 	return goodMatches;
// }

int CStitching::Stitching(Mat img1, Mat img2, Mat result) {

	//采用CUDA并行方式进行图像陪准
	SiftGPU sift_1;
	SiftGPU sift_2;
	char *myargv[2] = { "-cuda", "0" };
	sift_1.ParseParam(2, myargv);
	sift_2.ParseParam(2, myargv);

	//检查硬件是否支持SiftGPU
	int support = sift_1.CreateContextGL();
	if (support != SiftGPU::SIFTGPU_FULL_SUPPORTED) {
		cout << "SiftGPU is not supported!" << endl;
		return 1;
	}

	boost::timer timer;
	int width = img1.cols;
	int height = img1.rows;
	//运行sift算法
	sift_1.RunSIFT(width, height, img1.data, GL_LUMINANCE, GL_UNSIGNED_BYTE);
	sift_2.RunSIFT(width, height, img2.data, GL_LUMINANCE, GL_UNSIGNED_BYTE);
	cout << "runsift() cost time=" << timer.elapsed() << endl;

	//获取关键点和描述子
	int num_1 = sift_1.GetFeatureNum();
	int num_2 = sift_2.GetFeatureNum();

	cout << "features Num of pic1:" << num_1 << endl;
	cout << "features Num of pic2:" << num_2 << endl;

	vector<float> descriptors_1(128 * num_1);
	vector<float> descriptors_2(128 * num_2);
	vector<SiftGPU::SiftKeypoint> keys_1(num_1);
	vector<SiftGPU::SiftKeypoint> keys_2(num_2);
	sift_1.GetFeatureVector(&keys_1[0], &descriptors_1[0]);
	sift_2.GetFeatureVector(&keys_2[0], &descriptors_2[0]);
	// Matching descriptor vectors using BFMatcher.OPENCV METHOD
	timer.restart(); // match time
	BFMatcher matcher;
	std::vector<DMatch> matches;
	matcher.match(descriptors_1, descriptors_2, matches);
	// std::vector<DMatch> goodMatches = FindGoodMatches(matches);

	// calculateOffset(goodMatches, CAL.AVERAGE);
	// cout << "good matches num = " << goodMatches.size() << endl;
	cout << "match cost time=" << timer.elapsed() << endl;
}
