#include <iostream>
#include <string>
#include <cstring>

#include "parser.hpp"

void usage(std::ostream &stream, std::string file_name) {
    stream << "usage: " << file_name << " <input file> <output>\n\
\tcan add in the end: -s for showing in realtime\n";
}

void parser(int argc, char const *argv[], std::string &input_file, std::string &output_file, bool &show) {
    if (argc < 3) {
        usage(std::cout, argv[0]);
        exit(1);
    }

    show = false;
    if (argc == 4) {
        if (!strcmp(argv[3], "-s")) {
            show = true;
        } else {
            usage(std::cout, argv[0]);
            exit(1);
        }
    }

    input_file = argv[1];
    output_file = argv[2];
}
