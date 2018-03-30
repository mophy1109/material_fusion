/*
 * @Author: USTB.MophyChen
 * @Date: 2018-03-29 16:30:25
 * @Last Modified by: USTB.MophyChen
 * @Last Modified time: 2018-03-30 14:20:09
 */

#include "CFusion.h"
#include "Preprocess.h"
#include "cv.h"
#include "highgui.h"
#include <dirent.h>
#include <iostream>
#include <string>

using namespace std;
using namespace cv;

void ProcessWithSamping() {
	char *src = "/media/cwh1001/Workspace/CUDA_projects/material_fusion/data/videos/500px.avi";
	char *tar_dir = "/media/cwh1001/Workspace/CUDA_projects/material_fusion/data/result/";

	// Sampling(src, tar_dir);

	vector<string> files = GetAllFiles(tar_dir, ".*(jpg|bmp|jpeg)$");
	cout << "==========start program=========" << endl;
	for (int i = 0; i < files.size(); i++) {
		cout << files[i] << endl;
	}
}

void ProcessWithoutSamping() {
	vector<string> files;
	char *src = "/media/cwh1001/Workspace/CUDA_projects/material_fusion/data/videos/500px.avi";
	char *tar_dir = "/media/cwh1001/Workspace/CUDA_projects/material_fusion/data/result/";
}

int main() {
	ProcessWithSamping();
	return 0;
}