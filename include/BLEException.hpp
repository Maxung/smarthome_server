#include <stdexcept>
#include <string>

class BLEException : public std::runtime_error {
    public:
        explicit BLEException(const std::string &message)
            : std::runtime_error(message) {}
};

class AdapterSelectionException : public BLEException {
    public:
        explicit AdapterSelectionException(const std::string &message)
            : BLEException(message) {}
};

class PeripheralConnectionException : public BLEException {
    public:
        explicit PeripheralConnectionException(const std::string &message)
            : BLEException(message) {}
};

class CharacteristicSubscriptionException : public BLEException {
    public:
        explicit CharacteristicSubscriptionException(const std::string &message)
            : BLEException(message) {}
};
