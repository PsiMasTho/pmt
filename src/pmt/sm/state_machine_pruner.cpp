#include "pmt/sm/state_machine_pruner.hpp"

#include "pmt/sm/state_machine.hpp"

#include <unordered_map>
#include <vector>

namespace pmt::sm {
using namespace pmt::container;

namespace {
class Locals {
public:
 std::unordered_map<StateNrType, StateNrType> _old_to_new;
 std::vector<StateNrType> _pending;
};

auto push_and_visit(StateMachinePruner::Args& args_, Locals& locals_, StateNrType state_nr_) {
 if (auto const itr = locals_._old_to_new.find(state_nr_); itr != locals_._old_to_new.end()) {
  return;
 }

 StateNrType const state_nr_new = args_._state_nr_from_new.value_or(state_nr_);
 args_._state_nr_from_new = args_._state_nr_from_new.has_value() ? std::make_optional(state_nr_new + 1) : std::nullopt;

 locals_._pending.push_back(state_nr_);
 locals_._old_to_new.emplace(state_nr_, state_nr_new);
 return;
}

auto take(Locals& locals_) -> StateNrType {
 StateNrType const ret = locals_._pending.back();
 locals_._pending.pop_back();
 return ret;
}

void follow_epsilon_transitions(StateMachinePruner::Args& args_, Locals& locals_, State& state_old_) {
 IntervalSet<StateNrType> to_remove;

 state_old_.get_epsilon_transitions().for_each_key([&](StateNrType state_nr_next_old_) {
  if (args_._state_machine.get_state(state_nr_next_old_) == nullptr) {
   to_remove.insert(Interval(state_nr_next_old_));
  } else {
   push_and_visit(args_, locals_, state_nr_next_old_);
  }
 });

 to_remove.for_each_key([&](StateNrType state_nr_to_remove_) { state_old_.remove_epsilon_transition(state_nr_to_remove_); });
}

void follow_symbol_transitions(StateMachinePruner::Args& args_, Locals& locals_, State& state_old_) {
 std::vector<Interval<SymbolType>> to_remove;

 state_old_.get_symbol_transitions().for_each_interval([&](StateNrType state_nr_next_old_, Interval<SymbolType> const& interval_) {
  if (args_._state_machine.get_state(state_nr_next_old_) == nullptr) {
   to_remove.push_back(interval_);
  } else {
   push_and_visit(args_, locals_, state_nr_next_old_);
  }
 });

 while (!to_remove.empty()) {
  Interval<SymbolType> interval = to_remove.back();
  state_old_.remove_symbol_transitions(interval);
  to_remove.pop_back();
 }
}

void copy_epsilon_transitions(Locals& locals_, StateMachine& state_machine_new_, State const& state_old_, State& state_new_) {
 state_old_.get_epsilon_transitions().for_each_key([&](StateNrType state_nr_next_old_) { state_new_.add_epsilon_transition(locals_._old_to_new.find(state_nr_next_old_)->second); });
}

void copy_symbol_transitions(Locals& locals_, StateMachine& state_machine_new_, State const& state_old_, State& state_new_) {
 state_old_.get_symbol_transitions().for_each_interval([&](StateNrType state_nr_next_old_, Interval<SymbolType> const& interval_) { state_new_.add_symbol_transitions(interval_, locals_._old_to_new.find(state_nr_next_old_)->second); });
}

}  // namespace

void StateMachinePruner::prune(Args args_) {
 Locals locals;
 push_and_visit(args_, locals, args_._state_nr_from);

 while (!locals._pending.empty()) {
  StateNrType const state_nr_old = take(locals);
  State& state_old = *args_._state_machine.get_state(state_nr_old);

  follow_epsilon_transitions(args_, locals, state_old);
  follow_symbol_transitions(args_, locals, state_old);
 }

 // Remove states that were not visited
 for (StateNrType const state_nr : args_._state_machine.get_state_nrs()) {
  if (locals._old_to_new.find(state_nr) != locals._old_to_new.end()) {
   continue;
  }
  args_._state_machine.remove_state(state_nr);
 }

 // Renumber states if necessary
 if (args_._state_nr_from_new.has_value()) {
  StateMachine state_machine_new;
  for (auto const& [state_nr_old, state_nr_new] : locals._old_to_new) {
   State const* state_old = args_._state_machine.get_state(state_nr_old);
   State& state_new = state_machine_new.get_or_create_state(state_nr_new);

   copy_epsilon_transitions(locals, state_machine_new, *state_old, state_new);
   copy_symbol_transitions(locals, state_machine_new, *state_old, state_new);
   state_new.add_final_ids(state_old->get_final_ids());
  }

  // Replace the old state machine with the new one
  args_._state_machine = std::move(state_machine_new);
 }
}

}  // namespace pmt::sm