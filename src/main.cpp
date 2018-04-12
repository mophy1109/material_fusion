/*
 * @Author: USTB.MophyChen
 * @Date: 2018-03-29 16:30:25
 * @Last Modified by: USTB.mophy1109
 * @Last Modified time: 2018-04-04 18:10:51
 */

#include "CUtil.h"
#include "Preprocess.h"
#include "highgui.h"
#include <algorithm>
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
	for (int i = 0; i < files.size(); i++) {
	}
}

void ProcessWithoutSamping(char *dir) {
	vector<string> files = GetAllFiles(dir, ".*(jpg|bmp|jpeg)$");
	sort(files.begin(), files.end());
	cout << "==========start program=========" << endl;
	// for (int i = 0; i < files.size(); i++) {
	// 	cout << files[i] << endl;
	// }
	// char *src = "/media/cwh1001/Workspace/CUDA_projects/material_fusion/data/result/";
}

int main() {
	// ProcessWithSamping();
	char *src = "/media/cwh1001/Workspace/CUDA_projects/material_fusion/data/result/";

	ProcessWithoutSamping(src);
	return 0;
}