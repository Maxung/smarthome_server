#include "BLEManager.hpp"
#include "ConfigHandler.hpp"
#include "CrowServer.hpp"

int main(int, char **) {
    // read config
    Config config("../config.yaml");
    auto devices = config.getDevices();

    // create database
    std::shared_ptr<DataHandler> dataHandler =
        std::make_shared<DataHandler>(DataHandler(devices));

    // initialize ble manager and run it (asynchronously)
    BLEManager bleManager = BLEManager(dataHandler, devices);
    bleManager.run();

    // create and start server
    CrowServer srv = CrowServer(dataHandler, devices);
    srv.run();

    return EXIT_SUCCESS;
}