find_package(PkgConfig REQUIRED)

add_library(myongraphics SHARED)

if(APPLE)
  option(MYONG_METAL "Enable Metal backend for MyonGraphics" ON)
else()
  option(MYONG_METAL "Enable Metal backend for MyonGraphics" OFF)
endif()

target_sources(myongraphics
    PRIVATE
      include/myongraphics_internal.h
      src/myongraphics_instance.c
    PUBLIC
      include/myongraphics_enums.h
      include/myongraphics_impl.h
      include/myongraphics_structs.h
      include/myongraphics_func.h
      include/myongraphics.h
)

if(MYONG_METAL)
  message(STATUS "Metal backend enabled")

  set(CMAKE_OBJC_FLAGS "${CMAKE_OBJC_FLAGS} -fobjc-arc")

  enable_language(OBJC)

  target_include_directories(myongraphics
    PRIVATE
      "${METAL_INCLUDE_DIR}"
  )

  target_sources(myongraphics
    PUBLIC
      include/metal/myon_metaldevice.h
      include/metal/myon_metal.h
  )

  target_link_libraries(myongraphics PUBLIC "-framework Metal")

  target_compile_definitions(myongraphics PUBLIC MYONG_METAL=1)
endif()

target_include_directories(myongraphics
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
)

pkg_check_modules(VULKAN REQUIRED IMPORTED_TARGET vulkan)

target_link_libraries(myongraphics PUBLIC PkgConfig::VULKAN)
