/*
 * @Author: USTB.mophy1109
 * @Date: 2018-04-02 11:13:23
 * @Last Modified by: USTB.mophy1109
 * @Last Modified time: 2018-05-29 11:29:40
 */

#include "CStitching.h"
#include "CFusion.h"
#include <algorithm>	   //std::sort
#include <boost/timer.hpp> //timer
#include <iostream>
#include <math.h>
#include <opencv2/calib3d.hpp>
#include <vector>

using namespace cv;
using namespace std;

int CStitching::Stitching(CFuImage &img1, CFuImage &img2, Mat &result) {
	// check if the num of matches is less than 2500
	if (img2.key.size() < 10500) {
		cout << img1.key.size() << "," << img2.key.size() << endl;
		cout << "Key of img2 is less than 10500,pass." << endl;
		return 0;
	}
	boost::timer timer;

	// Matching descriptor vectors using SiftGPU Macher
	SiftMatchGPU matcher(8192);

	// maximum number to match
	matcher.SetLanguage(SiftMatchGPU::SIFTMATCH_LANGUAGE::SIFTMATCH_CUDA); // set cuda to accelerate

	if (matcher.VerifyContextGL() == 0)
		return 1;

	// set descriptors
	matcher.SetDescriptors(0, img1.FeatureNum, img1.descriptor);
	matcher.SetDescriptors(1, img2.FeatureNum, img2.descriptor);

	// save offsets for further use
	vector<Point> offsets;
	vector<Point> train_point;
	vector<Point> query_point;

	int match_buf[8192][2];
	int nmatch = matcher.GetSiftMatch(8192, match_buf);

	for (int i = 0; i < nmatch; i++) {
		train_point.push_back(Point(img1.key[match_buf[i][0]].x, img1.key[match_buf[i][0]].y));
		query_point.push_back(Point(img2.key[match_buf[i][1]].x, img2.key[match_buf[i][1]].y));
		// cout << "(" << offsets[i].x << "," << offsets[i].y << ")" << endl;
	}
	cout << "Found " << nmatch << " matches.      Match cost time=" << timer.elapsed() << endl;

	timer.restart();
	Offset off = CalOffset(train_point, query_point, CalOffsetMethod);
	// Offset off2 = CalOffset(train_point, query_point, AVERAGE);
	cout << "OFFSET AFFINE:(" << off.x << "," << off.y << ")      CalOffset cost time=" << timer.elapsed() << endl;
	// cout << "OFFSET Average:(" << off2.x << "," << off2.y << ")" << endl;

	g_offset.x += off.x;
	g_offset.y += off.y;

	timer.restart();

	// imshow("result0", result);
	vector<Mat> rois = CalROI(result, img2.m_Img);
	cout << "CalROI cost time=" << timer.elapsed() << endl;
	timer.restart();
	Mat RoiFusion = FusionImages(rois[0], rois[1], MULTIBAND);
	// cout << RoiFusion.cols << " , " << RoiFusion.rows <<endl;
	// cout << result.cols << " , " << result.rows <<endl;

	cout << "Fusion cost time=" << timer.elapsed() << endl;
	// cout << result.type() << " "<< RoiFusion.type() << endl;
	RoiFusion.copyTo(result(rec_ROI));
	// imshow("result", result);
	// waitKey(0);
	img1.Clone(img2);
	return 0;
}

Offset CStitching::CalOffset(vector<Point> train_point, vector<Point> query_point, CalMethod method) {
	Offset off = { -9999, -9999 }; // if returns -9999 or so means error
	switch (method) {
		case AVERAGE: {
			// calculate avg of offsets
			double sum_x = 0, sum_y = 0;
			int i;
			for (i = 0; i < train_point.size(); i++) {
				sum_x += train_point[i].x - query_point[i].x;
				sum_y += train_point[i].y - query_point[i].y;
			}
			off = { (int)round(sum_x / i), (int)round(sum_y / i) };
			break;
		}
		case HOMO: {
		}
		case AFFINE: {
			//利用仿射变换模型、RANSAC进行变换矩阵的计算
			//矩阵形式  [m1 m2 m3]
			//         [n1 n2 n3]
			//理想情况下（无旋转和畸变）(m3, n3)即为所求的偏移量
			std::vector<uchar> inliers(train_point.size(), 0);
			Mat AffineMatrix = estimateAffine2D(query_point, train_point, inliers, RANSAC, 3, 2000, 0.999, 50);
			off = { (int)round(AffineMatrix.ptr<double>(0)[2]), (int)round(AffineMatrix.ptr<double>(1)[2]) };
			// cout << "estimateAffinePartial2D" << AffineMatrix << "\n" << std::flush;
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

vector<Mat> CStitching::CalROI(Mat &img1, Mat img2) {
	Mat ROI1;
	Mat ROI2;
	Mat out, ROIimg1, ROIimg2;
	int rows1 = img1.rows;
	int cols1 = img1.cols;
	int rows2 = img2.rows;
	int cols2 = img2.cols;
	// cout << "GLOBAL OFFSET:(" << g_offset.x << "," << g_offset.y << ")" << endl;
	// cout << "img2 :(" << cols2 << "," << rows2 << ")" << endl;
	if (g_offset.x >= 0 && g_offset.y >= 0) {
		// image2 is at the right buttom corner of image1
		out = Mat(max(rows1, g_offset.y + rows2), max(g_offset.x + cols2, cols1), CV_8U, Scalar(0));
		// cout << "type 1 : right buttom" << endl;

		int ROI_x = g_offset.x;
		int ROI_y = g_offset.y;
		int ROI_width = min(cols2, cols1 - g_offset.x);
		int ROI_height = min(rows2, rows1 - g_offset.y);
		rec_ROI = Rect(ROI_x, ROI_y, ROI_width, ROI_height);
		// cout << ROI_width << "," << ROI_height << endl;

		// put 2 images together and extract ROIs
		img1.copyTo(out(Rect(0, 0, cols1, rows1)));
		ROIimg1 = out(rec_ROI).clone();
		img2.copyTo(out(Rect(g_offset.x, g_offset.y, cols2, rows2)));
		ROIimg2 = out(rec_ROI).clone();
	} else if (g_offset.x >= 0 && g_offset.y < 0) {
		// image2 is at the right top corner of image1
		out = Mat(-g_offset.y + rows1, max(g_offset.x + cols2, cols1), CV_8U, Scalar(0));
		// cout << "type 2 : right top" << endl;

		int ROI_x = g_offset.x;
		int ROI_y = -g_offset.y;
		int ROI_width = min(cols2, cols1 - g_offset.x);
		int ROI_height = rows2 + g_offset.y;
		rec_ROI = Rect(ROI_x, ROI_y, ROI_width, ROI_height);
		// cout << ROI_width << "," << ROI_height << endl;

		// put 2 images together and extract ROIs
		img1.copyTo(out(Rect(0, -g_offset.y, cols1, rows1)));
		ROIimg1 = out(rec_ROI).clone();
		img2.copyTo(out(Rect(g_offset.x, 0, cols2, rows2)));
		ROIimg2 = out(rec_ROI).clone();
		g_offset.y = 0;
	} else if (g_offset.x < 0 and g_offset.y >= 0) {
		// image2 is at the left bottom corner of image1

		out = Mat(max(rows1, g_offset.y + rows2), -g_offset.x + cols1, CV_8U, Scalar(0));
		// cout << "type 3 :left bottom" << endl;

		int ROI_x = -g_offset.x;
		int ROI_y = g_offset.y;
		int ROI_width = cols2 + g_offset.x;
		int ROI_height = min(rows2, rows1 - g_offset.y);
		rec_ROI = Rect(ROI_x, ROI_y, ROI_width, ROI_height);
		// cout << ROI_width << "," << ROI_height << endl;

		// put 2 images together and extract ROIs
		img1.copyTo(out(Rect(-g_offset.x, 0, cols1, rows1)));
		ROIimg1 = out(rec_ROI).clone();
		img2.copyTo(out(Rect(0, g_offset.y, cols2, rows2)));
		ROIimg2 = out(rec_ROI).clone();
		g_offset.x = 0;
	} else {
		// image2 is at the left top corner of image1
		out = Mat(-g_offset.y + rows1, -g_offset.x + cols1, CV_8U, Scalar(0));
		// cout << "type 4 :left top" << endl;
		int ROI_x = -g_offset.x;
		int ROI_y = -g_offset.y;
		int ROI_width = cols2 + g_offset.x;
		int ROI_height = rows2 + g_offset.y;
		rec_ROI = Rect(ROI_x, ROI_y, ROI_width, ROI_height);
		// cout << ROI_width << "," << ROI_height << endl;

		// put 2 images together and extract ROIs
		img1.copyTo(out(Rect(-g_offset.x, -g_offset.y, cols1, rows1)));
		ROIimg1 = out(rec_ROI).clone();
		img2.copyTo(out(Rect(0, 0, cols2, rows2)));
		ROIimg2 = out(rec_ROI).clone();
		g_offset.x = 0;
		g_offset.y = 0;
	}

	// imshow("roi1", ROIimg1);
	// imshow("roi2", ROIimg2);
	// waitKey(500);
	// imwrite("./temp/tmp_img.jpg", out);
	img1.release();
	img1 = out;
	vector<Mat> ROI;

	ROI.push_back(ROIimg1);
	ROI.push_back(ROIimg2);
	return (ROI);
}