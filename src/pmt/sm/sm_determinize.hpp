#pragma once

#include "pmt/sm/state_machine.hpp"

namespace pmt::sm {

auto sm_determinize(StateMachine const& state_machine_, StateNrType state_nr_from_) -> StateMachine;

}  // namespace pmt::sm