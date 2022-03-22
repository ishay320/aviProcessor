#include <opencv2/aruco.hpp>
#include <opencv2/calib3d.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/opencv.hpp>

#include <chrono>
#include <iostream>
#include <signal.h>
#include <string>
#include <thread>
#include <unistd.h>
#include <vector>

static bool readCameraParameters(std::string filename, cv::Mat &camMatrix, cv::Mat &distCoeffs) {
    cv::FileStorage fs(filename, cv::FileStorage::READ);
    if (!fs.isOpened()) {
        return false;
    }
    fs["camera_matrix"] >> camMatrix;
    fs["distortion_coefficients"] >> distCoeffs;
    return true;
}

int main(int argc, char const *argv[]) {
    cv::VideoCapture cap;
    bool opened        = false;
    std::string Width  = "1280";
    std::string Height = "720";
    opened             = cap.open("nvarguscamerasrc ! video/x-raw(memory:NVMM), width=" + Width + ", height=" + Height +
                                      ", format=NV12, framerate=(fraction)60/1 ! nvvidconv ! video/x-raw, format=(string)BGRx ! "
                                                  "videoconvert ! appsink sync=false async=true",
                                  cv::CAP_GSTREAMER);
    if (!opened) {
        std::cerr << "failed to open video input: " << std::endl;
        return 1;
    }
    cv::Ptr<cv::aruco::Dictionary> dictionary             = cv::aruco::getPredefinedDictionary(cv::aruco::PREDEFINED_DICTIONARY_NAME(8));
    cv::Ptr<cv::aruco::DetectorParameters> detectorParams = cv::aruco::DetectorParameters::create();

    // create board object
    cv::Ptr<cv::aruco::GridBoard> gridboard = cv::aruco::GridBoard::create(5, 7, 0.033, 0.003, dictionary);
    cv::Ptr<cv::aruco::Board> board         = gridboard.staticCast<cv::aruco::Board>();

    // ******************************
    cv::Mat cameraMatrix, distCoeffs;
    // You can read camera parameters from tutorial_camera_params.yml
    readCameraParameters("./test.json", cameraMatrix, distCoeffs); // This function is located in detect_markers.cpp

    // ******************************
    bool showBoard = false;
    while (cap.grab()) {
        cv::Mat image, imageCopy;
        cap.retrieve(image);

        std::vector<int> ids;
        std::vector<std::vector<cv::Point2f>> corners, rejected;

        // detect markers
        cv::aruco::detectMarkers(image, dictionary, corners, ids, detectorParams, rejected);

        // refind strategy to detect more markers
        if (false) {
            cv::aruco::refineDetectedMarkers(image, board, corners, ids, rejected);
        }

        // ******************************
        // draw results
        image.copyTo(imageCopy);

        if (ids.size() > 0) {
            cv::aruco::drawDetectedMarkers(imageCopy, corners, ids);
            std::vector<cv::Vec3d> rvecs, tvecs;
            if (showBoard) {
                cv::aruco::estimatePoseBoard(corners, ids, board, cameraMatrix, distCoeffs, rvecs, tvecs);
            } else {
                cv::aruco::estimatePoseSingleMarkers(corners, 0.05, cameraMatrix, distCoeffs, rvecs, tvecs);
            }
            // draw axis for each marker
            for (std::size_t i = 0; i < ids.size(); i++)
                cv::aruco::drawAxis(imageCopy, cameraMatrix, distCoeffs, rvecs[i], tvecs[i], 0.1);
        }
        cv::putText(imageCopy, "Press 'ESC' to exit", cv::Point(10, 20), cv::FONT_HERSHEY_SIMPLEX, 0.5, cv::Scalar(255, 0, 0), 2);
        // ******************************

        cv::imshow("out", imageCopy);
        char key = (char)cv::waitKey(27);
        if (key == 27) {
            break;
        }
        if (key == ' ') {
            showBoard = !showBoard;
        }
    }

    return 0;
}
