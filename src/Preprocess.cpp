/*
 * @Author: USTB.mophy1109
 * @Date: 2018-04-02 11:13:46
 * @Last Modified by: USTB.mophy1109
 * @Last Modified time: 2018-04-24 09:25:49
 */

#include <iostream>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

using namespace std;
using namespace cv;

void Sampling(char *src, char *tar_dir) {

	//打开视频
	VideoCapture capture(src);

	//检测是否正常打开:成功打开时，isOpened返回ture
	if (!capture.isOpened())
		cout << "fail to open!" << endl;

	//获取帧数
	long totalFrameNumber = capture.get(CV_CAP_PROP_FRAME_COUNT);
	cout << "Num of Frames:" << totalFrameNumber << " Frames" << endl;

	//获取帧率
	double rate = capture.get(CV_CAP_PROP_FPS);
	cout << "Frame Rate: " << rate << "fps" << endl;

	//获取分辨率
	int width = (int)capture.get(CV_CAP_PROP_FRAME_WIDTH);
	int height = (int)capture.get(CV_CAP_PROP_FRAME_HEIGHT);
	cout << "Resolution: " << width << "*" << height << endl;
	cout << "Sampling video...." << endl;

	Mat frameImg; //存储单帧图像
	Mat roi_img;
	Rect rect(0, 0, width, height - 2); //由于源视频底部出现黑边，需要裁剪2px宽度，roi设置为width×（height-2）的图像

	int i = 0; //采样序号

	for (int currentFrame = 0; currentFrame < totalFrameNumber; currentFrame++) {

		//读取帧
		if (!capture.read(frameImg)) {
			cout << "读取视频失败" << endl;
			return;
		}

		//设置采样间隔,当前设置采样率为3fps
		if (currentFrame % int(rate / 3) != 0) {
			continue;
		}

		frameImg(rect).copyTo(roi_img); //截取图像

		char tmp_filename[20];
		sprintf(tmp_filename, "%05d.jpg", i);

		string filename(tmp_filename);
		string tar_path = tar_dir + filename;

		//调试输出
		// cout<< tar_path <<endl;
		imwrite(tar_path, roi_img);
		i++;
	}
}
