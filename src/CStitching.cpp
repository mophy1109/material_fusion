/*
 * @Author: USTB.mophy1109
 * @Date: 2018-04-02 11:13:23
 * @Last Modified by: USTB.mophy1109
 * @Last Modified time: 2018-04-08 16:40:58
 */

#include "CStitching.h"
#include "SiftGPU.h"

#include <GL/gl.h>
#include <algorithm>	   //std::sort
#include <boost/timer.hpp> //timer
#include <iostream>
#include <opencv2/calib3d.hpp>
#include <vector>

using namespace cv;
using namespace std;

int CStitching::Stitching(Mat img1, Mat img2, Mat result) {

	//采用CUDA并行方式进行图像配准
	SiftGPU sift_1;
	SiftGPU sift_2;
	char *myargv[2] = { "-cuda", "0" };
	sift_1.ParseParam(2, myargv);
	sift_2.ParseParam(2, myargv);

	// test if your hardware support SiftGPU
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
	float *descriptors_1 = new float[128 * num_1];
	float *descriptors_2 = new float[128 * num_2];
	vector<SiftGPU::SiftKeypoint> keys_1(num_1);
	vector<SiftGPU::SiftKeypoint> keys_2(num_2);
	sift_1.GetFeatureVector(&keys_1[0], &descriptors_1[0]);
	sift_2.GetFeatureVector(&keys_2[0], &descriptors_2[0]);

	// Matching descriptor vectors using SiftGPU Macher
	timer.restart(); // match time
	SiftMatchGPU matcher(4096);

	// maximum number to match
	matcher.SetLanguage(SiftMatchGPU::SIFTMATCH_LANGUAGE::SIFTMATCH_CUDA); // set cuda to accelerate

	if (matcher.VerifyContextGL() == 0)
		return 1;

	// set descriptors
	matcher.SetDescriptors(0, num_1, descriptors_1);
	matcher.SetDescriptors(1, num_2, descriptors_2);

	// save offsets for further use
	vector<Point> offsets;
	vector<Point> train_point;
	vector<Point> query_point;

	int match_buf[4096][2];
	int nmatch = matcher.GetSiftMatch(4096, match_buf);
	for (int i = 0; i < nmatch; i++) {
		train_point.push_back(Point(keys_1[match_buf[i][0]].x, keys_1[match_buf[i][0]].y));
		query_point.push_back(Point(keys_2[match_buf[i][1]].x, keys_2[match_buf[i][1]].y));
		// cout << "(" << (string)offsets[i].x << ")" << endl;
	}
	cout << "Found " << nmatch << " matches.     Match cost time=" << timer.elapsed() << endl;

	timer.restart();
	Offset off = CalOffset(train_point, query_point, CalOffsetMethod);
	cout << "AFFINE OFFSET:(" << off.x << "," << off.y << ")      CalOffset cost time=" << timer.elapsed() << endl;

	return 0;
}

Offset CStitching::CalOffset(vector<Point> train_point, vector<Point> query_point, CalMethod method) {
	Offset off = { -9999, -9999 }; //返回项中出现-9999等说明出现错误
	switch (method) {
		case AVERAGE: {
			double sum_x = 0, sum_y = 0;
			int i;
			for (i = 0; i < train_point.size(); i++) {
				sum_x += query_point[i].x - train_point[i].x;
				sum_y += query_point[i].y - train_point[i].y;
			}
			off = { sum_x / i, sum_y / i };
			break;
		}
		case HOMO: {
			break;
		}
		case AFFINE: {
			std::vector<uchar> inliers(train_point.size(), 0);
			Mat AffineMatrix = estimateAffine2D(train_point, query_point, inliers, RANSAC, 3, 2000, 0.995, 10);
			off = { AffineMatrix.ptr<double>(0)[2], AffineMatrix.ptr<double>(1)[2] };
			// std::cout << "estimateAffinePartial2D" << AffineMatrix << "\n" << std::flush;
			break;
		}
		case PROSAC: {
			break;
		}
	}

	// timer
	// boost::timer timer;

	return off;
}