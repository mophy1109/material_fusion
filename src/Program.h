/*
 * @Author: USTB.mophy1109
 * @Date: 2018-05-25 11:43:09
 * @Last Modified by: USTB.mophy1109
 * @Last Modified time: 2018-05-25 11:44:54
 */

#ifndef PROGRAM_H
#define PROGRAM_H

#include <opencv2/core/cuda.hpp>
#include <opencv2/core/cuda_stream_accessor.hpp>
#include <opencv2/opencv.hpp>

void calculateSF_caller(const cv::cuda::PtrStepSz<uchar> &img1, const cv::cuda::PtrStepSz<uchar> &img2, cv::cuda::PtrStepSz<uchar> result, cudaStream_t stream);

void calculateSF(const cv::cuda::GpuMat &img1, const cv::cuda::GpuMat &img2, cv::cuda::GpuMat &result, cv::cuda::Stream &stream);

cv::Mat getSFMatrix(cv::Mat roi1, cv::Mat roi2);
#endif