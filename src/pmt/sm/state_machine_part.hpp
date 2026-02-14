#pragma once

#include "pmt/container/interval_set.hpp"
#include "pmt/fw_decl.hpp"
#include "pmt/sm/primitives.hpp"

#include <optional>

PMT_FW_DECL_NS_CLASS(pmt::sm, StateMachine)

namespace pmt::sm {

class StateMachinePart {
private:
 // -$ Data $-
 std::optional<StateNrType> _incoming_state_nr;
 std::unordered_map<StateNrType, pmt::container::IntervalSet<SymbolType>> _outgoing_symbol_transitions;
 pmt::container::IntervalSet<StateNrType> _outgoing_epsilon_transitions;

public:
 // -$ Functions $-
 // --$ Lifetime $--
 StateMachinePart() = default;
 explicit StateMachinePart(StateNrType incoming_state_nr_);

 // --$ Other $--
 void set_incoming_state_nr(StateNrType incoming_state_nr_);
 auto get_incoming_state_nr() const -> std::optional<StateNrType>;
 void clear_incoming_state_nr();

 void add_outgoing_symbol_transition(StateNrType state_nr_from_, SymbolType symbol_);
 void add_outgoing_symbol_transition(StateNrType state_nr_from_, pmt::container::Interval<SymbolType> interval_);
 void add_outgoing_epsilon_transition(StateNrType state_nr_from_);

 auto get_state_nrs_with_outgoing_symbol_transitions() const -> pmt::container::IntervalSet<StateNrType>;
 auto get_outgoing_symbol_transitions(StateNrType state_nr_from_) const -> pmt::container::IntervalSet<SymbolType> const&;

 auto get_outgoing_epsilon_transitions() const -> pmt::container::IntervalSet<StateNrType> const&;

 void clear_outgoing_transitions();
 void merge_outgoing_transitions(StateMachinePart& other_);

 void connect_outgoing_transitions_to(StateNrType state_nr_to_, StateMachine& state_machine_);
};

}  // namespace pmt::sm