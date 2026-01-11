cmake_minimum_required(VERSION 3.20)
project(SnakeGame VERSION 1.0 LANGUAGES CXX)

# C++20 설정
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# 컴파일러 경고 설정
add_compile_options(-Wall -Wextra -Wpedantic)

# SFML 찾기
find_package(SFML 3.0 COMPONENTS Graphics Window System Audio REQUIRED)

# GameLibrary 찾기 (상위 디렉토리에 있다고 가정)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/../GameLibrary ${CMAKE_BINARY_DIR}/GameLibrary)

# 소스 파일 수집
file(GLOB_RECURSE SOURCES
    "src/*.cpp"
)

# 실행 파일 생성
add_executable(${PROJECT_NAME} ${SOURCES})

# Include 디렉토리 설정
target_include_directories(${PROJECT_NAME}
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/include
        ${CMAKE_CURRENT_SOURCE_DIR}/../GameLibrary/include
)

# 라이브러리 링크
target_link_libraries(${PROJECT_NAME}
    PRIVATE
        GameLibrary
        SFML::Graphics
        SFML::Window
        SFML::System
        SFML::Audio
)

# 리소스 파일 복사 (빌드 후)
add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
        ${CMAKE_CURRENT_SOURCE_DIR}/engine.yaml
        $<TARGET_FILE_DIR:${PROJECT_NAME}>/engine.yaml
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
        ${CMAKE_CURRENT_SOURCE_DIR}/game.yaml
        $<TARGET_FILE_DIR:${PROJECT_NAME}>/game.yaml
        COMMENT "Copying config files to build directory"
)

# assets 폴더가 존재할 때만 복사
if(EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/assets)
    add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_directory
            ${CMAKE_CURRENT_SOURCE_DIR}/assets
            $<TARGET_FILE_DIR:${PROJECT_NAME}>/assets
            COMMENT "Copying assets to build directory"
    )
endif()

# 정보 출력
message(STATUS "SnakeGame Configuration:")
message(STATUS "  C++ Standard: ${CMAKE_CXX_STANDARD}")
message(STATUS "  Build Type: ${CMAKE_BUILD_TYPE}")