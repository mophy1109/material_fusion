/*
 * @Author: USTB.mophy1109
 * @Date: 2018-05-18 14:49:13
 * @Last Modified by: USTB.mophy1109
 * @Last Modified time: 2018-05-23 14:49:31
 */

#include <opencv2/cudaimgproc.hpp>
#include <opencv2/core/cuda.hpp>
#include <opencv2/core/utility.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/core/cuda_stream_accessor.hpp>
#include <opencv2/opencv.hpp>
#include <iostream>

using namespace cv;
using namespace std;
using namespace cv::cuda;

void calculateSF_caller(const PtrStepSz<uchar> &img1, const PtrStepSz<uchar> &img2, PtrStepSz<uchar> result, cudaStream_t stream);

void calculateSF(const GpuMat &img1, const GpuMat &img2, GpuMat &result, Stream &stream = Stream::Null()) {
	CV_Assert(img1.type() == CV_8UC1);
	result.create(img1.rows / 4, img1.cols / 4, img1.type());
	cudaStream_t s = StreamAccessor::getStream(stream);
	calculateSF_caller(img1, img2, result, s);
}

Mat getSFMatrix(Mat roi1, Mat roi2) {
	// roi1 and roi2 should be gray sclase images of the same region
	// getSFMatrix return a 0-1 Matrix, where 1 means roi1 is clear than roi2, 0 means the other way round
	// cout << roi1.rows << " , " << roi1.cols << endl;
	Mat dst_img = Mat::zeros(roi1.rows / 4, roi1.cols / 4, roi1.type()); // save result

	GpuMat d_img1(roi1);
	GpuMat d_img2(roi2);
	GpuMat bli_img, res_img;
	// res_img.create(d_img1.size(), d_img1.type());

	calculateSF(d_img1, d_img2, bli_img);
	cuda::bilateralFilter(bli_img, bli_img, 30, 10, 7);
	cuda::resize(bli_img, res_img, Size(roi1.cols, roi1.rows), 0, 0, INTER_NEAREST);
	res_img.download(dst_img);
	return dst_img;
}