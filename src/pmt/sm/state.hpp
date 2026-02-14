#pragma once

#include "pmt/container/interval_map.hpp"
#include "pmt/container/interval_set.hpp"
#include "pmt/sm/primitives.hpp"

namespace pmt::sm {

class State {
private:
 // -$ Data $-
 pmt::container::IntervalMap<SymbolType, StateNrType> _symbol_transitions;
 pmt::container::IntervalSet<StateNrType> _epsilon_transitions;
 pmt::container::IntervalSet<FinalIdType> _final_ids;

public:
 // -$ Functions $-
 // -$ Lifetime $-
 State() = default;

 // --$ Other $--
 void add_symbol_transition(SymbolType symbol_, StateNrType state_nr_);
 void add_symbol_transitions(pmt::container::Interval<SymbolType> symbol_interval_, StateNrType state_nr_);

 void remove_symbol_transition(SymbolType symbol_);
 void remove_symbol_transitions(pmt::container::Interval<SymbolType> symbol_interval_);
 void clear_symbol_transitions();

 auto get_symbol_transition(SymbolType symbol_) const -> StateNrType;
 auto get_symbol_transitions() const -> pmt::container::IntervalMap<SymbolType, StateNrType> const&;

 auto get_symbols() const -> pmt::container::IntervalSet<SymbolType>;

 void add_epsilon_transition(StateNrType state_nr_);
 void remove_epsilon_transition(StateNrType state_nr_);
 auto get_epsilon_transitions() const -> pmt::container::IntervalSet<StateNrType> const&;

 void add_final_id(FinalIdType accept_);
 void add_final_ids(pmt::container::Interval<FinalIdType> final_ids_interval_);
 void add_final_ids(pmt::container::IntervalSet<FinalIdType> const& final_ids_interval_set_);

 void remove_final_id(FinalIdType accept_);
 void remove_final_ids(pmt::container::Interval<FinalIdType> final_ids_interval_);
 void remove_final_ids(pmt::container::IntervalSet<FinalIdType> const& final_ids_interval_set_);

 auto get_final_ids() const -> pmt::container::IntervalSet<FinalIdType> const&;
};

}  // namespace pmt::sm
