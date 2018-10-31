set(CMAKE_MACOSX_RPATH 1)

add_library(cbmx
    SHARED
    version.cpp
    writer.cpp
)

target_link_libraries(cbmx "bmx-0.1")

set(CMAKE_BUILD_WITH_INSTALL_RPATH FALSE)
set(CMAKE_INSTALL_RPATH "${CMAKE_INSTALL_PREFIX}/lib")
set(CMAKE_INSTALL_RPATH_USE_LINK_PATH FALSE)

install(TARGETS cbmx DESTINATION "${CMAKE_INSTALL_PREFIX}/lib")
