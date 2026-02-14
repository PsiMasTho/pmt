#pragma once

#include "pmt/sm/state_machine.hpp"
#include "pmt/sm/primitives.hpp"

namespace pmt::sm {

class StateMachineDeterminizer {
public:
 // -$ Functions $-
 // --$ Other $--
 static auto determinize(StateMachine const& input_state_machine_, StateNrType state_nr_from_ = StateNrStart) -> StateMachine;
};

}  // namespace pmt::sm