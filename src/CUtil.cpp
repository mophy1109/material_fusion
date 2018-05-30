/*
 * @Author: USTB.mophy1109
 * @Date: 2018-04-08 14:07:18
 * @Last Modified by: USTB.mophy1109
 * @Last Modified time: 2018-05-29 16:29:44
 */

#include "CUtil.h"
#include "CFuImage.h"
#include "CStitching.h"
#include "SiftGPU.h"
#include <boost/timer.hpp> //timer
#include <dirent.h>
#include <iostream>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/xfeatures2d.hpp>
#include <regex>

using namespace std;
using namespace cv;

vector<string> GetAllFiles(string path, string suffix) {
	vector<string> files;
	files.clear();
	regex reg_obj(suffix);

	vector<string> paths;
	paths.push_back(path);

	for (int i = 0; i < paths.size(); i++) {
		string curr_path = paths[i];
		DIR *dp;
		struct dirent *dirp;
		if ((dp = opendir(curr_path.c_str())) == NULL) {
			cerr << "can not open this file." << endl;
			continue;
		}
		while ((dirp = readdir(dp)) != NULL) {
			if (dirp->d_type == 4) {
				if ((dirp->d_name)[0] == '.') // 排除 '.' 和 '..'
					continue;
				string tmp_path = curr_path + "/" + dirp->d_name;
				cout << tmp_path << " ";
				paths.push_back(tmp_path);
				cout << paths[paths.size() - 1] << endl;
			} else if (dirp->d_type == 8) {
				if (regex_match(dirp->d_name, reg_obj)) {
					string full_path = curr_path + "/" + dirp->d_name;
					files.push_back(full_path);
				}
			}
		}
		closedir(dp);
	}
	return files;
}

// process with video
int ProcessWithSampling(char *src) {

	// Init siftgpu,using cuda
	SiftGPU sift;
	char *myargv[4] = { "-cuda", "0", "-v", "0" }; // siftGPU 默认参数
	sift.ParseParam(4, myargv);

	// test if your hardware support SiftGPU
	int support = sift.CreateContextGL();
	if (support != SiftGPU::SIFTGPU_FULL_SUPPORTED) {
		cout << "SiftGPU is not supported!" << endl;
		return 1;
	}

	// open video
	VideoCapture capture(src);

	// check if opened successfully(isOpened returns ture)
	if (!capture.isOpened())
		cout << "fail to open video!" << endl;

	// get total number of frames
	long totalFrameNumber = capture.get(CV_CAP_PROP_FRAME_COUNT);
	cout << "Num of Frames:" << totalFrameNumber << " Frames" << endl;

	// get frame rate
	double rate = capture.get(CV_CAP_PROP_FPS);
	cout << "Frame Rate: " << rate << "fps" << endl;

	// get resolution
	int width = (int)capture.get(CV_CAP_PROP_FRAME_WIDTH);
	int height = (int)capture.get(CV_CAP_PROP_FRAME_HEIGHT);
	cout << "Resolution: " << width << "*" << height << endl;
	cout << "Sampling video...." << endl;

	Mat frameImg;						// save temp frame
	Rect rect(0, 0, width, height - 2); //由于源视频底部出现黑边，需要裁剪2px宽度，roi设置为width×（height-2）的图像

	int i = 1; // number
	cout << "==========start program=========" << endl;

	// timer
	boost::timer timer;
	if (!capture.read(frameImg)) {
		cout << "读取视频失败" << endl;
		return 1;
	}
	Mat result;
	cvtColor(frameImg(rect), result, COLOR_RGB2GRAY); // first frame

	CStitching stitch;
	CFuImage F_Img1 = CFuImage(result);

	for (int currentFrame = 1; currentFrame < totalFrameNumber; currentFrame++) {

		// read frame
		if (!capture.read(frameImg)) {
			cout << "读取视频失败" << endl;
			return 1;
		}

		// set sampling rate to 5fps
		if (currentFrame % int(rate / 5) != 0) {
			continue;
		}

		// print out some infomation of progress
		cout << "\nProcessing image " << i << " and image " << i + 1 << endl;
		i++;

		// save temp frame of gray
		Mat tempImg;
		cvtColor(frameImg(rect), tempImg, COLOR_RGB2GRAY);

		// generate info of temp frame, save to F_Img2
		CFuImage F_Img2 = CFuImage(tempImg);

		// Stitching, using F_Img1 and F_Img2 to calculate ROI and excute fusion
		stitch.Stitching(F_Img1, F_Img2, result);

		// set F_Img1 to F_Img2, so that the procedure above can correctly be excuted
		// F_Img1.Clone(F_Img2);
		tempImg.release();
		// imwrite("../data/result/base.jpg", result);
		// imshow("temp_result", result);
		// waitKey(50);

	}
	cout << "total cost time:" << timer.elapsed() << endl;
	imwrite("../data/result/base.jpg", result);
}

// process with pics
int ProcessWithoutSampling(char *dir) {
	// Init siftgpu,using cuda
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
		// imwrite("../data/result/base.jpg", result);
		// F_Img1.Clone(F_Img2);
		// imshow("result", result);
		tempImg.release();
	}
	cout << "total cost time:" << timer.elapsed() << endl;
	imwrite("../data/result/base.jpg", result);
}
