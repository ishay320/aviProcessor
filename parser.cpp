#include <cstring>
#include <iostream>
#include <string>
#include <vector>

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

class Token {
    // for parsing: int | bool | string | float
  private:
    char type;
    bool *b;
    int *i;
    std::string *s;
    float *f;

    std::string token;
    std::string usage;

  public:
    Token(std::string token, std::string usage, bool *b, int *i, std::string *s, float *f) {
        this->b = b;
        this->i = i;
        this->s = s;
        this->f = f;

        this->token = token;
        this->usage = usage;
    }

    // TODO: check for size of token: -- or - or custom
    std::string getToken() { return "-" + this->token; }
    std::string getUsage() { return this->usage; }

    char getType() {
        if (this->b != nullptr) {
            return 'b';
        } else if (this->i != nullptr) {
            return 'i';
        } else if (this->s != nullptr) {
            return 's';
        } else if (this->f != nullptr) {
            return 'f';
        } else {
            return '?';
        }
    }
    bool getBool() { return *(this->b); }
    int getInt() { return *(this->i); }
    std::string getString() { return *(this->s); }
    float getFloat() { return *(this->f); }

    void setBool(bool b) { *(this->b) = b; }
    void setInt(int i) { *(this->i) = i; }
    void setString(std::string s) { *(this->s) = s; }
    void setFloat(float f) { *(this->f) = f; }
};

std::vector<Token> g_container_vec;

void parseExist(std::string token, std::string usage, bool *container) {
    Token con{token, usage, container, nullptr, nullptr, nullptr};
    g_container_vec.push_back(con);
}

void parseString(std::string token, std::string default_opt, std::string usage, std::string *container) {
    Token con{token, usage, nullptr, nullptr, container, nullptr};
    con.setString(default_opt);
    g_container_vec.push_back(con);
}

void parseInt(std::string token, int default_opt, std::string usage, int *container) {
    Token con{token, usage, nullptr, container, nullptr, nullptr};
    con.setInt(default_opt);
    g_container_vec.push_back(con);
}

void parseFloat(std::string token, float default_opt, std::string usage, float *container) {
    Token con{token, usage, nullptr, nullptr, nullptr, container};
    con.setFloat(default_opt);
    g_container_vec.push_back(con);
}

void parserPrint(void) {
    std::cout << "***********\n";
    for (auto &&i : g_container_vec) {
        char type = i.getType();
        switch (type) {
        case 'b':
            std::cout << "token: " << i.getToken() << " |usage: " << i.getUsage() << " |type: bool -> " << i.getBool() << std::endl;
            break;
        case 'i':
            std::cout << "token: " << i.getToken() << " |usage: " << i.getUsage() << " |type: int -> " << i.getInt() << std::endl;
            break;
        case 's':
            std::cout << "token: " << i.getToken() << " |usage: " << i.getUsage() << " |type: string -> " << i.getString() << std::endl;
            break;
        case 'f':
            std::cout << "token: " << i.getToken() << " |usage: " << i.getUsage() << " |type: float -> " << i.getFloat() << std::endl;
            break;
        case '?':
            std::cout << "token: " << i.getToken() << " |usage: " << i.getUsage() << " |type: ??????" << std::endl;
            break;
        default:
            break;
        }
    }

    std::cout << "***********\n";
}

void parseAll(int argc, char const *argv[]) {
    for (int i = 1; i < argc; i++) {
        for (auto &&key : g_container_vec) {
            if (key.getToken().compare(argv[i]) == 0) {
                switch (key.getType()) {
                case 'b':
                    key.setBool(true);
                    break;
                case 's':
                    if (i < argc) {
                        key.setString(argv[++i]);
                    } else {
                        // TODO: throw error
                    }
                    break;
                case 'i':
                    if (i < argc) {
                        key.setInt(std::stoi(argv[++i]));
                    } else {
                        // TODO: throw error
                    }
                    break;
                case 'f':
                    if (i < argc) {
                        key.setInt(std::stof(argv[++i]));
                    } else {
                        // TODO: throw error
                    }
                    break;
                default:
                    // TODO: throw error
                    break;
                }
                break;
            }
        }
    }
}