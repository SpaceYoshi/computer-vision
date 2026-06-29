#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <iostream>

using namespace cv;
using namespace std;

int main() {
	const string path = "resources/sao-p-manga.png";
	Mat img = imread(path);
	Mat imgWarp;
	float w = 500, h = 700;

	const Point2f src[4] = { {463,358},{729,491},{39,540},{304,822} };
	const Point2f dst[4] = { {0.0f,0.0f},{w,0.0f},{0.0f,h},{w,h} };

	const Mat matrix = getPerspectiveTransform(src, dst);
	warpPerspective(img, imgWarp, matrix, Size(static_cast<int>(w), static_cast<int>(h)));

	// Mark corners of the original image
	for (int i = 0; i < 4; i++)
	{
		circle(img, src[i], 10, Scalar(0, 0, 255), FILLED);
	}

	imshow("Image", img);
	imshow("Image Warp", imgWarp);
	waitKey(0);
	return 0;
}
