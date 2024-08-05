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

using ROCKSDB_NAMESPACE::DB;
using ROCKSDB_NAMESPACE::Options;
using ROCKSDB_NAMESPACE::PinnableSlice;
using ROCKSDB_NAMESPACE::ReadOptions;
using ROCKSDB_NAMESPACE::Status;
using ROCKSDB_NAMESPACE::WriteBatch;
using ROCKSDB_NAMESPACE::WriteOptions;
using namespace std;

class DataHandler {
    public:
        DataHandler();
        void writeMeasurement(string &byteArray, const char *prefix);
        string readLastMeasurement(const char *prefix);
        void readMeasurement();

    private:
        unique_ptr<DB> m_Db;
        float convertTemperature(string &byteArray);
};