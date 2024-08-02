#include "BLEManager.hpp"
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

    BLEManager bleManager = BLEManager(dataHandler);

    bleManager.run();

    // std::this_thread::sleep_for(20s);

    crow::SimpleApp app;

    CROW_ROUTE(app, "/")
    ([dataHandler](const crow::request &req) {
        return getTemp(dataHandler.get());
    });

    app.port(18080).run();

    return EXIT_SUCCESS;
}