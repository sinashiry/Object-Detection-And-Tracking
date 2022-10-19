#include "core.h"
#include "opencv2/core/core.hpp"
#include<stdio.h>
//#include<hls_video.h>
#include<hls_opencv.h>

#define INPUT_IMAGE_ADDR	"C:\\Users\\SINA\\Desktop\\Test.jpg"
#define OUTPUT_IMAGE_ADDR	"C:\\Users\\SINA\\Desktop\\img.jpg"
char outImage[720][1280];

void saveImage(const std::string path,cv::InputArray inArr)
{
	double min;
	double max;
	cv::minMaxIdx(inArr,&min,&max);
	cv::Mat adj;
	cv::convertScaleAbs(inArr,adj,255/max);
	cv::imwrite(path,inArr);
}

int main()
{
	printf("Loading Image:.:.\n");
	//INput image
	cv::Mat inImg=cv::imread(INPUT_IMAGE_ADDR);
	//Convert image to GRAYSCALE
	//cv::cvtColor(inImg,inImg,CV_BGR2GRAY);
	printf("rows:%d cols:%d\n",inImg.rows,inImg.cols);

	//Declare in out ports
	hls::stream<uint_32_ch> inputstream;
	hls::stream<uint_32_ch> outputstream;

	//OpenCV mat that point to a array(cv::Size(width,height))
	cv::Mat imgCvout(cv::Size(inImg.cols,inImg.rows),CV_8UC3,outImage,cv::Mat::AUTO_STEP);

	cvMat2AXIvideo(inImg,inputstream);
	unsigned long int r=find(inputstream,outputstream,720,1280,45,255,100,255,0,95,0,143,1);
	AXIvideo2cvMat(outputstream,imgCvout);
	//printf("Center Location=\n%d",r);
	//printf("Center Location=\nX=%d, Y=%d\n",X,Y);
	saveImage(std::string(OUTPUT_IMAGE_ADDR),imgCvout);
	return 0;
}
