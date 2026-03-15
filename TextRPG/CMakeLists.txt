cmake_minimum_required(VERSION 3.20)
project(TextRPG VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_compile_options(-Wall -Wextra -Wpedantic)

find_package(SFML 3.0 COMPONENTS Graphics Window System Audio REQUIRED)

add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/../GameLibrary ${CMAKE_BINARY_DIR}/GameLibrary)

file(GLOB_RECURSE SOURCES "src/*.cpp")

add_executable(${PROJECT_NAME} ${SOURCES})

target_include_directories(${PROJECT_NAME}
        PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/include
        ${CMAKE_CURRENT_SOURCE_DIR}/../GameLibrary/include
)

target_link_libraries(${PROJECT_NAME}
        PRIVATE
        GameLibrary
        SFML::Graphics
        SFML::Window
        SFML::System
        SFML::Audio
)

# config 복사
add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
        ${CMAKE_CURRENT_SOURCE_DIR}/engine.yaml
        $<TARGET_FILE_DIR:${PROJECT_NAME}>/engine.yaml
        COMMENT "Copying config files to build directory"
)

# assets 복사
if(EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/assets)
    add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_directory
            ${CMAKE_CURRENT_SOURCE_DIR}/assets
            $<TARGET_FILE_DIR:${PROJECT_NAME}>/assets
            COMMENT "Copying assets to build directory"
    )
endif()

# data 복사
if(EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/data)
    add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_directory
            ${CMAKE_CURRENT_SOURCE_DIR}/data
            $<TARGET_FILE_DIR:${PROJECT_NAME}>/data
            COMMENT "Copying data files to build directory"
    )
endif()

message(STATUS "TextRPG Configuration:")
message(STATUS "  C++ Standard: ${CMAKE_CXX_STANDARD}")
message(STATUS "  Build Type: ${CMAKE_BUILD_TYPE}")