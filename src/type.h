#pragma once

#include <bitset>
#include <cinttypes>

const std::uint8_t MAX_COMPONENTS = 4;
using ComponentBitField = std::bitset<MAX_COMPONENTS>;
using ComponentTypeId = std::uint64_t;
using EntityIdType = std::uint64_t;
