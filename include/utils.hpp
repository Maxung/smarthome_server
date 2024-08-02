#ifndef UTILS_HPP
#define UTILS_HPP

#include "simpleble/SimpleBLE.h"
#include "simpleble/Types.h"

namespace Utils {
void print_byte_array(const SimpleBLE::ByteArray &bytes);
bool contains_all(const std::vector<SimpleBLE::BluetoothUUID> &v1,
                  const std::vector<SimpleBLE::BluetoothUUID> &v2);
} // namespace Utils

#endif