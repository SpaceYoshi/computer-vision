#include <opencv2/highgui.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/objdetect.hpp>
#include <opencv2/videoio.hpp>

#include <algorithm>
#include <filesystem>
#include <iostream>
#include <string>
#include <vector>

namespace fs = std::filesystem;

namespace {

constexpr int escapeKey = 27;

std::vector<cv::Rect> detectCharacters(const cv::Mat& plateImage) {
    cv::Mat grayImage;
    cv::Mat blurredImage;
    cv::Mat thresholdImage;
    cv::Mat morphologyImage;

    cv::cvtColor(plateImage, grayImage, cv::COLOR_BGR2GRAY);
    cv::GaussianBlur(grayImage, blurredImage, cv::Size{3, 3}, 0);
    cv::adaptiveThreshold(blurredImage, thresholdImage, 255,
                          cv::ADAPTIVE_THRESH_GAUSSIAN_C,
                          cv::THRESH_BINARY_INV, 11, 10);

    const cv::Mat kernel = cv::getStructuringElement(cv::MORPH_RECT, cv::Size{3, 3});
    cv::morphologyEx(thresholdImage, morphologyImage, cv::MORPH_CLOSE, kernel);

    std::vector<std::vector<cv::Point>> contours;
    cv::findContours(morphologyImage, contours, cv::RETR_EXTERNAL,
                     cv::CHAIN_APPROX_SIMPLE);

    std::vector<cv::Rect> characterBoxes;
    for (const auto& contour : contours) {
        const cv::Rect box = cv::boundingRect(contour);
        if (box.height > 20 && box.width > 10 && box.height > box.width) {
            characterBoxes.push_back(box);
        }
    }

    std::ranges::sort(characterBoxes, {}, &cv::Rect::x);
    return characterBoxes;
}

fs::path executableDirectory(const char* executable) {
    std::error_code error;
    const auto path = fs::absolute(executable, error);
    return error ? fs::current_path() : path.parent_path();
}

} // namespace

int main(int argc, char* argv[]) {
    const fs::path cascadePath = argc > 1
        ? fs::path{argv[1]}
        : executableDirectory(argv[0]) / "resources" /
              "haarcascade_russian_plate_number.xml";

    cv::CascadeClassifier plateCascade;
    plateCascade.load(cascadePath.string());
    if (plateCascade.empty()) {
        std::cerr << "Could not load cascade file: " << cascadePath << '\n';
        return 1;
    }

    cv::VideoCapture camera{0};
    if (!camera.isOpened()) {
        std::cerr << "Could not open camera 0.\n";
        return 1;
    }

    const fs::path outputDirectory = fs::current_path() / "plates";
    std::error_code error;
    fs::create_directories(outputDirectory, error);
    if (error) {
        std::cerr << "Could not create output directory " << outputDirectory
                  << ": " << error.message() << '\n';
        return 1;
    }

    while (true) {
        cv::Mat image;
        if (!camera.read(image) || image.empty()) {
            std::cerr << "Could not read a frame from the camera.\n";
            break;
        }

        std::vector<cv::Rect> plates;
        plateCascade.detectMultiScale(image, plates, 1.1, 10);

        for (std::size_t plateIndex = 0; plateIndex < plates.size(); ++plateIndex) {
            const cv::Rect& plate = plates[plateIndex];
            cv::Mat plateImage = image(plate);

            cv::rectangle(image, plate, cv::Scalar{255, 0, 0}, 3);
            cv::imwrite((outputDirectory /
                         ("plate-" + std::to_string(plateIndex) + ".png")).string(),
                        plateImage);

            const auto characterBoxes = detectCharacters(plateImage);
            for (std::size_t characterIndex = 0;
                 characterIndex < characterBoxes.size(); ++characterIndex) {
                const cv::Rect& characterBox = characterBoxes[characterIndex];
                const cv::Mat characterImage = plateImage(characterBox);

                const auto filename = "plate-" + std::to_string(plateIndex) +
                                      "-char-" + std::to_string(characterIndex) +
                                      ".png";
                cv::imwrite((outputDirectory / filename).string(), characterImage);

                cv::rectangle(plateImage, characterBox, cv::Scalar{255, 0, 255}, 3);
            }
        }

        cv::imshow("Capture", image);
        if (cv::waitKey(30) == escapeKey) {
            break;
        }
    }

    cv::destroyAllWindows();
    return 0;
}
