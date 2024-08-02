#include "DataHandler.hpp"
#include <string>

std::string kDBPath = "./rocksdb_hygrometer";

DataHandler::DataHandler() {
    DB *db;

    Options options;
    // Optimize RocksDB. This is the easiest way to get RocksDB to perform well
    options.IncreaseParallelism();
    options.OptimizeLevelStyleCompaction();
    // create the DB if it's not already present
    options.create_if_missing = true;

    Status s = DB::Open(options, kDBPath, &db);
    if (!s.ok())
        throw DatabaseException(s.ToString());

    // make the db ptr unique by giving directly via reset
    m_Db.reset(db);
}

void DataHandler::writeMeasurement(string &byteArray, const char *prefix) {
    uint64_t timestamp = static_cast<uint64_t>(time(nullptr));
    float temperature = convertTemperature(byteArray);
    cout << temperature << endl;
    Status s = m_Db->Put(WriteOptions(), prefix + to_string(timestamp),
                         to_string(temperature));
    if (!s.ok())
        throw DatabaseException(s.ToString());
}

string DataHandler::readLastMeasurement(const char *prefix) {
    rocksdb::ReadOptions readOptions;
    std::unique_ptr<rocksdb::Iterator> it(m_Db->NewIterator(readOptions));

    // Seek to the last key that starts with the given prefix
    string upperBound =
        string(prefix) + char(255); // 255 is the highest ASCII character
    it->SeekForPrev(upperBound);

    while (it->Valid()) {
        // Check if the current key starts with the prefix
        if (it->key().starts_with(prefix)) {
            cout << it->value().ToString() << endl;
            return it->value().ToString();
        }
        // Move to the previous key
        it->Prev();
    }

    // If no matching key found
    return string("");
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

float DataHandler::convertTemperature(string &byteArray) {
    int tempInt = convertBytesToInt(byteArray);
    return static_cast<float>(tempInt) / 100.0;
}