cmake_minimum_required(VERSION 3.24.2)

project(TheSnakeGame LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# ------------------------------------
# Project paths
# ------------------------------------
set(ProjectPath ${CMAKE_CURRENT_SOURCE_DIR}/..)
set(LibsPath ${ProjectPath}/../libs)
set(PDCursesPath ${LibsPath}/PDCurses-3.8)
set(spdlogPath ${LibsPath}/spdlog-1.x)
set(IncludePath ${PDCursesPath} ${spdlogPath}/include)


# ------------------------------------
# External libraries
# ------------------------------------
set(Libraries ${PDCursesPath}/pdcurses.a)


# ------------------------------------
# Source files
# ------------------------------------
set(
    SourceFiles

    main.cpp
    TerminalAPI.cpp
    Snake.cpp
)

set(
    HeaderFiles

    TerminalAPI.hpp
    MenuScreen.hpp
    ControlsScreen.hpp
    Screen.hpp
    GameScreen.hpp
    Snake.hpp
    Timer.hpp
    Log.hpp
)

# ------------------------------------
# Configuration handling
# ------------------------------------
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    string(APPEND CMAKE_CXX_FLAGS "-DSNK_DEBUG")
    list(APPEND Libraries spdlog)
    list(APPEND SourceFiles Log.cpp)
    add_subdirectory(${spdlogPath} ${CMAKE_BINARY_DIR}/spdlog-bin-${CMAKE_BUILD_TYPE})
elseif(CMAKE_BUILD_TYPE STREQUAL "Release")
    string(APPEND CMAKE_CXX_FLAGS "-DSNK_RELEASE")
endif()

set(${RUNTIME_OUTPUT_DIRECTORY} ${CMAKE_SOURCE_DIR}/../bin-${CMAKE_HOST_SYSTEM_NAME}-${CMAKE_BUILD_TYPE})
add_executable(Main ${SourceFiles} ${HeaderFiles})
target_include_directories(Main PUBLIC ${IncludePath})
target_link_libraries(Main ${Libraries})

