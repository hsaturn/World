cmake_minimum_required(VERSION 2.4.0)

project(world)

set(CMAKE_BUILD_TYPE Debug)
set(INSTALL_PKGCONFIG_DIR "${CMAKE_INSTALL_PREFIX}/share/pkgconfig" CACHE PATH "Installation directory for pkgconfig (.pc) files")

# Find source files
file(GLOB SOURCES src/*.cpp)

# Include header files
include_directories(include /usr/local/include/SaturnLib)
link_directories(/usr/local/lib/SaturnLib)

# Create shared library
add_executable(${PROJECT_NAME} ${SOURCES})

# Install library
# install(TARGETS ${PROJECT_NAME} DESTINATION lib/${PROJECT_NAME})

# Install library headers
# file(GLOB HEADERS include/*.h)
# install(FILES ${HEADERS} DESTINATION include/${PROJECT_NAME})

target_link_libraries(world SaturnLib)
