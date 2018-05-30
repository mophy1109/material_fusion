/*
 * @Author: USTB.mophy1109
 * @Date: 2018-05-02 11:30:14
 * @Last Modified by: USTB.mophy1109
 * @Last Modified time: 2018-05-29 11:23:20
 */

#include <opencv2/core/cuda.hpp>
#include <cuda_runtime.h>

using namespace std;
using namespace cv;
using namespace cv::cuda;

//define the image block size to calculate SF,
#define SUB_BLOCK_WIDTH 4
#define SUB_BLOCK_HEIGHT 4

__global__ void calSF(
    const PtrStepSz<uchar> img1,
    const PtrStepSz<uchar> img2,
    PtrStepSz<uchar> result)
    {
    //calculate Spatial Frequency of images and return the weight-matrix

    __shared__ int RF1;//block shared memory for save RF1 in a block
    __shared__ int CF1;//block shared memory for save CF1 in a block
    __shared__ int RF2;//block shared memory for save RF2 in a block
    __shared__ int CF2;//block shared memory for save CF2 in a block

    RF1 = CF1 = RF2 = CF2 = 0;
    __syncthreads();

    /*
      Cause img1 and img2 have the same block size,
      only need to calculate CF^2 and RF^2 to compare.
      so RF & CF here are actually RF^2 and CF^2
    */

    int x = threadIdx.x + blockIdx.x * blockDim.x;
    int y = threadIdx.y + blockIdx.y * blockDim.y;

    if (x < img1.cols && y < img1.rows)
    {   
        //using aromic ops to avoid conflict reading-modification-writing ops
        if (y > 0){
            atomicAdd(&CF1, (img1(y,x) - img1(y-1,x))*(img1(y,x) - img1(y-1,x)));
            atomicAdd(&CF2, (img2(y,x) - img2(y-1,x))*(img2(y,x) - img2(y-1,x)));
        }
        if (x > 0){
            atomicAdd(&RF1, (img1(y,x) - img1(y,x-1))*(img1(y,x) - img1(y,x-1)));
            atomicAdd(&RF2, (img2(y,x) - img2(y,x-1))*(img2(y,x) - img2(y,x-1)));
        }
        __syncthreads();

        //generate weight_matrix, no need to use atomic ops cause thread in the same block get the same result
        if (RF1 + CF1 > RF2 + CF2){
            result(y/SUB_BLOCK_HEIGHT, x/SUB_BLOCK_WIDTH) = 1;
        }else{
            result(y/SUB_BLOCK_HEIGHT, x/SUB_BLOCK_WIDTH) = 0;
        }
    }
}

void calculateSF_caller(const PtrStepSz<uchar> &img1, const PtrStepSz<uchar> &img2, PtrStepSz<uchar> result, cudaStream_t stream)
{
    // define the block size
    dim3 block(SUB_BLOCK_WIDTH,SUB_BLOCK_HEIGHT);

    // define the grid size, which is dynamically calculated with the size of img
    dim3 grid((img1.cols + block.x - 1)/block.x,(img1.rows + block.y - 1)/block.y);

    calSF<<<grid, block>>>(img1, img2, result);//get RF&CF of img1 and img2
    cudaDeviceSynchronize();
}