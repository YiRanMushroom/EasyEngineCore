module;

export module Easy.Core.Basic;

export import yrm.msvc.std.compat;

namespace Easy {
    export {
        template<typename T>
        using Arc = std::shared_ptr<T>;

        template<typename T>
        using Rc = std::shared_ptr<T>;

        template<typename... Ts>
        using DynArray = std::vector<Ts...>;

        template<typename T, size_t N>
        using Array = std::array<T, N>;
    }
}
