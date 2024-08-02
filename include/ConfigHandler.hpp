#include <optional>
#include <string>
#include <vector>
#include <yaml-cpp/yaml.h>

struct Characteristic {
        std::string uuid;
};

struct Service {
        std::string uuid;
        std::vector<Characteristic> characteristics;
};

struct Device {
        std::string name;
        std::optional<std::string> address;
        std::vector<Service> services;
};

class Config {
    public:
        explicit Config(const std::string &filepath);
        std::vector<Device> getDevices() const;

    private:
        std::vector<Device> devices;
};