#include <opencv2/opencv.hpp>

#include <chrono>
#include <iostream>
#include <string>
#include <thread>
#include <unistd.h>

#include <condition_variable>
#include <mutex>
#include <queue>

#define ESC 25

class FrameQueue {
    std::condition_variable work_available;
    std::mutex work_mutex;
    std::queue<cv::Mat> frames;
    bool done_work = false;

  public:
    void workDone() {
        done_work = true;
        push_work(cv::Mat());
    }
    void push_work(cv::Mat frame) {
        std::unique_lock<std::mutex> lock(work_mutex);

        bool was_empty = frames.empty();
        frames.push(frame);

        lock.unlock();

        if (was_empty) {
            work_available.notify_one();
        }
    }

    cv::Mat wait_and_pop() {
        std::unique_lock<std::mutex> lock(work_mutex);
        while (frames.empty()) {
            work_available.wait(lock);
        }

        cv::Mat tmp = frames.front();
        frames.pop();
        return tmp;
    }
};

double randomWait() {
    double r = ((double)rand() / (RAND_MAX));
    usleep(r * 60 * 1000); // sleep for a random time in range of 0-60 ms
    return (r * 60 * 1000);
}

void process(cv::Mat &frame, cv::VideoWriter &video, bool show) {
    cv::putText(frame, "Big Buck Bunny", cv::Point(0, 200), cv::FONT_HERSHEY_DUPLEX, 0.7, cv::Scalar(0, 0, 0), 2, false);
    // write the frame to file
    video.write(frame);

    randomWait();

    // TODO: add option to turn on and off maybe -s for show
    if (show)
        // display the resulting frame
        imshow("Frame", frame);
}

void frameBufferManager(FrameQueue &frame_queue, cv::VideoWriter &video, bool show) {
    while (true) {
        cv::Mat frame = frame_queue.wait_and_pop();
        if (frame.empty())
            break;
        process(frame, video, show);
    }
}

void usage(std::ostream &stream, std::string file_name) { stream << "usage: " << file_name << " <input file> <output>\n"; }

int main(int argc, char const *argv[]) {
    if (argc < 3) {
        usage(std::cout, argv[0]);
        return 1;
    }
    std::string input_file = argv[1];
    std::string output_file = argv[2];

    // create a VideoCapture object and open the input file
    cv::VideoCapture cap(input_file);

    // check if opened successfully
    if (!cap.isOpened()) {
        std::cout << "Error opening video stream or file" << std::endl;
        return 1;
    }

    // get resolutions of the input video
    int frame_width = cap.get(cv::CAP_PROP_FRAME_WIDTH);
    int frame_height = cap.get(cv::CAP_PROP_FRAME_HEIGHT);
    double fps = cap.get(cv::CAP_PROP_FPS);

    // define the codec and create VideoWriter object
    cv::VideoWriter video(output_file, cv::VideoWriter::fourcc('a', 'v', 'c', '1'), fps, cv::Size(frame_width, frame_height));

    FrameQueue frame_queue;
    std::thread thread_frames(frameBufferManager, std::ref(frame_queue), std::ref(video), true);

    auto timer_last = std::chrono::high_resolution_clock::now();
    while (true) {
        auto timer_now = std::chrono::high_resolution_clock::now();
        std::chrono::microseconds delta = std::chrono::duration_cast<std::chrono::microseconds>(timer_now - timer_last);
        if (delta.count() / 1000 < 40) { // wait the time that left
            usleep(40 * 1000 - delta.count());
        }
#ifdef DEBUG
        std::cout << "loop took: " << ((double)delta.count()) / 1000 << " left: " << (40 * 1000 - delta.count()) / 1000 << "\n";
#endif
        timer_last = std::chrono::high_resolution_clock::now();

        cv::Mat frame;
        // capture frame-by-frame
        cap >> frame;

        // if the frame is empty, the file ended
        if (frame.empty())
            break;

        // TODO: send frame to thread
        frame_queue.push_work(frame);

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
