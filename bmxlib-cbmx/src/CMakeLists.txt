set(CMAKE_MACOSX_RPATH 1)

add_library(cbmx
    SHARED
    version.cpp
    writer.cpp
)

set_target_properties(cbmx
    PROPERTIES
    PUBLIC_HEADER
    "../include/cbmx/version.h;../include/cbmx/writer.h;../include/cbmx/essence_type.h;"
)

target_link_libraries(cbmx "bmx-0.1")

set(CMAKE_BUILD_WITH_INSTALL_RPATH FALSE)
set(CMAKE_INSTALL_RPATH "${CMAKE_INSTALL_PREFIX}/lib")
set(CMAKE_INSTALL_RPATH_USE_LINK_PATH FALSE)

install(
    TARGETS cbmx
    LIBRARY DESTINATION "${CMAKE_INSTALL_PREFIX}/lib"
    PUBLIC_HEADER DESTINATION "${CMAKE_INSTALL_PREFIX}/include")
