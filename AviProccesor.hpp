#include <opencv2/opencv.hpp>

#include <condition_variable>
#include <mutex>
#include <queue>

#define ESC 25

class FrameQueue {
  private:
    std::condition_variable work_available;
    std::mutex work_mutex;
    std::queue<cv::Mat> frames;

  public:
    void workDone();

    void pushWork(cv::Mat frame);

    cv::Mat waitAndPop();
};

double randomWait();

cv::Mat process(cv::Mat &frame);

void frameBufferManager(FrameQueue &frame_queue, cv::VideoWriter &video, bool show);