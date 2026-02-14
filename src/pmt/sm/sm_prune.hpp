#pragma once

#include "pmt/sm/state_machine.hpp"

#include <optional>

namespace pmt::sm {

auto sm_prune(StateMachine const& state_machine_, StateNrType state_nr_from_, std::optional<StateNrType> state_nr_from_new_ = std::nullopt) -> StateMachine;

}  // namespace pmt::sm