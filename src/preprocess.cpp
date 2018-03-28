#include <opencv2/core/core.hpp>  
#include <opencv2/highgui/highgui.hpp>  
#include <opencv2/imgproc/imgproc.hpp>  
#include <iostream>  
#include "preprocess.h"

using namespace std;  
using namespace cv;  

void sampling(string src, string tar_dir){

    //打开视频
    VideoCapture capture(src);

    //检测是否正常打开:成功打开时，isOpened返回ture  
    if(!capture.isOpened())  
        cout<<"fail to open!"<<endl;

    //获取帧数
    long totalFrameNumber = capture.get(CV_CAP_PROP_FRAME_COUNT);  
    cout<<"Num of Frames:"<<totalFrameNumber<<" Frames"<<endl;
    //获取帧率
    double rate = capture.get(CV_CAP_PROP_FPS);  
    cout<<"Frame Rate: "<<rate<<"fps"<<endl;
    //获取分辨率
    int width = (int)capture.get(CV_CAP_PROP_FRAME_WIDTH);
    int height = (int)capture.get(CV_CAP_PROP_FRAME_HEIGHT);
    cout<<"Resolution: "<<width<<"*"<<height<<endl;

    Mat frameImg;//存储单帧图像
    Mat roi_img;
    Rect rect(0, 0, width, height - 2);//由于源视频地步出现黑边，需要裁剪2px宽度，roi设置为width×（height-1）的图像

    long i = 0; //采样序号

    for (long f = 0; f < totalFrameNumber; f++)
    {
        //读取下一帧
        if(!capture.read(frameImg))
        {
            cout<<"读取视频失败"<<endl;
            return -1;
        }

        //设置采样间隔,当前设置采样率为3fps
        if (f % int(fps/3) != 0){
            continue;
        }
       
        frameImg(rect).copyto(roi_img);//截取图像

        string tmp_filename;
        sprintf(tmp_filename, "sam", "%05d", i,"jpg");//统一文件名格式

        string tar_path = tar_dir + tmp_filename;

        //调试输出
        cout<< tar_path <<endl;

        imwrite(tmp_filename, roi_img);

        cout<<"Reading "<<currentFrame<<" frame"<<endl;

        currentFrame++;
    }  
}
