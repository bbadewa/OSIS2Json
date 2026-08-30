#pragma once
#include<exception>
#include <string>

enum ExceptionReason {
    EMPTY,
    MALFORMED,
    NONE,
    MISSING_IN,
    MISSING_OUT,
    STRANGE_ARG,
    NO_ARG

};

//Exceptions

class ArgumentException : public std::exception {
    private:
    std::string message;
    public:
    ArgumentException(ExceptionReason e, std::string arg) {
        switch (e) {
            case MISSING_IN:
                message = "Missing input file (-i [FILEPATH]).";
                break;
            case MISSING_OUT:
                message = "Missing output file (-o [FILEPATH]).";
                break;
            case STRANGE_ARG:
                message = "Strange argument \"" + arg + "\".";
                break;
            case NO_ARG:
                message = "No argument specified for \"" + arg + "\".";
                break;
            case MALFORMED:
                message = "Malformed parameter \"" + arg + "\" for preceding switch.";
                break;
            default:
                break;
        }
    }
    const char* what() {
        return message.c_str();
    }
};

class IngestionException : public std::exception {
    private:
    std::string message;
    public:
    IngestionException(std::string filename) { message = "Failed to load file " + filename + "! \n";}
    const char* what() {
        return message.c_str();
    }
};

class ConversionException : public std::exception {
    private:
    std::string message;
    public:
    ConversionException(ExceptionReason r) { 
        switch (r) {
            case MALFORMED:
            message = "This OSIS file is malformed or otherwise unparsable!"; 
            break;
            case EMPTY:
            message = "This OSIS file has no verses, or the verses are incorrectly marked!";
            default:
            message = "An unknown runtime error occured while parsing this OSIS file!";
            break;
        }
        message += " Please check your file and try again!";
    }
    const char* what() {
        return message.c_str();
    }
};

class PrintException : public std::exception {
    private:
    std::string message;
    public:
    PrintException(std::string filename) { message = "Failed to write to file " + filename + "! \n";}
    const char* what() {
        return message.c_str();
    }
};
