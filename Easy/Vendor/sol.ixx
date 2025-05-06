module;

#include <utility>
#include <memory>
#include <filesystem>
#include <string>
#include <string_view>
#include <functional>
#include <optional>
#include <iostream>

export module Easy.Vendor.sol;

export {
#include <sol/forward.hpp>
#include <sol/forward_detail.hpp>
#include <sol/assert.hpp>
#include <sol/bytecode.hpp>
#include <sol/stack.hpp>
#include <sol/object.hpp>
#include <sol/function.hpp>
#include <sol/protected_function.hpp>
#include <sol/usertype.hpp>
#include <sol/table.hpp>
#include <sol/state.hpp>
#include <sol/coroutine.hpp>
#include <sol/thread.hpp>
#include <sol/userdata.hpp>
#include <sol/metatable.hpp>
#include <sol/as_args.hpp>
#include <sol/variadic_args.hpp>
#include <sol/variadic_results.hpp>
#include <sol/lua_value.hpp>
}