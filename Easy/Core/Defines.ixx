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

        template<typename Tp, typename... Args>
        Arc<Tp> MakeArc(Args &&... args) {
            return std::make_shared<std::remove_reference_t<Tp>>(std::forward<Args>(args)...);
        }

        template<typename Tp, typename... Args>
        Rc<Tp> MakeRc(Args &&... args) {
            return std::make_shared<std::remove_reference_t<Tp>>(std::forward<Args>(args)...);
        }
    }
}
