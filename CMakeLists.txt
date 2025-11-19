cmake_minimum_required(VERSION 3.10)
project(RevvedUp LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

include("${CMAKE_CURRENT_SOURCE_DIR}/vcpkg/scripts/buildsystems/vcpkg.cmake")

find_package(SFML COMPONENTS Network Graphics Window Audio System CONFIG REQUIRED)
find_package(imgui CONFIG REQUIRED)
find_package(ImGui-SFML CONFIG REQUIRED)

file(COPY ${PROJECT_SOURCE_DIR}/assets DESTINATION ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})
file(GLOB_RECURSE SOURCES "src/*.cpp") 

add_executable(${PROJECT_NAME} ${SOURCES}) 

target_include_directories(${PROJECT_NAME} PRIVATE include)

target_link_libraries(${PROJECT_NAME} PRIVATE SFML::Network SFML::Graphics SFML::Window SFML::Audio SFML::System)

if(DEFINED DEV_PHASE)
    target_link_libraries(${PROJECT_NAME} PRIVATE imgui::imgui ImGui-SFML::ImGui-SFML)
endif()
