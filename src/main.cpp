#include "BLEManager.hpp"
#include "ConfigHandler.hpp"
#include <crow.h>
#include <iostream>
#include <memory>
#include <thread>

using namespace std::chrono_literals;

crow::response getLastMeasurementsOnly(
    DataHandler *dataHandler,
    unordered_map<string, vector<pair<string, string>>> &devices) {
    crow::json::wvalue result;

    // Iterate through each device
    for (auto &d : devices) {
        const string &deviceName = d.first;
        crow::json::wvalue deviceJson;

        // Iterate through each characteristic for the device
        for (auto &c : d.second) {
            const string &characteristicName = c.second;

            // Read the last measurement for this characteristic
            auto res = dataHandler->readLastMeasurement(deviceName,
                                                        characteristicName);

            double value = res.first;
            long long timestamp = res.second;

            crow::json::wvalue characteristicJson;
            characteristicJson["value"] = value;
            characteristicJson["timestamp"] = timestamp;

            deviceJson[characteristicName] = std::move(characteristicJson);
        }
        result[deviceName] = std::move(deviceJson);
    }
    return crow::response(result);
}

crow::response getAllMeasurements(
    DataHandler *dataHandler,
    unordered_map<string, vector<pair<string, string>>> &devices) {

    crow::json::wvalue result;

    // Iterate through each device
    for (auto &d : devices) {
        const std::string &deviceName = d.first;
        crow::json::wvalue deviceJson;

        // Iterate through each characteristic for the device
        for (auto &c : d.second) {
            const std::string &characteristicName = c.second;

            // Read all measurements for this characteristic
            auto res = dataHandler->readAllMeasurements(deviceName,
                                                        characteristicName);

            // Create a JSON array to hold all measurements
            crow::json::wvalue characteristicJsonArray =
                crow::json::wvalue::list();

            // Iterate through each measurement
            for (size_t i = 0; i < res.size(); ++i) {
                crow::json::wvalue measurementJson;
                measurementJson["value"] = res[i].first;
                measurementJson["timestamp"] = res[i].second;

                // Add the measurement to the JSON array
                characteristicJsonArray[i] = std::move(measurementJson);
            }

            // Add the characteristic JSON array to the device JSON
            deviceJson[characteristicName] = std::move(characteristicJsonArray);
        }

        // Add the device JSON to the final result
        result[deviceName] = std::move(deviceJson);
    }

    // Return the JSON response
    return crow::response(result);
}

int main(int, char **) {

    Config config("../config.yaml");
    auto devices = config.getDevices();

    std::shared_ptr<DataHandler> dataHandler =
        std::make_shared<DataHandler>(DataHandler(devices));

    BLEManager bleManager = BLEManager(dataHandler, devices);

    bleManager.run();

    // std::this_thread::sleep_for(20s);

    crow::SimpleApp app;

    CROW_ROUTE(app, "/last")
    ([dataHandler, &devices](const crow::request &req) {
        return getLastMeasurementsOnly(dataHandler.get(), devices);
    });

    CROW_ROUTE(app, "/all")
    ([dataHandler, &devices](const crow::request &req) {
        return getAllMeasurements(dataHandler.get(), devices);
    });

    app.port(18080).run();

    return EXIT_SUCCESS;
}