#include <opencv2/opencv.hpp>

#include <chrono>
#include <iostream>
#include <signal.h>
#include <string>
#include <thread>
#include <unistd.h>

#include "parser.hpp"

/*
1. get file or camera input, and file output - parser
2. show camera or video(pictures?)
3. real time capture pics - and show aruco on out
4. calibrate base on what took
5. profit ;)
*/


bool there_is_work = true;

void signal_callback(int signum) {
    std::cout << "\nexiting gracefully\n";
    there_is_work = false;
}

int main(int argc, char const *argv[]) {
    std::string input_file;
    std::string output_file;
    bool show;
    bool camera;
    int err = parser(argc, argv, input_file, output_file, show, camera);
    if (err == 1) {
        usage(std::cout, argv[0]);
        return 1;
    }

    signal(SIGINT, signal_callback);

    // create a VideoCapture object and open the input file
    cv::VideoCapture cap;
    if (camera) {
        cap.open(-1);
    } else {
        cap.open(input_file);
    }
    // check if opened successfully
    if (!cap.isOpened()) {
        std::cerr << "Error opening video stream or file" << std::endl;
        return 1;
    }

    while (there_is_work) {
        cv::Mat frame;
        // capture frame-by-frame
        cap >> frame;

        // if the frame is empty, the file ended
        if (frame.empty())
            break;

        // press ESC on keyboard to exit
        char c = (char)cv::waitKey(ESC);
        if (c == 27)
            break;
    }

    // release the video capture objects
    cap.release();

    // closes frame
    cv::destroyAllWindows();

    return 0;
}
