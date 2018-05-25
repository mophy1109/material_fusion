/*
 * @Author: USTB.mophy1109
 * @Date: 2018-04-08 14:07:21
 * @Last Modified by: USTB.mophy1109
 * @Last Modified time: 2018-04-08 16:41:04
 */

#ifndef CUTIL_H
#define CUTIL_H

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/xfeatures2d.hpp>
#include <string>
#include <vector>

typedef struct Offset { int x, y; } Offset;
enum CalMethod {
	AVERAGE,
	HOMO,
	AFFINE,
	PROSAC,
	// OTHERS....

};

std::vector<std::string> GetAllFiles(std::string path, std::string suffix);
int ProcessWithSampling(char *src);
int ProcessWithoutSampling(char *dir);
#endif