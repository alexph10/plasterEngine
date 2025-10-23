cmake_minimum_required(VERSION 3.16)
project(plasterEngine)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Don't set custom output directories - let Visual Studio handle it
# set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
# set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

# Find Vulkan
find_package(Vulkan REQUIRED)

# Add GLFW as subdirectory
add_subdirectory(external/glfw)

# Add GLM as subdirectory
add_subdirectory(external/glm)

# Include directories
include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${VULKAN_INCLUDE_DIR}
)

# Engine library sources
set(ENGINE_SOURCES
    src/Core/Window.cpp
    src/Core/Application.cpp
    src/Graphics/VulkanContext.cpp
    src/Graphics/Renderer.cpp
    src/Engine.cpp
)

# Create engine library
add_library(plasterEngine STATIC ${ENGINE_SOURCES})

# Link dependencies
target_link_libraries(plasterEngine PUBLIC
    Vulkan::Vulkan
    glfw
    glm::glm
)

# Include directories for library
target_include_directories(plasterEngine PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

# Main executable
add_executable(plasterEngine_app src/main.cpp)

# Link executable to library
target_link_libraries(plasterEngine_app PRIVATE plasterEngine)

# Compiler warnings
if(MSVC)
    target_compile_options(plasterEngine PRIVATE /W4)
    target_compile_options(plasterEngine_app PRIVATE /W4)
else()
    target_compile_options(plasterEngine PRIVATE -Wall -Wextra -Wpedantic)
    target_compile_options(plasterEngine_app PRIVATE -Wall -Wextra -Wpedantic)
endif()

