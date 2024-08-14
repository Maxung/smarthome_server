#ifndef CONFIGHPP
#define CONFIGHPP

#include <fstream>
#include <iostream>
#include <optional>
#include <string>
#include <utility>
#include <vector>
#include <yaml-cpp/yaml.h>

using namespace std;

class Config {
    public:
        explicit Config(const string &filepath);
        unordered_map<string, vector<pair<string, string>>> getDevices() const;

    private:
        unordered_map<string, vector<pair<string, string>>> m_Devices;
};

#endif