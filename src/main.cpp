/*
 * @Author: USTB.MophyChen
 * @Date: 2018-03-29 16:30:25
 * @Last Modified by: USTB.mophy1109
 * @Last Modified time: 2018-04-16 14:40:51
 */

#include "CFuImage.h"
#include "CStitching.h"
#include "Preprocess.h"
#include "highgui.h"
#include <algorithm>
#include <boost/timer.hpp> //timer
#include <dirent.h>
#include <iostream>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/xfeatures2d.hpp>
#include <string>

using namespace std;
using namespace cv;

void ProcessWithSamping(char *src, char *tar_dir) {
	Sampling(src, tar_dir);
	vector<string> files = GetAllFiles(tar_dir, ".*(jpg|bmp|jpeg)$");
	sort(files.begin(), files.end());
	cout << "==========start program=========" << endl;
}

int ProcessWithoutSamping(char *dir) {
	//采用CUDA并行方式进行图像配准
	SiftGPU sift;
	char *myargv[4] = { "-cuda", "0", "-v", "0" }; // siftGPU 默认参数
	sift.ParseParam(4, myargv);

	// test if your hardware support SiftGPU
	int support = sift.CreateContextGL();
	if (support != SiftGPU::SIFTGPU_FULL_SUPPORTED) {
		cout << "SiftGPU is not supported!" << endl;
		return 1;
	}
	boost::timer timer;
	vector<string> files = GetAllFiles(dir, ".*(jpg|bmp|jpeg)$");
	sort(files.begin(), files.end());
	cout << "==========start program=========" << endl;
	Mat result = imread(files[0], IMREAD_GRAYSCALE);
	CStitching stitch;
	CFuImage F_Img1 = CFuImage(result);
	for (int i = 1; i < files.size() - 1; i++) {
		cout << "\nProcessing image " << i - 1 << " and image " << i << endl;
		Mat tempImg = imread(files[i], IMREAD_GRAYSCALE);
		CFuImage F_Img2 = CFuImage(tempImg);
		stitch.Stitching(F_Img1, F_Img2, result);
		imwrite("../data/result/base.jpg", result);
		F_Img1.Clone(F_Img2);
		tempImg.release();

		// char *src = "/media/cwh1001/Workspace/CUDA_projects/material_fusion/data/result/";
	}
	cout << "total cost time:" << timer.elapsed() << endl;
	// imwrite("../data/result/base.jpg", result);
}

int main() {

	char src[255] = "../data/videos/500px.avi";
	char tar[255] = "../data/500X/";
	// cin >> tar;
	// ProcessWithSamping(src, tar);
	ProcessWithoutSamping(tar);
	return 0;
}