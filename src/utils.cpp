#include "utils.hpp"

#include <iomanip>
#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <ranges>

void Utils::print_byte_array(const SimpleBLE::ByteArray& bytes) {
    for (auto b : bytes) {
        std::cout << std::hex << std::setfill('0') << std::setw(2) << (uint32_t)((uint8_t)b) << " ";
    }
    std::cout << std::endl;
}

bool Utils::contains_all(const std::vector<std::string>& v1, const std::vector<std::string>& v2) {
    return std::ranges::all_of(v2, [&v1](const std::string& s) {
        return std::ranges::find(v1, s) != v1.end();
    });
}
