// SMT vision configuration helpers.

#pragma once

#include <string>
#include <unordered_map>

void smt_vision_apply_architecture_defaults(
        const std::string & architecture,
        std::unordered_map<std::string, std::string> & ep_config);
