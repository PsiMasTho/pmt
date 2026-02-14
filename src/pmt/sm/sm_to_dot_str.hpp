#pragma once

#include "pmt/fw_decl.hpp"
#include "pmt/sm/primitives.hpp"

#include <functional>
#include <string>

PMT_FW_DECL_NS_CLASS(pmt::sm, StateMachine)

namespace pmt::sm {

static inline char const state_machine_dotfile_skel_path_rel[] = "dot/state_machine-skel.dot";

using FinalIdToStringFn = std::function<std::string(FinalIdType)>;
auto sm_to_dot_str(pmt::sm::StateMachine const& state_machine_, FinalIdToStringFn const& final_id_to_str_, std::string state_machine_dotfile_skel_) -> std::string;

}  // namespace pmt::sm
