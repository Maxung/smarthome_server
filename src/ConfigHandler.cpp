#include "ConfigHandler.hpp"

Config::Config(const string &filepath) {
    std::cout << "Reading config file." << std::endl;
    // Open and parse the YAML file
    YAML::Node config = YAML::LoadFile(filepath);

    // Parse the YAML data into devices vector
    for (auto it : config) {
        const YAML::Node &dev = it.second;
        string deviceName = dev["name"].as<string>();
        vector<pair<string, string>> srvCharacPair;
        // Parse 'services'
        for (const YAML::Node &service : dev["services"]) {
            string srvUuid = service["uuid"].as<string>();
            // Parse 'characteristics'
            for (const YAML::Node &characteristic :
                 service["characteristics"]) {
                string characteristicUuid = characteristic["uuid"].as<string>();
                srvCharacPair.push_back(make_pair(srvUuid, characteristicUuid));
            }
        }
        m_Devices[deviceName] = srvCharacPair;
    }
}

unordered_map<string, vector<pair<string, string>>> Config::getDevices() const {
    return m_Devices;
}