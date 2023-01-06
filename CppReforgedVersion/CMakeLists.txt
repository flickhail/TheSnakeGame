cmake_minimum_required(VERSION 3.24.2)

project(TheSnakeGame LANGUAGES C CXX)

set(
    PDCursesPath
    ${CMAKE_CURRENT_SOURCE_DIR}/../libs/PDCurses-3.8
)

set(
    IncludePath
    ${PDCursesPath}
)

set(
    Libraries
    ${PDCursesPath}/pdcurses.a
)

set(
    SourceFiles
    main.cpp
    ScreenRenderer.cpp
    UserInput.cpp
    Snake.cpp
)

set(
    HeaderFiles
    ScreenRenderer.hpp
    UserInput.hpp
    MenuScreen.hpp
    ControlsScreen.hpp
    Screen.hpp
    GameScreen.hpp
    Snake.hpp
    Timer.hpp
    Logger.hpp
)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_executable(Main ${SourceFiles} ${HeaderFiles})
target_include_directories(Main PUBLIC ${IncludePath})
target_link_libraries(Main ${Libraries})