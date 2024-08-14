cmake_minimum_required(VERSION 3.15.0)
project(smarthome_server VERSION 0.8.0 LANGUAGES C CXX)
set(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(simpleble REQUIRED)
find_package(RocksDB REQUIRED)
find_package(crowcpp REQUIRED)
find_package(yamlcpp REQUIRED)
find_package(modernsqlite REQUIRED)

add_executable(smarthome_server
    src/main.cpp
    src/BLEManager.cpp
    src/DataHandler.cpp
    src/ConfigHandler.cpp
    src/CrowServer.cpp)
target_include_directories(smarthome_server SYSTEM PRIVATE include)
target_include_directories(smarthome_server PRIVATE ${modernsqlite_SOURCE_DIR}/hdr)
target_link_libraries(smarthome_server simpleble::simpleble RocksDB::rocksdb Crow::Crow yaml-cpp::yaml-cpp sqlite3)