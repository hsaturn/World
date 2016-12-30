cmake_minimum_required(VERSION 3.0)

project(world)

set(CMAKE_BUILD_TYPE Debug)
set(INSTALL_PKGCONFIG_DIR "${CMAKE_INSTALL_PREFIX}/share/pkgconfig" CACHE PATH "Installation directory for pkgconfig (.pc) files")

# Find source files
file(GLOB SOURCES core/*.cpp external/png_texture/*.cpp external/common/*.cpp)

# Include header files
include_directories(core/include external /usr/local/include/SaturnLib /usr/include/GLFW)
link_directories(/usr/local/lib/SaturnLib)

add_executable(${PROJECT_NAME} ${SOURCES})

file(COPY shaders DESTINATION .)
file(COPY textures DESTINATION .)
target_link_libraries(world SaturnLib glfw GLEW GL png)
set_property(TARGET world PROPERTY CXX_STANDARD 11)
