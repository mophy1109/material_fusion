/*
 * @Author: USTB.mophy1109
 * @Date: 2018-03-29 16:30:25
 * @Last Modified by: USTB.mophy1109
 * @Last Modified time: 2018-05-23 17:55:31
 */

#include "CFuImage.h"
#include "CStitching.h"
#include "Preprocess.h"
#include <algorithm>
#include <boost/timer.hpp> //timer
#include <dirent.h>
#include <iostream>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/xfeatures2d.hpp>
#include <string>

using namespace std;
using namespace cv;

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

		// set sampling rate to 3fps
		if (currentFrame % int(rate / 3) != 0) {
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
		F_Img1.Clone(F_Img2);
		tempImg.release();
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
		F_Img1.Clone(F_Img2);
		tempImg.release();
	}
	cout << "total cost time:" << timer.elapsed() << endl;
	imwrite("../data/result/base.jpg", result);
}

int main() {

	char src[255] = "../data/videos/500px.avi";
	char tar[255] = "../data/500X/";
	// cin >> tar;
	// ProcessWithoutSampling(tar);
	ProcessWithSampling(src);
	return 0;
}