#pragma once

// clang-format off
#include <cstdlib>

#include <spdlog/spdlog.h>
#include <spdlog/fmt/ostr.h>
// clang-format on

namespace MyonR {

    class Log
    {
    public:
        static void Init();

        static std::shared_ptr<spdlog::logger> &GetCoreLogger() {
            return s_CoreLogger;
        }
        static std::shared_ptr<spdlog::logger> &GetClientLogger() {
            return s_ClientLogger;
        }

    private:
        static std::shared_ptr<spdlog::logger> s_CoreLogger;
        static std::shared_ptr<spdlog::logger> s_ClientLogger;
    };

} // namespace MyonRuntime

#define MR_CORE_TRACE(...) ::MyonR::Log::GetCoreLogger()->trace(__VA_ARGS__)
#define MR_CORE_DEBUG(...) ::MyonR::Log::GetCoreLogger()->debug(__VA_ARGS__)
#define MR_CORE_INFO(...) ::MyonR::Log::GetCoreLogger()->info(__VA_ARGS__)
#define MR_CORE_WARN(...) ::MyonR::Log::GetCoreLogger()->warn(__VA_ARGS__)
#define MR_CORE_ERROR(...) ::MyonR::Log::GetCoreLogger()->error(__VA_ARGS__)
#define MR_CORE_CRITICAL(...)                                                  \
    ::MyonR::Log::GetCoreLogger()->critical(__VA_ARGS__)

#define MR_TRACE(...) ::MyonR::Log::GetClientLogger()->trace(__VA_ARGS__)
#define MR_DEBUG(...) ::MyonR::Log::GetClientLogger()->debug(__VA_ARGS__)
#define MR_INFO(...) ::MyonR::Log::GetClientLogger()->info(__VA_ARGS__)
#define MR_WARN(...) ::MyonR::Log::GetClientLogger()->warn(__VA_ARGS__)
#define MR_ERROR(...) ::MyonR::Log::GetClientLogger()->error(__VA_ARGS__)
#define MR_CRITICAL(...) ::MyonR::Log::GetClientLogger()->critical(__VA_ARGS__)

#define MR_CORE_ASSERT(cond, msg, ...)                                          \
    do {                                                                          \
        if (!(cond)) {                                                              \
        ::MyonR::Log::GetCoreLogger()->critical("[ASSERT] " msg, ##__VA_ARGS__);  \
        std::abort();                                                             \
        }                                                                           \
    } while (false)

#define MR_ASSERT(cond, msg, ...)                                               \
    do {                                                                          \
        if (!(cond)) {                                                              \
        ::MyonR::Log::GetClientLogger()->critical("[ASSERT] " msg, ##__VA_ARGS__);\
        std::abort();                                                             \
        }                                                                           \
    } while (false)

#define MR_DO_CORE_ASSERT(msg, ...)                                             \
    do {                                                                          \
        ::MyonR::Log::GetCoreLogger()->critical("[ASSERT] " msg, ##__VA_ARGS__);    \
            std::abort();                                                               \
    } while (false)

#define MR_DO_ASSERT(msg, ...)                                                 \
    do {                                                                         \
        ::MyonR::Log::GetClientLogger()->critical("[ASSERT] " msg, ##__VA_ARGS__); \
        std::abort();                                                              \
    } while (false)
