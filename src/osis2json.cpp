
#include <iostream>
#include <chrono>
#include "parser.hpp"

typedef struct {
    std::string in;
    std::string out;
    bool multithread;
    int threads;
} ParserSettings;

ParserSettings parseArgs(int argc, char* argv[]) {

    if (argc == 1) {
        throw ArgumentException(EMPTY, "");
    }

    ParserSettings settings;
    bool argSwitch = true; //True for flags, false for their inputs. 
    bool argType = false; //false for ints, true for strings;
    void* argPtr;
    int idx = 1;
    /*
    We'll tackle the problem like this:
    First, ingest a switch (-j for threads (jobs), -i for input, -o for output)
    This switch will set a pointer to where we'll manipulate our ParserSettings struct.
    Once the thing has been written to, we use the result of the write to either switch back to switch-mode
    or immediately return because there's been an error.
    */
    std::string arg;
    while (idx < argc) {
        arg = argv[idx];
        if (argSwitch) {
            if (arg == "-i") {
                argType = true;
                argPtr = &settings.in;
            } else if (arg == "-o") {
                argType = true;
                argPtr = &settings.out;
            } else if (arg == "-j") {
                argType = false;
                settings.multithread = true;
                argPtr = &settings.threads;
            } else {
                throw ArgumentException(STRANGE_ARG, arg); 
            }
        } else {
            if (argType) {
                *(std::string*)argPtr = arg;
            } else {
                try {
                    *(int*)argPtr = std::stoi(arg.c_str());
                } catch (std::exception e) {
                    throw ArgumentException(MALFORMED, arg);
                }
            }
        }
        idx++;
        argSwitch = !argSwitch;
    }
    if (!argSwitch) { //Completed early without finishing arguments.
        throw ArgumentException(NO_ARG, arg);
    }

    return settings;
}


int main(int argc, char* argv[]) {
    try {
        ParserSettings ps = parseArgs(argc, argv);
        Parser p(ps.multithread, ps.threads);
        p.Ingest(ps.in);
        p.Parse();
        p.Print(ps.out);
    } catch (const std::exception& e) {
        std::cerr << e.what() << std::endl;
        return -1;
    }
    return 0;
}