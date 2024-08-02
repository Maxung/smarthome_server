#include "BLEConfig.hpp"
#include "BLEException.hpp"
#include "DataHandler.hpp"
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
        BLEManager(shared_ptr<DataHandler> &dataHandler)
            : m_dataHandler{dataHandler} {}
        ~BLEManager();
        void run();

    private:
        shared_ptr<DataHandler> m_dataHandler;
        vector<SimpleBLE::BluetoothUUID> m_CharacteristicsToSubscribe = {
            SimpleBLE::BluetoothUUID(TEMPERATURE_CHARACTERISTIC),
            SimpleBLE::BluetoothUUID(HUMIDITY_CHARACTERISTIC)};
        unique_ptr<Adapter> m_Adapter;
        unique_ptr<Peripheral> m_Peripheral;
        vector<pair<SimpleBLE::BluetoothUUID, SimpleBLE::BluetoothUUID>>
            m_SubscribedCharacteristics;
        void selectAdapter();
        void findAndConnectPeripheral();
        void subscribeToCharacteristic();
};
