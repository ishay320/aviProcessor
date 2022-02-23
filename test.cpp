#include <iostream>
#include <string>

#include "AviProccesor.hpp"
#include "parser.hpp"

#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"

TEST_CASE("parsing test") {
    SUBCASE("without show") {
        int argc = 3;
        const char *argv[] = {"one", "two", "three"};
        std::string input_file;
        std::string output_file;
        bool show;
        CHECK_EQ(parser(argc, argv, input_file, output_file, show), 0);

        CHECK_EQ(input_file, "two");
        CHECK_EQ(output_file, "three");
        CHECK_EQ(show, false);
    }
    SUBCASE("with show") {
        int argc = 4;
        const char *argv[] = {"one", "two", "three", "-s"};
        std::string input_file;
        std::string output_file;
        bool show;
        CHECK_EQ(parser(argc, argv, input_file, output_file, show), 0);

        CHECK_EQ(input_file, "two");
        CHECK_EQ(output_file, "three");
        CHECK_EQ(show, true);
    }
    SUBCASE("parsing error") {
        int argc = 2;
        const char *argv[] = {"one", "two"};
        std::string input_file;
        std::string output_file;
        bool show;
        CHECK_EQ(parser(argc, argv, input_file, output_file, show), 1);
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
