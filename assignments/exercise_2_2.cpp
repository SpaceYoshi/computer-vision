#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <iostream>

using namespace cv;
using namespace std;

int main() {
    const string path = "resources/lynx-snow-2.png";
    const Mat img = imread(path);
    Mat imgGray, imgBlur, imgCanny;

    cvtColor(img, imgGray, COLOR_BGR2GRAY);
    GaussianBlur(imgGray, imgBlur, Size(7, 7), 5, 0);

    int threshold1 = 0, threshold2 = 0;

    namedWindow("Trackbars", (1024, 200));
    createTrackbar("Threshold 1", "Trackbars", &threshold1, 512);
    createTrackbar("Threshold 2", "Trackbars", &threshold2, 512);

    while (true) {
        Canny(imgBlur, imgCanny, threshold1, threshold2);

        imshow("Image", img);
        imshow("Image Gray", imgGray);
        imshow("Image Blur", imgBlur);
        imshow("Image Canny", imgCanny);
        waitKey(1);
    }
}
