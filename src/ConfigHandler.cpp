#include "ConfigHandler.hpp"
#include <fstream>
#include <yaml-cpp/yaml.h>

Config::Config(const std::string &filepath) {
    // Open and parse the YAML file
    YAML::Node config = YAML::LoadFile(filepath);

    // Parse the YAML data into devices vector
    for (auto it : config) {
        Device device;
        const YAML::Node &dev = it.second;
        device.name = dev["name"].as<std::string>();

        //  Parse optional 'address' field
        if (dev["address"]) {
            device.address = dev["address"].as<std::string>();
        }

        // Parse 'services'
        for (const YAML::Node &service : dev["services"]) {
            Service srv;
            srv.uuid = service["uuid"].as<std::string>();

            // Parse 'characteristics'
            for (const YAML::Node &characteristic :
                 service["characteristics"]) {
                Characteristic charac;
                charac.uuid = characteristic["uuid"].as<std::string>();
                srv.characteristics.push_back(charac);
            }
            device.services.push_back(srv);
        }
        devices.push_back(device);
    }
}

std::vector<Device> Config::getDevices() const { return devices; }