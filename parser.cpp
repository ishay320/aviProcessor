#include <cstring>
#include <iostream>
#include <string>

#include "parser.hpp"


int getArgPosition(int argc, char const *argv[], char arg_name) {
    std::string arg;
    arg += "-";
    arg += arg_name;

    for (int i = 0; i < argc; i++) {
        if (argv[i] == arg) {
            return i;
        }
    }
    return -1;
}

inline bool getArg(int argc, char const *argv[], char arg_name) { return getArgPosition(argc, argv, arg_name) == -1 ? false : true; }

// TODO: regex the file ([\d|\w]+\..+)
std::string getFileName(int argc, char const *argv[], int pos) {
    if (argv[pos][0] != '-') {
        return std::string(argv[pos]);
    }

    return std::string();
}

void usage(std::ostream &stream, std::string file_name) {
    stream << "usage: " << file_name << " <input file?> <output> [options]\n\
\toptions: -s for showing in realtime\n\
\t\t -c for camera input (can remove the input arg)\n";
}

int parser(int argc, char const *argv[], std::string &input_file, std::string &output_file, bool &show, bool &camera) {
    if (argc < 3) {
        return 1;
    }

    show = getArg(argc, argv, 's');
    camera = getArg(argc, argv, 'c');
    std::cerr << show;

    input_file = getFileName(argc, argv, 1);
    if (input_file.empty())
        return 1;
    output_file = getFileName(argc, argv, 2);
    if (output_file.empty()) {
        if (!camera) {
            return 1;
        }

        output_file = input_file;
    }
    return 0;
}
