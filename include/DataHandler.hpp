#include "DataHandlerException.hpp"
#include "rocksdb/db.h"
#include "rocksdb/options.h"
#include "rocksdb/slice.h"
#include "sqlite_modern_cpp.h"
#include <algorithm>
#include <iomanip>
#include <iostream>
#include <memory>
#include <ranges>
#include <string>
#include <vector>

using namespace std;
using namespace sqlite;

class DataHandler {
    public:
        DataHandler(
            unordered_map<string, vector<pair<string, string>>> devices);
        void writeMeasurement(string &byteArray, string deviceName,
                              string characteristic);
        pair<double, int> readLastMeasurement(string deviceName,
                                              string characteristicUuid);
        vector<pair<double, int>>
        readAllMeasurements(string deviceName, string characteristicUuid);

    private:
        unique_ptr<database> m_Db;
        float convertByteArray(string &byteArray);
};