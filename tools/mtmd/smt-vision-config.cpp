// SMT vision configuration helpers.

#include "smt-vision-config.h"

#include <cstdlib>

namespace {

static bool smt_vision_ep_filter_contains(const std::string & filter, const std::string & op_type) {
    size_t begin = 0;
    while (begin <= filter.size()) {
        size_t end = filter.find(';', begin);
        if (end == std::string::npos) {
            end = filter.size();
        }
        if (filter.substr(begin, end - begin) == op_type) {
            return true;
        }
        begin = end + 1;
    }
    return false;
}

static void smt_vision_ep_filter_append(std::string & filter, const std::string & op_type) {
    if (smt_vision_ep_filter_contains(filter, op_type)) {
        return;
    }
    if (!filter.empty() && filter.back() != ';') {
        filter += ';';
    }
    filter += op_type;
}

static void smt_vision_set_env_if_unset(const char * name, const std::string & value) {
    const char * env_value = std::getenv(name);
    if (env_value && env_value[0] != '\0') {
        return;
    }
#if defined(_WIN32)
    _putenv_s(name, value.c_str());
#else
    setenv(name, value.c_str(), 0);
#endif
}

static void smt_vision_set_env(const char * name, const std::string & value) {
#if defined(_WIN32)
    _putenv_s(name, value.c_str());
#else
    setenv(name, value.c_str(), 1);
#endif
}

}  // namespace

void smt_vision_apply_architecture_defaults(
        const std::string & architecture,
        std::unordered_map<std::string, std::string> & ep_config) {
    static const char * const disable_float16_epilogue = "SPACEMIT_EP_DISABLE_FLOAT16_EPILOGUE";
    static const char * const disable_op_type_filter   = "SPACEMIT_EP_DISABLE_OP_TYPE_FILTER";

    if (architecture == "Qwen3VL" && ep_config.find(disable_float16_epilogue) == ep_config.end()) {
        const char * env_value = std::getenv(disable_float16_epilogue);
        if (env_value && env_value[0] != '\0') {
            ep_config[disable_float16_epilogue] = env_value;
        } else {
            ep_config[disable_float16_epilogue] = "1";
            smt_vision_set_env_if_unset(disable_float16_epilogue, "1");
        }
    }

    if (architecture == "Qwen3VL") {
        std::string op_type_filter;
        if (auto it = ep_config.find(disable_op_type_filter); it != ep_config.end()) {
            op_type_filter = it->second;
        } else if (const char * env_value = std::getenv(disable_op_type_filter)) {
            op_type_filter = env_value;
        }
        smt_vision_ep_filter_append(op_type_filter, "MatMul");
        smt_vision_ep_filter_append(op_type_filter, "Gemm");
        ep_config[disable_op_type_filter] = op_type_filter;
        smt_vision_set_env(disable_op_type_filter, op_type_filter);
    }
}
