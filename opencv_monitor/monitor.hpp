#ifndef _MONITOR_HPP
#define _MONITOR_HPP

// opencv head file
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/video/background_segm.hpp>
#include <opencv2/objdetect/objdetect.hpp>
#include <opencv2/imgproc/imgproc.hpp>

// cpp
#include <ctime>
#include <iostream>
#include <string>
#include <cstdio>
#include <unistd.h>
#include <sstream>

using namespace cv;
using namespace std;

//extern and const value declare
extern Ptr<BackgroundSubtractor> pMOG;		// 背景剔除方法
extern Ptr<BackgroundSubtractor> pMOG2;
extern int keyboard;

const float h_ranges[] = {0, 256};
const float s_ranges[] = {0, 180};
const float* ranges[] = {h_ranges, s_ranges};

const int h_bins = 50, s_bins = 60;
const int histSize[] = {h_bins, s_bins};

extern char recordName[128];

/* 主要的函数，获取和处理数据
 * showWindow: 窗口显示控制
 * method: 背景减除方法
 * unnormal: 当出现与背景图像不同的帧数达到这个阈值时，才开始记录，防止过于敏感
 * fps: 写入视频的帧率
 */
void processCamera(bool showWindow, 
		unsigned int method,
		unsigned int unnormal=10,
		unsigned int fps=24) {
	// 捕获摄像头
	// cvCapture *capture = cvCaptureFromCAM(0);
	CvCapture *capture = cvCreateFileCapture("test.mp4");
	if(!capture) {
		cerr << "Unable to open camera " << endl;
		exit(EXIT_FAILURE);
	}
	
	// 背景帧标识
	bool backGroundFlag = true;  // 初始时为true, 将第一帧设置为背景之后就为false
	// 是否记录当前帧
	bool recordFlag = false;
	// 当前帧图像
	Mat frame;
	// 前景图像
	Mat fgMask;
	// 图像的HSV格式
	Mat HSV;
	// 直方图
	MatND base, cur;
	// unnormal帧数
	unsigned int UnnormalFrames = 0;
	
	int channels[] = {0, 1};
	
	CvSize size = cvSize(
			(int)cvGetCaptureProperty(capture, CV_CAP_PROP_FRAME_WIDTH),
			(int)cvGetCaptureProperty(capture, CV_CAP_PROP_FRAME_HEIGHT)
			);
	
	// 视频写入
	CvVideoWriter *recorder = cvCreateVideoWriter(recordName, CV_FOURCC('D', 'I', 'V', 'X'), 
			32, size, 1);
	
	// ESC or 'q' for quitting
	while((char)keyboard != 'q' && (char)keyboard != 27) {
		if(!capture){
			cerr << "Unable to read camera" << endl;
			cerr << "Exiting...." << endl;
			exit(EXIT_FAILURE);
		}

		frame = cvQueryFrame(capture);

		// 设置背景减除方法, 将原始图像处理后写入fgMask
		if(method == 0)
			pMOG2->operator()(frame, fgMask);	// MOG2方法(指针方法调用)
		else if (method == 1)
			pMOG->operator()(frame, fgMask);	// MOG方法
		else if(method == 2)
			fgMask = frame;						// 不做变换
		
		// 设置背景图像
		if(backGroundFlag){
			// RGB转HSV
			cvtColor(frame, HSV, CV_BGR2HSV);
			// 计算直方图
			calcHist(&HSV, 1, channels, Mat(), base, 2, histSize, ranges, true, true);
			// normalize
			normalize(base, base, 0, 1, NORM_MINMAX, -1, Mat());
			backGroundFlag = false;
		}
		
		// 普通帧的图像
		cvtColor(frame, HSV, CV_BGR2HSV);
		calcHist(&HSV, 1, channels, Mat(), cur, 2, histSize, ranges, true, false);
		normalize(cur, cur, 0, 1, NORM_MINMAX, -1, Mat());
		
		// 比较当前帧和背景图像的直方图
		double comp = compareHist(base, cur, 0); // 越小说明两个图越不相似
		double compThreshold = 0.65; 
		if (comp < compThreshold)	// 说明当前帧有变化
			UnnormalFrames += 1;
		else if(UnnormalFrames > 0) 
			UnnormalFrames--;
		
		if(UnnormalFrames > unnormal) // 如果unnormal的帧数记录达到阈值，则开始记录
			recordFlag = true;
		else if(UnnormalFrames <= 0) {
			UnnormalFrames = 0;
			recordFlag = false;
		}
		
		// 记录当前帧，写入
		if(recordFlag) {
			cvWriteFrame(recorder, &(IplImage(frame)));
		}

		if(showWindow && !fgMask.empty()) {
			imshow("Monitor", fgMask);
		}

		keyboard = waitKey(30);
	}

	// 释放摄像头
	cvReleaseCapture(&capture);
	// 释放记录流
	cvReleaseVideoWriter(&recorder);
}  // end processCamera


# endif // _MONITOR_HPP
