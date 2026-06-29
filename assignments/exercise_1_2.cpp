#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>

using namespace cv;
using namespace std;

int main() {
    const string path = "resources/er-x-ds.png";
    const Mat img = imread(path);
    Mat imgResize;

    resize(img, imgResize, Size(), 0.5, 0.5);

    const Rect roi(150, 275, 200, 200); // Knight starts at (150, 275) and is 200x200
    const Mat imgCrop = img(roi);

    imshow("Image", img);
    imshow("Image Resize", imgResize);
    imshow("Image Crop", imgCrop);
    waitKey(0);
    return 0;
}
