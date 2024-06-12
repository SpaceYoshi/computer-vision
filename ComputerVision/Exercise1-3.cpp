#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <iostream>

using namespace cv;
using namespace std;

void main() {
	Mat img(512, 700, CV_8UC3, Scalar(255, 255, 255)); // Scalar(B, G, R), OpenCV uses BGR

	circle(img, Point(350, 256), 150, Scalar(0, 0, 255), FILLED);
	
	Scalar lineColor = Scalar(0, 0, 0);
	int lineThickness = 10;
	line(img, Point(0, 0), Point(700, 512), lineColor, lineThickness);
	line(img, Point(0, 512), Point(700, 0), lineColor, lineThickness);

	rectangle(img, Point(250, 200), Point(450, 300), Scalar(0, 0, 0), FILLED);
	putText(img, "OpenCV", Point(260, 265), FONT_HERSHEY_DUPLEX, 1.5, Scalar(255, 255, 255), 3);

	imshow("Image", img);
	waitKey(0);
}
