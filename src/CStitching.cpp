/*
 * @Author: USTB.mophy1109
 * @Date: 2018-04-02 11:13:23
 * @Last Modified by: USTB.mophy1109
 * @Last Modified time: 2018-04-08 16:40:58
 */

#include "CStitching.h"
#include "SiftGPU.h"

#include <GL/gl.h>
#include <boost/timer.hpp> //timer
#include <iostream>
#include <vector>

using namespace cv;
using namespace std;

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
	vector<Offset> offsets;
	int match_buf[4096][2];
	int nmatch = matcher.GetSiftMatch(4096, match_buf);
	for (int i = 0; i < nmatch; i++) {
		float deltx = keys_2[match_buf[i][1]].x - keys_1[match_buf[i][0]].x;
		float delty = keys_2[match_buf[i][1]].y - keys_1[match_buf[i][0]].y;
		Offset tmp_offset;
		tmp_offset.x = deltx;
		tmp_offset.y = delty;
		offsets.push_back(tmp_offset);
		// cout << "(" << (string)offsets[i].x << ")" << endl;
	}
	Offset off = CalOffset(offsets, AVERAGE);
	cout << "(" << off.x << "," << off.y << ")" << endl;
	cout << "Found " << nmatch << " matches." << endl;
	cout << "match cost time=" << timer.elapsed() << endl;

	return 0;
}

Offset CStitching::CalOffset(vector<Offset> offsets, int method) {
	Offset off = { -9999, -9999 };
	switch (method) {
		case AVERAGE: {
			float sum_x = 0, sum_y = 0;
			int i;
			for (i = 0; i < offsets.size(); i++) {
				sum_x += offsets[i].x;
				sum_y += offsets[i].y;
			}
			off = { sum_x / i, sum_y / i };
			break;
		}
		case MODE: {

			break;
		}
		case HOMO: {
			break;
		}
		case RANSAC: {
			break;
		}
	}

	// timer
	// boost::timer timer;

	return off;
}