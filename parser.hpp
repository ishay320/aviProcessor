#include <iostream>
#include <string>

// TODO: auto generate usage
void addUsage(std::string usage);
void usage(std::ostream &stream, std::string file_name);
int parser(int argc, char const *argv[], std::string &input_file, std::string &output_file, bool &show, bool &camera);

/**
 * @brief parse if token exist
 * EXAMPLE: main -c
 *          c exist -> true
 *          q not exist -> false
 *
 * @param token
 * @param usage
 * @param container the container will updated in parse all
 */
void parseExist(std::string token, std::string usage, bool *container);
// TODO: add required option
void parseString(std::string token, std::string default_opt, std::string usage, std::string *container);
void parseInt(std::string token, int default_opt, std::string usage, int *container);
void parseFloat(std::string token, float default_opt, std::string usage, float *container);

/**
 * @brief parse all the tokens that saved
 *
 * @param argc count of words
 * @param argv array of words, with the first in the array the main file name
 */
void parseAll(int argc, char const *argv[]);

/**
 * @brief print all the parser tokens
 * for testing
 */
void parserPrint(void);