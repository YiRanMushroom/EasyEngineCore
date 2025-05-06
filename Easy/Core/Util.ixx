export module Easy.Core.Util;

import Easy.Core.Basic;

namespace Easy {
    export class SteadyFrameTimer {
    public:
        inline static std::chrono::duration<float> s_MinDeltaTime = std::chrono::duration<float>(1.f / 4096.f);
        // 4096 FPS
        inline static std::chrono::duration<float> s_MaxDeltaTime = std::chrono::duration<float>(100.f); // 100s
        inline static std::chrono::duration<float> s_DefaultDeltaTime = std::chrono::duration<float>(1.f / 60.f);
        // 60 FPS

        template<typename T = std::chrono::duration<float>>
        T GetDeltaTime() {
            auto now = std::chrono::steady_clock::now();
            auto deltaTime = now - m_LastFrameTime;
            m_LastFrameTime = now;

            if (deltaTime < s_MinDeltaTime)
                return s_MinDeltaTime;
            if (deltaTime > s_MaxDeltaTime)
                return s_DefaultDeltaTime;

            return std::chrono::duration_cast<T>(deltaTime);
        }

    private:
        std::chrono::steady_clock::time_point m_LastFrameTime;
    };

    export template<typename BuilderType>
    class Builder {
        // public:
        // using ValueType = typename BuilderType::ValueType;

    protected:
        BuilderType &&Next() {
            return std::move(static_cast<BuilderType &&>(*this));
        }

    private:
        BuilderType &AsBuilder() {
            return static_cast<BuilderType &>(*this);
        }
    };
}

namespace Easy {
    export class fn_nothing {
        void operator()(const auto &...) const {}
    };

    export template<typename T, typename DeleteType = fn_nothing, T default_value = T{}>
    class unique_owner_default {
    public:
        using value_type = T;
        using delete_type = DeleteType;

        constexpr unique_owner_default() : value{default_value} {}

        constexpr static unique_owner_default from_exchanged(value_type &&value) {
            return unique_owner_default{std::exchange(value, default_value)};
        }

        constexpr static unique_owner_default from_value(const value_type &value) {
            return unique_owner_default{value};
        }

        constexpr unique_owner_default(const unique_owner_default &) = delete;

        constexpr unique_owner_default(unique_owner_default &&other) noexcept : value{other.value} {
            other.value = default_value;
        }

        constexpr unique_owner_default &operator=(const unique_owner_default &) = delete;

        constexpr unique_owner_default &operator=(unique_owner_default &&other) noexcept {
            if (this == &other) {
                return *this;
            }

            this->reset();

            this->value = std::exchange(other.value, default_value);

            return *this;
        }

        constexpr auto operator<=>(const unique_owner_default &) const = default;

        constexpr const value_type &get() const {
            return value;
        }

        constexpr value_type &get() {
            return value;
        }

        [[nodiscard]] constexpr bool has_value() const {
            return value != default_value;
        }

        constexpr explicit operator bool() const { // NOLINT
            return has_value();
        }

        constexpr void reset() {
            if constexpr (!(std::is_same_v<delete_type, fn_nothing> || std::is_same_v<delete_type, void>
                            || std::is_same_v<delete_type, nullptr_t>)) {
                if (value != default_value) {
                    delete_type{}(std::exchange(value, default_value));
                }
            }
        }

        constexpr ~unique_owner_default() {
            reset();
        }

        constexpr unique_owner_default(value_type value) : value{value} {}

    private:
        value_type value;
    };

    export template<typename T, typename DeleteType = fn_nothing>
    class unique_owner_optional {
    public:
        using value_type = T;
        using delete_type = DeleteType;

        constexpr unique_owner_optional() = default;

        constexpr static unique_owner_optional from_exchanged(value_type &&value) {
            return unique_owner_optional{std::exchange(value, std::nullopt)};
        }

        constexpr static unique_owner_optional from_value(const value_type &value) {
            return unique_owner_optional{value};
        }

        constexpr unique_owner_optional(const unique_owner_optional &) = delete;

        constexpr unique_owner_optional(unique_owner_optional &&other) noexcept : value{
            std::exchange(other.value, std::optional<value_type>{})
        } {}

        constexpr unique_owner_optional &operator=(const unique_owner_optional &) = delete;

        constexpr unique_owner_optional &operator=(unique_owner_optional &&other) noexcept {
            if (this == &other) {
                return *this;
            }

            this->reset();

            value = std::exchange(other.value, std::optional<value_type>{});

            return *this;
        }

        constexpr auto operator<=>(const unique_owner_optional &) const = default;

        constexpr const value_type &get() const {
            return *value;
        }

        constexpr bool has_value() const {
            return value.has_value();
        }

        constexpr explicit operator bool() const {
            return has_value();
        }

        constexpr void reset() {
            if (value.has_value()) {
                if constexpr (!(std::is_same_v<delete_type, fn_nothing> || std::is_same_v<delete_type, void>
                                || std::is_same_v<delete_type, nullptr_t>)) {
                    delete_type{}(*std::exchange(value, std::optional<value_type>{}));
                }
            }
        }

        constexpr unique_owner_optional(value_type value) : value{value} {}

    private:
        std::optional<value_type> value;
    };

    export constexpr bool create_directory(const std::filesystem::path &path) {
        // if directory is "" return true
        if (path.empty()) {
            return true;
        }
        if (std::filesystem::exists(path)) {
            return true;
        }
        return std::filesystem::create_directories(path);
    }

    export template<size_t v>
    constexpr size_t StringLenOf = []() {
        size_t res = 0;
        size_t value = v;
        while (value > 0) {
            value /= 10;
            res++;
        }
        return res;
    }();

    export template<size_t v>
    constexpr auto SizeTToString = []() {
        std::array<char, StringLenOf<v> + 1> buffer{0};
        size_t value = v;
        size_t index = StringLenOf<v>;
        buffer[index] = '\0';
        while (value > 0) {
            --index;
            buffer[index] = '0' + (value % 10);
            value /= 10;
        }
        return buffer;
    }();

    export template<size_t N>
    struct StringLiteral {
        char Data[N];

        constexpr size_t Length() const noexcept {
            return N - 1;
        }

        consteval StringLiteral(const char (&str)[N]) : Data{} { // NOLINT
            std::copy_n(str, N, Data);
        }

        consteval StringLiteral() : Data{} {}

        template<size_t OtherSize>
        constexpr auto operator+(const StringLiteral<OtherSize> &other) const noexcept {
            constexpr size_t newSize = N + OtherSize - 1;
            StringLiteral<newSize> result{};
            std::copy_n(Data, N - 1, result.Data);
            std::copy_n(other.Data, OtherSize, result.Data + N - 1);
            return result;
        }
    };
}
