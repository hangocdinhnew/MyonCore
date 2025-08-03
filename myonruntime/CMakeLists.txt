find_package(PkgConfig REQUIRED)

add_library(myonruntime SHARED)

target_sources(myonruntime
    PRIVATE
      src/MyonRuntime/core/Log.cpp
      src/MyonRuntime/core/Window.cpp
      src/MyonRuntime/core/Runtime.cpp
    PUBLIC
      include/MyonRuntime/core/Log.hpp
      include/MyonRuntime/core/Window.hpp
      include/MyonRuntime/core/Runtime.hpp
      include/MyonRuntime.hpp
)

target_include_directories(myonruntime
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
)

pkg_check_modules(spdlog REQUIRED IMPORTED_TARGET spdlog)
pkg_check_modules(SDL3 REQUIRED IMPORTED_TARGET sdl3)

target_link_libraries(myonruntime PUBLIC PkgConfig::spdlog)
target_link_libraries(myonruntime PUBLIC PkgConfig::SDL3)
