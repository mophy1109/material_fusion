#include <opencv2/core/cuda.hpp>
#include <opencv2/core/cuda_stream_accessor.hpp>
#include <opencv2/opencv.hpp>

using namespace cv;
using namespace cv::cuda;

void calculateSF_caller(const PtrStepSz<uchar> &img1, const PtrStepSz<uchar> &img2, PtrStepSz<uchar> result, cudaStream_t stream);

void calculateSF(const GpuMat &img1, const GpuMat &img2, GpuMat &result, Stream &stream = Stream::Null()) {
	CV_Assert(img1.type() == CV_8UC1);
	result.create(img1.rows / 4, img1.cols / 4, img1.type());
	cudaStream_t s = StreamAccessor::getStream(stream);
	calculateSF_caller(img1, img2, result, s);
}