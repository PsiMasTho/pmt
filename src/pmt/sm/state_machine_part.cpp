#include "pmt/sm/state_machine_part.hpp"

#include "pmt/sm/state_machine.hpp"

namespace pmt::sm {
using namespace pmt::container;

StateMachinePart::StateMachinePart(StateNrType incoming_state_nr_)
 : _incoming_state_nr(incoming_state_nr_) {
}

void StateMachinePart::set_incoming_state_nr(StateNrType incoming_state_nr_) {
 _incoming_state_nr = incoming_state_nr_;
}

auto StateMachinePart::get_incoming_state_nr() const -> std::optional<StateNrType> {
 return _incoming_state_nr;
}

void StateMachinePart::clear_incoming_state_nr() {
 _incoming_state_nr.reset();
}

void StateMachinePart::add_outgoing_symbol_transition(StateNrType state_nr_from_, SymbolType symbol_) {
 _outgoing_symbol_transitions[state_nr_from_].insert(Interval(symbol_));
}

void StateMachinePart::add_outgoing_symbol_transition(StateNrType state_nr_from_, Interval<SymbolType> interval_) {
 _outgoing_symbol_transitions[state_nr_from_].insert(interval_);
}

void StateMachinePart::add_outgoing_epsilon_transition(StateNrType state_nr_from_) {
 _outgoing_epsilon_transitions.insert(Interval(state_nr_from_));
}

auto StateMachinePart::get_state_nrs_with_outgoing_symbol_transitions() const -> IntervalSet<StateNrType> {
 IntervalSet<StateNrType> ret;
 for (auto const& [state_nr_from, _] : _outgoing_symbol_transitions) {
  ret.insert(Interval(state_nr_from));
 }
 return ret;
}

auto StateMachinePart::get_outgoing_symbol_transitions(StateNrType state_nr_from_) const -> IntervalSet<SymbolType> const& {
 if (auto const itr = _outgoing_symbol_transitions.find(state_nr_from_); itr != _outgoing_symbol_transitions.end()) {
  return itr->second;
 }
 static const IntervalSet<SymbolType> empty;
 return empty;
}

auto StateMachinePart::get_outgoing_epsilon_transitions() const -> IntervalSet<StateNrType> const& {
 return _outgoing_epsilon_transitions;
}

void StateMachinePart::clear_outgoing_transitions() {
 _outgoing_symbol_transitions.clear();
 _outgoing_epsilon_transitions.clear();
}

void StateMachinePart::merge_outgoing_transitions(StateMachinePart& other_) {
 for (auto& [state_nr_from, symbols] : other_._outgoing_symbol_transitions) {
  _outgoing_symbol_transitions[state_nr_from].inplace_or(symbols);
 }

 _outgoing_epsilon_transitions.inplace_or(other_._outgoing_epsilon_transitions);

 other_.clear_outgoing_transitions();
}

void StateMachinePart::connect_outgoing_transitions_to(StateNrType state_nr_to_, StateMachine& state_machine_) {
 state_machine_.get_or_create_state(state_nr_to_);

 for (auto const& [state_nr_from, symbols] : _outgoing_symbol_transitions) {
  State& state_from = state_machine_.get_or_create_state(state_nr_from);
  symbols.for_each_interval([&](Interval<SymbolType> const& interval_) { state_from.add_symbol_transitions(interval_, state_nr_to_); });
 }

 _outgoing_epsilon_transitions.for_each_key([&](StateNrType state_nr_from_) { state_machine_.get_or_create_state(state_nr_from_).add_epsilon_transition(state_nr_to_); });

 clear_outgoing_transitions();
}

}  // namespace pmt::sm