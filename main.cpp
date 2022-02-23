#include <opencv2/opencv.hpp>

#include <iostream>
#include <string>

#define ESC 25

void usage(std::ostream &stream, std::string file_name) { stream << "usage: " << file_name << " <input file> <output>\n"; }

int main(int argc, char const *argv[]) {
    if (argc < 3) {
        usage(std::cout, argv[0]);
        return 1;
    }
    std::string input_file = argv[1];
    std::string output_file = argv[2];

    // Create a VideoCapture object and open the input file
    cv::VideoCapture cap(input_file);

    // Check if opened successfully
    if (!cap.isOpened()) {
        std::cout << "Error opening video stream or file" << std::endl;
        return 1;
    }

    // resolutions of the frame
    int frame_width = cap.get(cv::CAP_PROP_FRAME_WIDTH);
    int frame_height = cap.get(cv::CAP_PROP_FRAME_HEIGHT);

    // Define the codec and create VideoWriter object
    cv::VideoWriter video(output_file, cv::VideoWriter::fourcc('M', 'J', 'P', 'G'), 10, cv::Size(frame_width, frame_height));

    while (true) {
        // TODO: read every 40ms - maybe use delta time to wait for next frame

        cv::Mat frame;
        // Capture frame-by-frame
        cap >> frame;

        // If the frame is empty, break immediately
        if (frame.empty())
            break;

        // TODO: add on the frame "first blender video ;)"
        // Write the frame to file
        video.write(frame);

        // TODO: add option to turn on and off maybe -s for show
        // Display the resulting frame
        imshow("Frame", frame);

        // Press ESC on keyboard to exit
        char c = (char)cv::waitKey(ESC);
        if (c == 27)
            break;
    }

    // release the video capture objects
    cap.release();
    video.release();

    // Closes frame
    cv::destroyAllWindows();

    return 0;
}
