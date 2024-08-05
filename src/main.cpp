#include "BLEManager.hpp"
#include "ConfigHandler.hpp"
#include <crow.h>
#include <iostream>
#include <memory>
#include <thread>

using namespace std::chrono_literals;

crow::response getTemp(DataHandler *dataHandler) {
    std::string lastTemp = dataHandler->readLastMeasurement("temperature-");
    std::string lastHumd = dataHandler->readLastMeasurement("humidity-");
    crow::json::wvalue x(
        {{"test",
          {{"temp", std::stod(lastTemp)}, {"humd", std::stod(lastHumd)}}}});
    return crow::response(x);
}

int main(int, char **) {

    std::shared_ptr<DataHandler> dataHandler =
        std::make_shared<DataHandler>(DataHandler());

    Config config("../config.yaml");
    auto devices = config.getDevices();

    BLEManager bleManager = BLEManager(dataHandler, devices);

    bleManager.run();

    // // std::this_thread::sleep_for(20s);

    // crow::SimpleApp app;

    // CROW_ROUTE(app, "/")
    // ([dataHandler](const crow::request &req) {
    //     return getTemp(dataHandler.get());
    // });

    // app.port(18080).run();
    // std::string configFilePath = "/Users/max/hygrometer_core/config.yaml";

    // try {
    //     Config config(configFilePath);
    //     auto devices = config.getDevices();

    //     // Display the parsed data
    //     for (const auto &device : devices) {
    //         std::cout << "Device: " << device.name << std::endl;
    //         if (device.address.has_value()) {
    //             std::cout << "  Address: " << device.address.value()
    //                       << std::endl;
    //         }
    //         for (const auto &service : device.services) {
    //             std::cout << "  Service UUID: " << service.uuid << std::endl;
    //             for (const auto &characteristic : service.characteristics) {
    //                 std::cout
    //                     << "    Characteristic UUID: " << characteristic.uuid
    //                     << std::endl;
    //             }
    //         }
    //     }
    // } catch (const std::exception &e) {
    //     std::cerr << "Error: " << e.what() << std::endl;
    //     return 1;
    // }

    return EXIT_SUCCESS;
}