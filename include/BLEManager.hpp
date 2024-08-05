#include "BLEConfig.hpp"
#include "BLEException.hpp"
#include "ConfigHandler.hpp"
#include "DataHandler.hpp"
#include "simpleble/Types.h"
#include "utils.hpp"
#include <iostream>
#include <memory>
#include <simpleble/SimpleBLE.h>
#include <thread>
#include <utility>
#include <vector>

using namespace SimpleBLE::Safe;
using namespace std;
using namespace std::chrono_literals;

class BLEManager {
    public:
        BLEManager(shared_ptr<DataHandler> &dataHandler,
                   unordered_map<string, vector<pair<string, string>>> &devices)
            : m_dataHandler{dataHandler}, m_Devices{devices} {}
        ~BLEManager();
        void run();

    private:
        shared_ptr<DataHandler> m_dataHandler;
        unordered_map<string, vector<pair<string, string>>> m_Devices;
        unordered_map<string,
                      pair<unique_ptr<Peripheral>,
                           vector<tuple<SimpleBLE::BluetoothUUID,
                                        SimpleBLE::BluetoothUUID, bool>>>>
            m_Peripherals;
        vector<SimpleBLE::BluetoothUUID> m_CharacteristicsToSubscribe = {
            SimpleBLE::BluetoothUUID(TEMPERATURE_CHARACTERISTIC),
            SimpleBLE::BluetoothUUID(HUMIDITY_CHARACTERISTIC)};
        unique_ptr<Adapter> m_Adapter;
        vector<Peripheral> m_Peripheralsss;
        vector<pair<SimpleBLE::BluetoothUUID, SimpleBLE::BluetoothUUID>>
            m_SubscribedCharacteristics;

        void selectAdapter();
        void findAndConnectPeripheral();
        void subscribeToCharacteristic();
};
