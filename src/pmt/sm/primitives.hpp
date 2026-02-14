#pragma once

#include <cstdint>
#include <limits>

namespace pmt::sm {

using SymbolType = uintmax_t;
using StateNrType = uintmax_t;
using FinalIdType = uintmax_t;

enum : StateNrType {
 StateNrStart = 0,
 StateNrInvalid = std::numeric_limits<StateNrType>::max(),
};

}  // namespace pmt::rt
