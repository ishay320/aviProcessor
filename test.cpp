#include <iostream>
#include <string>

#include "AviProccesor.hpp"
#include "parser.hpp"

#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"

TEST_CASE("parsing test") {
    SUBCASE("without show") {
        int argc = 5;
        const char *argv[] = {"one", "-o", "outfile", "-i", "infile"};
        std::string input_file;
        std::string output_file;
        bool show;
        bool camera;
        CHECK_EQ(parser(argc, argv, input_file, output_file, show, camera), 0);

        CHECK_EQ(input_file, "infile");
        CHECK_EQ(output_file, "outfile");
        CHECK_EQ(show, false);
    }
    SUBCASE("with show") {
        int argc = 6;
        const char *argv[] = {"one", "-o", "outfile", "-i", "infile", "-s"};
        std::string input_file;
        std::string output_file;
        bool show;
        bool camera;
        CHECK_EQ(parser(argc, argv, input_file, output_file, show, camera), 0);

        CHECK_EQ(input_file, "infile");
        CHECK_EQ(output_file, "outfile");
        CHECK_EQ(show, true);
    }
    SUBCASE("parsing error") {
        int argc = 2;
        const char *argv[] = {"one", "two"};
        std::string input_file;
        std::string output_file;
        bool show;
        bool camera;
        CHECK_EQ(parser(argc, argv, input_file, output_file, show, camera), 1);
    }
}
TEST_CASE("avi processor") {
    SUBCASE("random wait") {
        for (size_t i = 0; i < 10; i++) {
            double rand = randomWait();
            CHECK_LE(rand, 60 * 1000);
        }
    }
    SUBCASE("FrameQueue") {
        FrameQueue fq;
        fq.pushWork(cv::Mat());
        cv::Mat frame = fq.waitAndPop();
        CHECK(frame.empty());

        fq.workDone();
        CHECK(frame.empty());
    }
}
