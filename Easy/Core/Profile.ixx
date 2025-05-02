module;
#include <cstring>

export module Easy.Core.Profile;

import Easy.Core.Basic;
import Easy.Core.Util;

namespace Easy {
    export template<typename CallBackFn> requires
        requires(CallBackFn fn, std::chrono::microseconds dur) {
            fn(dur);
        }
    class ScopedTimerWithCallBack {
    public:
        using function_type = CallBackFn;

    public:
        explicit ScopedTimerWithCallBack(CallBackFn fn)
            : m_CallBack(fn), m_StartTime(std::chrono::high_resolution_clock::now()) {}

        ~ScopedTimerWithCallBack() {
            auto endTime = std::chrono::high_resolution_clock::now();
            auto duration = std::chrono::duration_cast<std::chrono::microseconds>(endTime - m_StartTime);
            m_CallBack(duration);
        }

    private:
        function_type m_CallBack;
        std::chrono::time_point<std::chrono::high_resolution_clock> m_StartTime;
    };

    export template<typename Fn>
    auto Profile(Fn &&fn) {
        return ScopedTimerWithCallBack<Fn>(std::forward<Fn>(fn));
    }

    export struct ProfileInfo {
        std::array<std::string_view, 16> StrToCat{};
        std::chrono::microseconds Duration{};

        template<size_t N> requires (N > 64) // 64 is the minimum size for a string
        void writeTo(char (&buffer)[N]) const {
            // use safe string copy
            size_t offset = sprintf(buffer, "Duration: %lld us From: ", Duration.count());
            for (const auto &str: StrToCat) {
                if (offset + str.size() < N) {
                    memcpy(buffer + offset, str.data(), str.size());
                    offset += str.size();
                } else {
                    break;
                }
            }
            buffer[offset] = '\0'; // null-terminate the string
        }
    };

    export std::vector<ProfileInfo> g_LastProfileInfos;
    export std::vector<ProfileInfo> g_ProfileInfos;

    export void StartProfiling() {
        g_ProfileInfos.clear();
    }

    export void (*g_ProfileFunctionCallback)(std::vector<ProfileInfo> &) = [
            ](std::vector<ProfileInfo> &infos) {
        std::swap(g_LastProfileInfos, infos);
    };


    export void EndProfiling() {
        g_ProfileFunctionCallback(g_ProfileInfos);
    }
}
