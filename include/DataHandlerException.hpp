#ifndef DATAHANDLEREXCEPTIONHPP
#define DATAHANDLEREXCEPTIONHPP

#include <stdexcept>
#include <string>

class DataHandlerException : public std::runtime_error {
    public:
        explicit DataHandlerException(const std::string &message)
            : std::runtime_error(message) {}
};

class DatabaseException : public DataHandlerException {
    public:
        explicit DatabaseException(const std::string &message)
            : DataHandlerException(message) {}
};

class DataConversionException : public DataHandlerException {
    public:
        explicit DataConversionException(const std::string &message)
            : DataHandlerException(message) {}
};

#endif