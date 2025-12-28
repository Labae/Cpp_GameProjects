cmake_minimum_required(VERSION 3.20)
project(GameLibrary VERSION 1.0 LANGUAGES CXX)

# C++20 설정
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# 컴파일러 경고 설정
add_compile_options(-Wall -Wextra -Wpedantic)

# SFML 찾기
find_package(SFML 3.0 COMPONENTS Graphics Window System Audio REQUIRED)

# 소스 파일 수집
file(GLOB_RECURSE SOURCES "src/*.cpp")
file(GLOB_RECURSE HEADERS "include/*.hpp")

# Static Library 생성
add_library(${PROJECT_NAME} STATIC ${SOURCES} ${HEADERS})

# Include 디렉토리 설정
target_include_directories(${PROJECT_NAME}
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/src
)

# SFML 링크
target_link_libraries(${PROJECT_NAME}
    PUBLIC
        SFML::Graphics
        SFML::Window
        SFML::System
        SFML::Audio
)

# 정보 출력
message(STATUS "GameLibrary Configuration:")
message(STATUS "  C++ Standard: ${CMAKE_CXX_STANDARD}")
message(STATUS "  Build Type: ${CMAKE_BUILD_TYPE}")