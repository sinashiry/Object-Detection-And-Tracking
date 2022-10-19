#include <iostream>
#include <string>
#include <sstream>
using namespace std;

//OpenCV Header Files
#include "opencv2/core/core.hpp"
#include "opencv2/highgui.hpp"
using namespace cv;


//OpenCV command line parser functions
//Keys accepted by command line parser
int main(int argc, const char** argv)
{
	//Timer
	double start_time, finish_time, start_total_time, finish_total_time;
	start_time = getTickCount();
	///////

	//Read Image/Video from file
	//Mat frame=imread("Test.jpg");
	VideoCapture cap("Test.mp4");
	Mat frame;
	Vec3b pixel;
	////////////////////////////
	int counter = 0;
	bool finish = false;
	while (finish = true)
	{
		counter++;
		cap >> frame;
		for (int ir = 0; ir < 1280; ir++)
		{
			for (int ic = 0; ic < 720; ic++)
			{
				pixel = frame.at<Vec3b>(Point(ir, ic));
				pixel[2] = pixel[2];
				pixel[1] = pixel[1];
				pixel[0] = pixel[0];
				int D = pixel[1] - pixel[0] / 2 - pixel[2] / 2;
				if (D>45)
				{
					if (pixel[0] >= 0 & pixel[0] <= 95 & pixel[1] >= 100 & pixel[1] <= 255 & pixel[2] >= 0 & pixel[2] <= 143)
					{
						pixel[2] = 0;
						pixel[1] = 0;
						pixel[0] = 0;
						frame.at<Vec3b>(Point(ir, ic)) = pixel;
					}
				}
				

			}

		}
		//cout << counter <<"\n";
		//imshow("img", frame);
		//imwrite("img.jpg", frame);
		waitKey(1);
		if (counter == 200) break;
	}
	finish_time = getTickCount();
	cout << "Time per frame: " << (finish_time - start_time) / getTickFrequency() << "secs" << endl;
	waitKey(30000);
	while(1)
	{ }
	return 0;
}