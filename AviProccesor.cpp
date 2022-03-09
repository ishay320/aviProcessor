#include <opencv2/aruco.hpp>
#include <opencv2/opencv.hpp>

#include <condition_variable>
#include <mutex>
#include <queue>
#include <thread>
#include <unistd.h>

#include "AviProccesor.hpp"

#define ESC 25

void FrameQueue::workDone() {
    pushWork(cv::Mat()); // add an empty frame
}

void FrameQueue::pushWork(cv::Mat frame) {
    std::unique_lock<std::mutex> lock(work_mutex); // overkill but fun

    bool was_empty = frames.empty();
    frames.push(frame);

    lock.unlock();

    if (was_empty) {
        work_available.notify_one();
    }
}

cv::Mat FrameQueue::waitAndPop() {
    std::unique_lock<std::mutex> lock(work_mutex);
    while (frames.empty()) {
        work_available.wait(lock);
    }

    cv::Mat tmp = frames.front();
    frames.pop();
    return tmp;
}

double randomWait() {
    double r = ((double)rand() / (RAND_MAX));
    usleep(r * 60 * 1000); // sleep for a random time in range of 0-60 ms
    return (r * 60 * 1000);
}

cv::Mat process(const cv::Mat &frame) {
    cv::Mat copy_frame;
    frame.copyTo(copy_frame);

    std::vector<int> markerIds;
    std::vector<std::vector<cv::Point2f>> markerCorners, rejectedCandidates;
    cv::Ptr<cv::aruco::DetectorParameters> parameters = cv::aruco::DetectorParameters::create();
    cv::Ptr<cv::aruco::Dictionary> dictionary = cv::aruco::getPredefinedDictionary(cv::aruco::DICT_6X6_250);
    cv::aruco::detectMarkers(copy_frame, dictionary, markerCorners, markerIds, parameters, rejectedCandidates);
    cv::aruco::drawDetectedMarkers(copy_frame, markerCorners, markerIds);
    randomWait();

    return copy_frame;
}

void frameBufferManager(FrameQueue &frame_queue, cv::VideoWriter &video, bool show) {
    while (true) {
        cv::Mat frame = frame_queue.waitAndPop();
        if (frame.empty()) // check for empty frames - end of work
            break;

        frame = process(frame);

        if (show) // display the resulting frame
            imshow("Frame", frame);

        // write the frame to file
        video.write(frame);
    }
}