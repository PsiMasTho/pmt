#pragma once

#include "pmt/container/multi_vector.hpp"
#include "pmt/sm/primitives.hpp"
#include "pmt/sm/state.hpp"

namespace pmt::sm {

class StateMachine {
private:
 // -$ Data $-
 pmt::container::MultiVector<StateNrType, State> _states;

public:
 // -$ Functions $-
 // --$ Lifetime $--
 StateMachine() = default;
 StateMachine(StateMachine const&) = default;
 StateMachine(StateMachine&&) noexcept = default;

 // --$ Operators $--
 auto operator=(StateMachine const&) -> StateMachine& = default;
 auto operator=(StateMachine&&) noexcept -> StateMachine& = default;

 // --$ Other $--
 auto get_state(StateNrType state_nr_) -> State*;
 auto get_state(StateNrType state_nr_) const -> State const*;

 auto get_state_nrs() const -> std::span<StateNrType const>;

 auto get_or_create_state(StateNrType state_nr_) -> State&;
 auto get_unused_state_nr() const -> StateNrType;
 auto create_new_state() -> StateNrType;

 void remove_state(StateNrType state_nr_);

 auto get_state_count() const -> size_t;
};

}  // namespace pmt::sm