module;

#include <sol/sol.hpp>

export module Easy.Vendor.sol;

namespace sol {
    export using sol::bytecode;
    export using sol::state;
    export using sol::state_view;
    export using sol::object;
    export using sol::table;
    export using sol::function;
    export using sol::protected_function;
    export using sol::protected_function_result;
    export using sol::variadic_args;
    export using sol::variadic_results;
    export using sol::stack_object;
    export using sol::load_result;
}