#include "BLEManager.hpp"
#include "utils.hpp"
#include <string>
#include <utility>

BLEManager::~BLEManager() {
    for (auto p : m_SubscribedCharacteristics) {
        m_Peripheral->unsubscribe(p.first, p.second);
    }
    m_Peripheral->disconnect();
}

void BLEManager::selectAdapter() {
    auto adapter_list = Adapter::get_adapters();

    if (!adapter_list.has_value()) {
        throw AdapterSelectionException("Couldn't retrieve adapters.");
    }

    if (adapter_list->empty()) {
        throw AdapterSelectionException("No adapters were found.");
    }

    cout << "Available adapters: \n";
    int i = 0;
    for (auto &adapter : *adapter_list) {
        cout << "[" << i++ << "] " << adapter.identifier().value() << " ["
             << adapter.address().value() << "]" << endl;
    }

    m_Adapter = make_unique<Adapter>(adapter_list->front());
}

void BLEManager::findAndConnectPeripheral() {
    std::vector<Peripheral> peripherals;

    m_Adapter->set_callback_on_scan_found([&](Peripheral peripheral) {
        std::cout << "Found device: "
                  << peripheral.identifier().value_or("UNKNOWN") << " ["
                  << peripheral.address().value_or("UNKNOWN") << "]"
                  << std::endl;

        if (peripheral.identifier().value_or("UNKNOWN").contains(
                "Bluefruit Temp")) {
            peripherals.push_back(peripheral);
            m_Adapter->scan_stop();
        }
    });

    m_Adapter->set_callback_on_scan_start(
        []() { std::cout << "Scan started." << std::endl; });
    m_Adapter->set_callback_on_scan_stop(
        []() { std::cout << "Scan stopped." << std::endl; });
    // Scan for 5 seconds and return.
    // m_Adapter->scan_for(5000);
    m_Adapter->scan_start();

    std::this_thread::sleep_for(2s);

    for (size_t i = 0; i < peripherals.size(); i++) {
        if (peripherals[i].identifier().value_or("UNKNOWN") ==
            "Bluefruit Temp") {
            m_Peripheral =
                std::make_unique<SimpleBLE::Safe::Peripheral>(peripherals[i]);
        }
    }

    if (m_Peripheral == nullptr) {
        throw PeripheralConnectionException("No fitting peripheral was found.");
    }

    // If the connection wasn't successful, no exception will be thrown.
    bool connect_was_successful = m_Peripheral->connect();

    if (!connect_was_successful) {
        std::cout << "Failed to connect to "
                  << m_Peripheral->identifier().value_or("UNKNOWN") << " ["
                  << m_Peripheral->address().value_or("UNKNOWN") << "]"
                  << std::endl;
        return throw PeripheralConnectionException(
            "Connection attempt was unsuccessfull.");
    }

    std::cout << "Successfully connected, listing services." << std::endl;
}

void BLEManager::subscribeToCharacteristic() {
    std::cout << "Successfully connected, listing services." << std::endl;
    auto services = m_Peripheral->services();

    if (!services.has_value()) {
        throw CharacteristicSubscriptionException("Failed to list services");
    }

    for (auto service : *services) {
        if (service.uuid() == ENVIRONMENT_SERVICE) {
            std::cout << "Service: " << service.uuid() << std::endl;
            vector<string> characteristicsUuids;
            for (auto characteristic : service.characteristics()) {
                std::cout << "  Characteristic: " << characteristic.uuid()
                          << std::endl;
                std::cout << "    capabilities:" << std::endl;

                for (auto c : characteristic.capabilities()) {
                    std::cout << "      " << c << std::endl;
                }

                characteristicsUuids.push_back(characteristic.uuid());
            }
            // if (Utils::contains_all(m_CharacteristicsToSubscribe,
            //                         characteristicsUuids))
            //     throw CharacteristicSubscriptionException(
            //         "Couldn't find all required characteristics.");
        }
    }

    // temperature notify
    bool subscribed = m_Peripheral->notify(
        SimpleBLE::BluetoothUUID(ENVIRONMENT_SERVICE),
        SimpleBLE::BluetoothUUID(TEMPERATURE_CHARACTERISTIC),
        [&](SimpleBLE::ByteArray bytes) {
            m_dataHandler->writeMeasurement(bytes, "temperature-");
        });

    if (subscribed)
        m_SubscribedCharacteristics.push_back(
            make_pair(SimpleBLE::BluetoothUUID(ENVIRONMENT_SERVICE),
                      m_CharacteristicsToSubscribe.at(0)));

    subscribed = m_Peripheral->notify(
        SimpleBLE::BluetoothUUID(ENVIRONMENT_SERVICE),
        SimpleBLE::BluetoothUUID(HUMIDITY_CHARACTERISTIC),
        [&](SimpleBLE::ByteArray bytes) {
            m_dataHandler->writeMeasurement(bytes, "humidity-");
        });

    if (subscribed)
        m_SubscribedCharacteristics.push_back(
            make_pair(SimpleBLE::BluetoothUUID(ENVIRONMENT_SERVICE),
                      m_CharacteristicsToSubscribe.at(1)));
}

void BLEManager::run() {
    selectAdapter();
    findAndConnectPeripheral();
    subscribeToCharacteristic();
}
