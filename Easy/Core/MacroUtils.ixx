module;

#include <spdlog/spdlog.h>
#include <spdlog/fmt/ostr.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/sinks/basic_file_sink.h>

export module Easy.Core.Log;

import Easy.Core.Basic;

namespace Easy {
    export class MacroUtils
    {
    public:
        static void Init() {
            std::vector<spdlog::sink_ptr> logSinks;
            logSinks.emplace_back(std::make_shared<spdlog::sinks::stdout_color_sink_mt>());
            logSinks.emplace_back(std::make_shared<spdlog::sinks::basic_file_sink_mt>("Easy.log", true));

            logSinks[0]->set_pattern("%^[%T] %n: %v%$");
            logSinks[1]->set_pattern("[%T] [%l] %n: %v");

            s_CoreLogger = std::make_shared<spdlog::logger>("Easy", begin(logSinks), end(logSinks));
            spdlog::register_logger(s_CoreLogger);
            s_CoreLogger->set_level(spdlog::level::trace);
            s_CoreLogger->flush_on(spdlog::level::trace);

            s_ClientLogger = std::make_shared<spdlog::logger>("APP", begin(logSinks), end(logSinks));
            spdlog::register_logger(s_ClientLogger);
            s_ClientLogger->set_level(spdlog::level::trace);
            s_ClientLogger->flush_on(spdlog::level::trace);
        }

        static Arc<spdlog::logger>& GetCoreLogger() { return s_CoreLogger; }
        static Arc<spdlog::logger>& GetClientLogger() { return s_ClientLogger; }
    private:
        inline static Arc<spdlog::logger> s_CoreLogger;
        inline static Arc<spdlog::logger> s_ClientLogger;
    };
}