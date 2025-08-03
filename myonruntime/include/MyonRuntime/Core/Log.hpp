#pragma once

// clang-format off
#include <spdlog/spdlog.h>
#include <spdlog/fmt/ostr.h>
// clang-format on

namespace Myon {

class Log {
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

#define MR_CORE_TRACE(...) ::Myon::Log::GetCoreLogger()->trace(__VA_ARGS__)
#define MR_CORE_INFO(...) ::Myon::Log::GetCoreLogger()->info(__VA_ARGS__)
#define MR_CORE_WARN(...) ::Myon::Log::GetCoreLogger()->warn(__VA_ARGS__)
#define MR_CORE_ERROR(...) ::Myon::Log::GetCoreLogger()->error(__VA_ARGS__)
#define MR_CORE_CRITICAL(...)                                                  \
  ::Myon::Log::GetCoreLogger()->critical(__VA_ARGS__)

#define MR_TRACE(...) ::Myon::Log::GetClientLogger()->trace(__VA_ARGS__)
#define MR_INFO(...) ::Myon::Log::GetClientLogger()->info(__VA_ARGS__)
#define MR_WARN(...) ::Myon::Log::GetClientLogger()->warn(__VA_ARGS__)
#define MR_ERROR(...) ::Myon::Log::GetClientLogger()->error(__VA_ARGS__)
#define MR_CRITICAL(...) ::Myon::Log::GetClientLogger()->critical(__VA_ARGS__)
