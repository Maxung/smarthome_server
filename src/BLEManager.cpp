#include "BLEManager.hpp"
#include "simpleble/PeripheralSafe.h"

BLEManager::~BLEManager() {
    for (auto &p : m_Peripherals) {
        auto &peripheral = p.second.first;
        for (auto &info : p.second.second) {
            const auto [serviceUuid, characteristicUuid, subscribed] = info;
            peripheral->unsubscribe(serviceUuid, characteristicUuid);
        }
        peripheral->disconnect();
    }
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

        auto peripheralName = peripheral.identifier().value_or("UNKNOWN");

        bool found =
            std::ranges::any_of(m_Devices, [&peripheralName](const auto &pair) {
                return pair.first == peripheralName;
            });

        if (found) {
            m_Peripheralsss.push_back(peripheral);
        }
    });

    m_Adapter->set_callback_on_scan_start(
        []() { std::cout << "Scan started." << std::endl; });
    m_Adapter->set_callback_on_scan_stop(
        []() { std::cout << "Scan stopped." << std::endl; });
    // Scan for 5 seconds and return.
    m_Adapter->scan_for(5000);

    for (auto &peripheral : m_Peripheralsss) {
        auto peripheralName = peripheral.identifier().value_or("UNKNOWN");

        auto currDevice = m_Devices.at(peripheralName);
        vector<tuple<SimpleBLE::BluetoothUUID, SimpleBLE::BluetoothUUID, bool>>
            characteristicsTuple;
        for (auto &sp : currDevice) {
            characteristicsTuple.push_back(
                make_tuple(sp.first, sp.second, false));
        }
        m_Peripherals[peripheralName] =
            make_pair(std::make_unique<SimpleBLE::Safe::Peripheral>(peripheral),
                      characteristicsTuple);
        bool connect_was_successful = peripheral.connect();

        cout << "Trying to connect to " << peripheralName << endl;

        if (!connect_was_successful) {
            std::cout << "Failed to connect to "
                      << peripheral.identifier().value_or("UNKNOWN") << " ["
                      << peripheral.address().value_or("UNKNOWN") << "]"
                      << std::endl;
            throw PeripheralConnectionException(
                "Connection attempt was unsuccessfull.");
        }

        std::cout << "Successfully connected, listing services." << std::endl;
    }

    if (m_Peripherals.empty()) {
        throw PeripheralConnectionException(
            "No fitting peripherals were found.");
    }
}

void BLEManager::subscribeToCharacteristic() {
    for (auto &peripheral : m_Peripherals) {
        auto &info = peripheral.second;
        auto services = info.first->services();

        if (!services.has_value()) {
            throw CharacteristicSubscriptionException(
                "Failed to list services");
        }

        for (auto &bleTuple : info.second) {
            auto &subscribed =
                std::get<2>(bleTuple); // Get a reference to 'subscribed'
            subscribed = info.first->notify(
                std::get<0>(bleTuple), std::get<1>(bleTuple),
                [&](SimpleBLE::ByteArray bytes) {
                    string hexString = bytes.toHex();
                    m_dataHandler->writeMeasurement(hexString, peripheral.first,
                                                    std::get<1>(bleTuple));
                });
        }
    }
}

void BLEManager::run() {
    selectAdapter();
    findAndConnectPeripheral();
    subscribeToCharacteristic();
}
