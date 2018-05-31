/*
 * @Author: USTB.mophy1109
 * @Date: 2018-03-30 12:49:23
 * @Last Modified by: USTB.mophy1109
 * @Last Modified time: 2018-05-31 10:49:26
 */

#include "CFusion.h"
#include "Program.h"
#include <opencv2/core/cuda.hpp>
#include <opencv2/core/utility.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;
using namespace cuda;

#define MAX_LEVEL 7
void calculateSF(const GpuMat &img1, const GpuMat &img2, GpuMat &result, Stream &stream = Stream::Null());

void eliminateBlack(Mat& img1,Mat& img2)
{	
	//modify the 0s in roi1 as the value of the same location in roi2
	for(int i=0;i<img1.rows;i++)
	{
	    for(int j=0;j<img1.cols;j++)
	    {
			if (img1.at<uchar>(i,j) == 0){
            	img1.at<uchar>(i,j)=img2.at<uchar>(i,j);
			}
        }
    }
}

Mat FusionImages(Mat &roi1, Mat &roi2, FusionMethod method) {
	switch (method) {
		case LINEAR: {
			break;
		}
		case MULTIBAND: {

			Mat_<float> f_img1;
			Mat_<float> f_img2;
			Mat_<float> f_Wtmat; // weight Matrix
			
			eliminateBlack(roi1, roi2);

			Mat SFMat = getSFMatrix(roi1, roi2);

			// reset type to float
			roi1.convertTo(f_img1, CV_32F);
			roi2.convertTo(f_img2, CV_32F);
			SFMat.convertTo(f_Wtmat, CV_32F);

			// calculate pyramids
			vector<Mat_<float>> lap1 = LaplacianPyramid(f_img1);
			vector<Mat_<float>> lap2 = LaplacianPyramid(f_img2);
			vector<Mat_<float>> gaussian_SF = GaussianPyramid(f_Wtmat);
			// vector<Mat> gaussian_SF = RGaussianPyramid(f_Wtmat);
			vector<Mat_<float>> reconstructLap;
			int level = (int)(log10(min(roi1.cols, roi1.rows)) / log10(2));
			int gau_InitLevel = min(level - 1, MAX_LEVEL);
			// generate reconstruct laplacian pyramid
			for (int i = 0; i < lap1.size(); i++) {
				// cout << lap1[i].cols << " , " << lap1[i].rows <<endl;
				// cout << lap2[i].cols << " , " << lap2[i].rows <<endl;
				// cout << gaussian_SF[gau_InitLevel - i].cols << " , " << gaussian_SF[gau_InitLevel - i].rows <<endl;
				// Mat GF;
				// gaussian_SF[i].convertTo(GF, CV_32F);

				// Mat_<float> temp_level = lap1[i].mul(GF) + lap2[i].mul(1 - GF);
				Mat_<float> temp_level = lap1[i].mul(gaussian_SF[gau_InitLevel - i]) + lap2[i].mul(1 - gaussian_SF[gau_InitLevel - i]);

				// imshow("gau", gaussian_SF[i]);
				// imshow("anti_gau", 1-gaussian_SF[i]);
				// waitKey(0);
				reconstructLap.push_back(temp_level);
			}

			Mat result;
			reconstruct(reconstructLap).convertTo(result, CV_8U);
			// imshow("reconstruct", result);
			// waitKey(50);
			return result;
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

Mat_<float> reconstruct(vector<Mat_<float>> input_pyramid) {
	// reconstruct Laplacian pyramid
	Mat currentImg = input_pyramid[0];
	for (int i = 1; i < input_pyramid.size(); i++) {
		Mat up_level;
		pyrUp(currentImg, up_level, input_pyramid[i].size());
		// Mat resize_level;
		// resize(up_level, resize_level, input_pyramid[i].size(), 0, 0 interpolation = INTER_CUBIC);
		currentImg = up_level + input_pyramid[i];
	}
	return currentImg;
}

// generate gaussian pyramid of weight matrix R
vector<Mat> RGaussianPyramid(const Mat_<float> R) {
	// generate the gaussian pyramid of image R
	int level = (int)(log10(min(R.cols, R.rows)) / log10(2));
	Mat G;
	R.convertTo(G, CV_8U);
	vector<Mat> GP;
	GP.push_back(G);
	for (int i = 0; i < level + 1; i++) {
		Mat down_level;
		cv::pyrDown(GP[i], down_level);
		// cout << GP[i].cols << " , " << GP[i].rows <<endl;
		// cout << down_level.cols << " , " << down_level.rows <<endl;

		// imshow("gaussian down level:" + to_string(i), StretchImage(down_level));
		// waitKey(0);
		// cout << down_level<<endl;
		GP.push_back(down_level);
	}
	return GP;
}

// generate gaussian pyramid of weight matrix R
vector<Mat_<float>> GaussianPyramid(const Mat_<float> R) {
	// generate the gaussian pyramid of image R
	int level = (int)(log10(min(R.cols, R.rows)) / log10(2));
	Mat_<float> G = R.clone();
	vector<Mat_<float>> GP;
	GP.push_back(G);
	for (int i = 0; i < level; i++) {
		Mat_<float> down_level;
		cv::pyrDown(GP[i], down_level);
		// cout << GP[i].cols << " , " << GP[i].rows <<endl;
		// cout << down_level.cols << " , " << down_level.rows <<endl;

		// imshow("gaussian down level:" + to_string(i), StretchImage(down_level));
		// waitKey(0);
		// cout << down_level<<endl;
		GP.push_back(down_level);
	}
	return GP;
}

//待测试内容：GPU pyramid效率对比
vector<Mat_<float>> LaplacianPyramid(const Mat_<float> &img) {
	// generate Laplacian Pyramid of image img
	// to generate Laplacian Pyramid, you need generate GaussianPyramid
	int level = (int)(log10(min(img.cols, img.rows)) / log10(2));
	vector<Mat_<float>> GP = GaussianPyramid(img);
	vector<Mat_<float>> LP; // LaplacianPyramid
	LP.push_back(GP[min(level - 1, MAX_LEVEL)]);
	// Mat currentImg = img;

	for (int i = min(level - 1, MAX_LEVEL); i > 0; i--) {
		Mat up_level;
		// cv::pyrDown(currentImg, down_level);			// down sample G[i]
		// pyrUp(down_level, up_level, currentImg.size()); // up sample
		pyrUp(GP[i], up_level, GP[i - 1].size()); // up sample

		Mat L = GP[i - 1] - up_level;
		// imshow("up" + to_string(i), up_level);
		LP.push_back(L); // add a level in Laplacian pyramid
		// cout << L << endl;
		// currentImg = down_level;
	}
	// LP.push_back(currentImg);
	return LP;
}