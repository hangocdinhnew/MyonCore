find_package(PkgConfig REQUIRED)

add_library(myongraphics SHARED)

target_sources(myongraphics
    PRIVATE
      src/myongraphics_instance.c
    PUBLIC
      include/myongraphics_enums.h
      include/myongraphics_impl.h
      include/myongraphics_structs.h
      include/myongraphics_func.h
      include/myongraphics.h
)

target_include_directories(myongraphics
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
)

pkg_check_modules(VULKAN REQUIRED IMPORTED_TARGET vulkan)

target_link_libraries(myongraphics PUBLIC PkgConfig::VULKAN)
