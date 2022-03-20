#include <cstring>
#include <iostream>
#include <string>

#include "parser.hpp"

static std::string g_usage_str;

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

std::string getArgStr(int argc, char const *argv[], char arg_name) {
    int pos = getArgPosition(argc, argv, arg_name);
    if (pos + 1 > argc || pos == -1) {
        return std::string();
    }

    return std::string(argv[pos + 1]);
}

// TODO: regex the file ([\d|\w]+\..+)
std::string getFileName(int argc, char const *argv[], int pos) {
    if (argv[pos][0] != '-') {
        return std::string(argv[pos]);
    }

    return std::string();
}

void addUsage(std::string usage) { g_usage_str = usage; }

void usage(std::ostream &stream, std::string file_name) { stream << "usage: " << file_name << g_usage_str << '\n'; }

int parser(int argc, char const *argv[], std::string &input_file, std::string &output_file, bool &show, bool &camera) {
    if (argc < 3) {
        return 1;
    }

    show = getArg(argc, argv, 's');
    camera = getArg(argc, argv, 'c');

    input_file = getArgStr(argc, argv, 'i');
    output_file = getArgStr(argc, argv, 'o');

    if (input_file.empty() && !camera) {
        return 1;
    }

    if (output_file.empty() && !show) {
        return 1;
    }
    return 0;
}
