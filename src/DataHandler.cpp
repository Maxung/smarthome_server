#include "DataHandler.hpp"
#include <string>
#include <utility>

std::string kDBPath = "./hygrometer.sqlite";

DataHandler::DataHandler(
    unordered_map<string, vector<pair<string, string>>> devices) {
    m_Db = make_unique<database>(kDBPath);

    *m_Db << "create table if not exists devices ("
             "deviceName text primary key);";

    *m_Db << "create table if not exists characteristics ("
             "characteristicUuid text primary key,"
             "deviceName text,"
             "foreign key (deviceName) references devices(deviceName));";

    *m_Db << "create table if not exists measurements ("
             "measurementID integer primary key autoincrement,"
             "characteristicUuid TEXT,"
             "value real not null,"
             "timestamp datetime default current_timestamp,"
             "foreign key (characteristicUuid) references "
             "characteristics(characteristicUuid));";

    for (auto &device : devices) {
        *m_Db << "insert or ignore into devices (deviceName) values (?);"
              << device.first;

        for (auto &p : device.second) {
            *m_Db
                << "insert or ignore into characteristics (characteristicUuid, "
                   "deviceName) values (?,?)"
                << p.second << device.first;
        }
    }

    // make the db ptr unique by giving directly via reset
    // m_Db.reset(&db);
}

void DataHandler::writeMeasurement(string &byteArray, string deviceName,
                                   string characteristic) {
    uint64_t timestamp = static_cast<uint64_t>(time(nullptr));
    float value = convertByteArray(byteArray);
    *m_Db
        << "insert into measurements (characteristicUuid, value) values (?, ?);"
        << characteristic << value;
}

pair<double, int> DataHandler::readLastMeasurement(string deviceName,
                                                   string characteristicUuid) {

    for (auto &&row :
         *m_Db << "SELECT m.value, strftime('%s', m.timestamp) AS timestamp "
                  "FROM Measurements m JOIN Characteristics c ON "
                  "m.characteristicUuid = c.characteristicUuid "
                  "WHERE c.deviceName = ? AND c.characteristicUuid = ? "
                  "ORDER BY timestamp DESC LIMIT 1;"
               << deviceName << characteristicUuid) {
        double value;
        int timestamp;
        row >> value >> timestamp;
        return make_pair(value, timestamp);
    }
    // If no matching key found
    return make_pair(0, 0);
}

int convertBytesToInt(const std::string &bytes) {
    std::stringstream ss;
    for (auto b : bytes) {
        ss << std::hex << (uint32_t)((uint8_t)b);
    }
    // Create a stringstream to convert hex string to int
    int result = 0;
    ss >> result;

    // Check for conversion errors
    if (ss.fail()) {
        throw std::invalid_argument("Invalid hex string.");
    }

    return result;
}

float DataHandler::convertByteArray(string &byteArray) {
    int tempInt = convertBytesToInt(byteArray);
    return static_cast<float>(tempInt) / 100.0;
}