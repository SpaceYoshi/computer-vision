#include <opencv2/opencv.hpp>

using namespace cv;
using namespace std;

int main() {
    string path = "Resources/lynx-snow.png";
    Mat img = imread(path);
    Mat imgIdentity, imgSharpening, imgEmboss;

    if (img.empty()) {
        cerr << "Image could not be loaded!" << endl;
        return -1;
    }

    // Identity Kernel
    Mat kernelIdentity = (Mat_<float>(3, 3) <<
        0, 0, 0,
        0, 1, 0,
        0, 0, 0);

    // Sharpening Kernel
    Mat kernelSharpening = (Mat_<float>(3, 3) <<
        0, -1, 0,
		-1, 5, -1,
		0, -1, 0);

    // Emboss Kernel
    Mat kernelEmboss = (Mat_<float>(3, 3) <<
        -2, -1, 0,
        -1, 1, 1,
        0, 1, 2);

    // Apply the custom filters
    filter2D(img, imgIdentity, -1, kernelIdentity);
    filter2D(img, imgSharpening, -1, kernelSharpening);
    filter2D(img, imgEmboss, -1, kernelEmboss);

    imshow("Original Image", img);
    imshow("Identity Kernel", imgIdentity);
    imshow("Sharpening Kernel", imgSharpening);
    imshow("Emboss Kernel", imgEmboss);
    waitKey(0);

    return 0;
}
