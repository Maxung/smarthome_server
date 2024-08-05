cmake_minimum_required(VERSION 3.15.0)
project(hygrometer_core VERSION 0.1.0 LANGUAGES C CXX)
set(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(simpleble REQUIRED)
find_package(RocksDB REQUIRED)
find_package(crowcpp REQUIRED)
find_package(yamlcpp REQUIRED)
find_package(modernsqlite REQUIRED)

add_executable(hygrometer_core
    src/main.cpp
    src/BLEManager.cpp
    src/utils.cpp
    src/DataHandler.cpp
    src/ConfigHandler.cpp)
target_include_directories(hygrometer_core SYSTEM PRIVATE include)
target_include_directories(hygrometer_core PRIVATE ${modernsqlite_SOURCE_DIR}/hdr)
target_link_libraries(hygrometer_core simpleble::simpleble RocksDB::rocksdb Crow::Crow yaml-cpp::yaml-cpp)