#ifndef CROWSERVERHPP
#define CROWSERVERHPP

#include "DataHandler.hpp"
#include <crow.h>

class CrowServer {
    public:
        CrowServer(
            shared_ptr<DataHandler> &dataHandler,
            unordered_map<string, vector<pair<string, string>>> &devices);
        void run();

    private:
        shared_ptr<DataHandler> m_dataHandler;
        unordered_map<string, vector<pair<string, string>>> m_Devices;
        crow::SimpleApp app;
        crow::response getLastMeasurementsOnly(
            DataHandler *dataHandler,
            unordered_map<string, vector<pair<string, string>>> &devices);
        crow::response getAllMeasurements(
            DataHandler *dataHandler,
            unordered_map<string, vector<pair<string, string>>> &devices);
};

#endif