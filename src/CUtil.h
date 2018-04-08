/*
 * @Author: USTB.mophy1109
 * @Date: 2018-04-08 14:07:21
 * @Last Modified by: USTB.mophy1109
 * @Last Modified time: 2018-04-08 16:41:04
 */

#ifndef CUTIL_H
#define CUTIL_H

#endif

typedef struct Offset { float x, y; } Offset;
enum CalMethod {
	AVERAGE,
	MODE,
	HOMO,
	RANSAC,
	// OTHERS....

};