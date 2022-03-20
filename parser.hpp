#include <iostream>
#include <string>

// TODO: auto generate usage 
void addUsage(std::string usage);
void usage(std::ostream &stream, std::string file_name);
int parser(int argc, char const *argv[], std::string &input_file, std::string &output_file, bool &show, bool &camera);