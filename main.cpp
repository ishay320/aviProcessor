#include <opencv2/opencv.hpp>

#include <chrono>
#include <iostream>
#include <string>
#include <unistd.h>

#define ESC 25

double randomWait() {
    double r = ((double)rand() / (RAND_MAX));
    usleep(r * 60 * 1000); // sleep for a random time in range of 0-60 ms
    return (r * 60 * 1000);
}

void usage(std::ostream &stream, std::string file_name) { stream << "usage: " << file_name << " <input file> <output>\n"; }

// TODO: add buffer and run processing in another thread
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

    auto timer_last = std::chrono::high_resolution_clock::now();
    while (true) {
        // TODO: read every 40ms - separate processing from capturing
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

        cv::putText(frame, "Big Buck Bunny", cv::Point(0, 200), cv::FONT_HERSHEY_DUPLEX, 0.7, cv::Scalar(0, 0, 0), 2, false);
        // write the frame to file
        video.write(frame);

        randomWait();

        // TODO: add option to turn on and off maybe -s for show
        // display the resulting frame
        imshow("Frame", frame);

        // press ESC on keyboard to exit
        char c = (char)cv::waitKey(ESC);
        if (c == 27)
            break;
    }

    // release the video capture objects
    cap.release();
    video.release();

    // closes frame
    cv::destroyAllWindows();

    return 0;
}
