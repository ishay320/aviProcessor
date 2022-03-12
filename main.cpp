#include <opencv2/opencv.hpp>

#include <chrono>
#include <iostream>
#include <signal.h>
#include <string>
#include <thread>
#include <unistd.h>

#include "AviProccesor.hpp"
#include "parser.hpp"

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
        cap.open(0);
    } else {
        cap.open(input_file);
    }
    // check if opened successfully
    if (!cap.isOpened()) {
        std::cerr << "Error opening video stream or file" << std::endl;
        return 1;
    }

    // get resolutions of the input video
    int frame_width = cap.get(cv::CAP_PROP_FRAME_WIDTH);
    int frame_height = cap.get(cv::CAP_PROP_FRAME_HEIGHT);
    double fps = cap.get(cv::CAP_PROP_FPS);

    // define the codec and create VideoWriter object
    cv::VideoWriter video;
    if (!output_file.empty()) {
        video = cv::VideoWriter{output_file, cv::VideoWriter::fourcc('a', 'v', 'c', '1'), fps, cv::Size(frame_width, frame_height)};

        if (!video.isOpened()) {
            std::cerr << "Error opening video stream or file" << std::endl;
            return 1;
        }
    }

    // start the processing thread and his queue
    FrameQueue frame_queue;
    std::thread thread_frames(frameBufferManager, std::ref(frame_queue), std::ref(video), show);

    auto timer_last = std::chrono::high_resolution_clock::now();
    while (there_is_work) {
        auto timer_now = std::chrono::high_resolution_clock::now();
        std::chrono::microseconds delta = std::chrono::duration_cast<std::chrono::microseconds>(timer_now - timer_last);
        if (delta.count() / 1000 < 40) { // wait the time that left
            usleep(40 * 1000 - delta.count());
        }
#ifdef DEBUG
        std::cout << "stream loop took: " << ((double)delta.count()) / 1000 << " waited: " << (40 * 1000 - delta.count()) / 1000 << "\n";
#endif
        timer_last = std::chrono::high_resolution_clock::now();

        cv::Mat frame;
        // capture frame-by-frame
        cap >> frame;

        // if the frame is empty, the file ended
        if (frame.empty())
            break;

        frame_queue.pushWork(frame);

        // press ESC on keyboard to exit
        char c = (char)cv::waitKey(ESC);
        if (c == 27)
            break;
    }

    // release the video capture objects
    cap.release();

    frame_queue.workDone();
    thread_frames.join();
    video.release();

    // closes frame
    cv::destroyAllWindows();

    return 0;
}
