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
        using Vec = std::vector<Ts...>;

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

        template<typename... Ts>
        using Box = std::unique_ptr<Ts...>;

        template<typename T, typename... Args>
        Box<T> MakeBox(Args &&... args) {
            return std::make_unique<std::remove_reference_t<T>>(std::forward<Args>(args)...);
        }
    }
}
