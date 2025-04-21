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
