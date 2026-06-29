#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <iostream>

using namespace cv;
using namespace std;

int main() {
	const string path = "resources/ball-stripes.png";
	const Mat img = imread(path);
	Mat imgDilate, imgErode;

	const Mat kernel = getStructuringElement(MORPH_RECT, Size(29, 29));
	dilate(img, imgDilate, kernel);
	erode(imgDilate, imgErode, kernel);

	imshow("Image", img);
	imshow("After Dilation", imgDilate);
	imshow("After Erosion", imgErode);
	waitKey(0);
	return 0;
}
