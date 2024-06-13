#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/objdetect.hpp>
#include <iostream>
#include <filesystem>

using namespace cv;
using namespace std;

// Extract characters
vector<Rect> detectCharacters(const Mat& imgPlate) {
    Mat imgGray, imgBlurred, imgThresh, imgMorph;
    vector<Rect> charBoxes;

    cvtColor(imgPlate, imgGray, COLOR_BGR2GRAY);
    GaussianBlur(imgGray, imgBlurred, Size(3, 3), 0);
    adaptiveThreshold(imgBlurred, imgThresh, 255, ADAPTIVE_THRESH_GAUSSIAN_C, THRESH_BINARY_INV, 11, 10);

    // Additional character separation
    Mat kernel = getStructuringElement(MORPH_RECT, Size(3, 3));
    morphologyEx(imgThresh, imgMorph, MORPH_CLOSE, kernel);

    vector<vector<Point>> contours;
    findContours(imgMorph, contours, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);

    // Get bounding boxes
    for (const auto& contour : contours) {
        Rect bbox = boundingRect(contour);
        // Check size of plate
        if (bbox.height > 20 &&
            bbox.width > 10 &&
            bbox.height > bbox.width) {
            charBoxes.push_back(bbox);
        }
    }

    return charBoxes;
}

int main() {
    VideoCapture cap(0);
    if (!cap.isOpened()) {
		cout << "Error: Unable to open the camera." << endl;
		return -1;
	}

    CascadeClassifier plateCascade;
    plateCascade.load("Resources/haarcascade_russian_plate_number.xml");
    if (plateCascade.empty()) {
        cout << "Error: XML file not loaded." << endl;
        return -1;
    }

    Mat img;
    vector<Rect> plates;

    while (true) {
        cap.read(img);
        plateCascade.detectMultiScale(img, plates, 1.1, 10);

        for (int i = 0; i < plates.size(); i++) {
            Mat imgCrop = img(plates[i]);

            // Draw plate box
            rectangle(img, plates[i].tl(), plates[i].br(), Scalar(255, 0, 0), 3);

            filesystem::create_directory("Plates");
            imwrite("Plates/plate-" + to_string(i) + ".png", imgCrop);

            // Get and save characters
            vector<Rect> charBoxes = detectCharacters(imgCrop);

            for (int j = 0; j < charBoxes.size(); j++) {
                Rect charBox = charBoxes[j];
                Mat charImg = imgCrop(charBox);
                imwrite("Plates/plate-" + to_string(i) + "-char-" + to_string(j) + ".png", charImg);

                // Draw char box
                rectangle(imgCrop, charBox.tl(), charBox.br(), Scalar(255, 0, 255), 3);
            }
        }

        // Press escape key to stop
        if (waitKey(1) == 27) {
            break;
        }

        imshow("Capture", img);
        waitKey(100); // Spare hardware resources
    }

    cap.release();
    destroyAllWindows();
    return 0;
}
