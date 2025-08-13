find_package(PkgConfig REQUIRED)

add_library(myonruntime SHARED)

target_sources(myonruntime
    PRIVATE
      src/MyonRuntime/
      src/MyonRuntime/Core/Log.cpp
      src/MyonRuntime/Core/Window.cpp
      src/MyonRuntime/Core/Runtime.cpp
      src/MyonRuntime/Utils/Defines.cpp
      src/MyonRuntime/Graphics/Graphics.cpp
      src/MyonRuntime/Graphics/GraphicsInstance.cpp
      src/MyonRuntime/Graphics/GraphicsSurface.cpp
      src/MyonRuntime/Graphics/GraphicsDevice.cpp
      src/MyonRuntime/Graphics/GraphicsSwapChain.cpp
    PUBLIC
      include/MyonRuntime/Core/Log.hpp
      include/MyonRuntime/Core/Window.hpp
      include/MyonRuntime/Core/Runtime.hpp
      include/MyonRuntime/Graphics/Graphics.hpp
      include/MyonRuntime/Graphics/GraphicsInstance.hpp
      include/MyonRuntime/Graphics/GraphicsSurface.hpp
      include/MyonRuntime/Graphics/GraphicsDevice.hpp
      include/MyonRuntime/Graphics/GraphicsSwapChain.hpp
      include/MyonRuntime.hpp
)

target_include_directories(myonruntime
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
)

pkg_check_modules(spdlog REQUIRED IMPORTED_TARGET spdlog)
pkg_check_modules(vulkan REQUIRED IMPORTED_TARGET vulkan)
pkg_check_modules(sdl3 REQUIRED IMPORTED_TARGET sdl3)

target_link_libraries(myonruntime PUBLIC PkgConfig::spdlog)
target_link_libraries(myonruntime PUBLIC PkgConfig::sdl3)
target_link_libraries(myonruntime PUBLIC PkgConfig::vulkan)
