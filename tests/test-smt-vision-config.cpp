#include "smt-vision-config.h"

#include <cassert>
#include <cstdlib>
#include <string>
#include <unordered_map>

int main() {
    static const char * const disable_float16_epilogue = "SPACEMIT_EP_DISABLE_FLOAT16_EPILOGUE";
    static const char * const disable_op_type_filter   = "SPACEMIT_EP_DISABLE_OP_TYPE_FILTER";

    {
#if defined(_WIN32)
        _putenv_s(disable_float16_epilogue, "");
        _putenv_s(disable_op_type_filter, "");
#else
        unsetenv(disable_float16_epilogue);
        unsetenv(disable_op_type_filter);
#endif
        std::unordered_map<std::string, std::string> ep_config;
        smt_vision_apply_architecture_defaults("Qwen3VL", ep_config);
        assert(ep_config.at(disable_float16_epilogue) == "1");
        assert(ep_config.at(disable_op_type_filter) == "MatMul;Gemm");
        assert(std::string(std::getenv(disable_float16_epilogue)) == "1");
        assert(std::string(std::getenv(disable_op_type_filter)) == "MatMul;Gemm");
    }

    {
#if defined(_WIN32)
        _putenv_s(disable_float16_epilogue, "0");
        _putenv_s(disable_op_type_filter, "Conv");
#else
        setenv(disable_float16_epilogue, "0", 1);
        setenv(disable_op_type_filter, "Conv", 1);
#endif
        std::unordered_map<std::string, std::string> ep_config;
        smt_vision_apply_architecture_defaults("Qwen3VL", ep_config);
        assert(ep_config.at(disable_float16_epilogue) == "0");
        assert(ep_config.at(disable_op_type_filter) == "Conv;MatMul;Gemm");
        assert(std::string(std::getenv(disable_float16_epilogue)) == "0");
        assert(std::string(std::getenv(disable_op_type_filter)) == "Conv;MatMul;Gemm");
    }

    {
        std::unordered_map<std::string, std::string> ep_config = {
            { disable_float16_epilogue, "0" },
            { disable_op_type_filter, "Gemm;Add" },
        };
        smt_vision_apply_architecture_defaults("Qwen3VL", ep_config);
        assert(ep_config.at(disable_float16_epilogue) == "0");
        assert(ep_config.at(disable_op_type_filter) == "Gemm;Add;MatMul");
    }

    {
#if defined(_WIN32)
        _putenv_s(disable_op_type_filter, "");
#else
        unsetenv(disable_op_type_filter);
#endif
        std::unordered_map<std::string, std::string> ep_config;
        smt_vision_apply_architecture_defaults("LlavaQwen2ForCausalLM", ep_config);
        assert(ep_config.find(disable_float16_epilogue) == ep_config.end());
        assert(ep_config.find(disable_op_type_filter) == ep_config.end());
    }

    return 0;
}
