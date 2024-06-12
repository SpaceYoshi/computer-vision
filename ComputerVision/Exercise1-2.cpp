#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <iostream>

using namespace cv;
using namespace std;

void main() {
	string path = "Resources/er-x-ds.png";
	Mat img = imread(path);
	Mat imgResize, imgCrop;

	resize(img, imgResize, Size(), 0.5, 0.5);

	Rect roi(150, 275, 200, 200);
	imgCrop = img(roi);

	imshow("Image", img);
	imshow("Image Resize", imgResize);
	imshow("Image Crop", imgCrop);
	waitKey(0);
}
