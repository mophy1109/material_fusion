/*
 * @Author: USTB.mophy1109
 * @Date: 2018-04-02 16:05:55
 * @Last Modified by: USTB.mophy1109
 * @Last Modified time: 2018-04-16 11:09:17
 * @For test
 */

#include "CStitching.h"
#include "Preprocess.h"
#include <dirent.h>
#include <iostream>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/xfeatures2d.hpp>
#include <string>

using namespace std;
using namespace cv;

void mytestsift() {

	Mat img1 = imread("/media/cwh1001/Workspace/CUDA_projects/material_fusion/data/500X 500PIC/sam00001.jpg", IMREAD_GRAYSCALE);
	Mat img2 = imread("/media/cwh1001/Workspace/CUDA_projects/material_fusion/data/500X 500PIC/sam00002.jpg", IMREAD_GRAYSCALE);
	Mat result;
	CStitching CCCC;
	CCCC.Stitching(img1, img2, result);
}

int main() {
	mytestsift();
	return 0;
}