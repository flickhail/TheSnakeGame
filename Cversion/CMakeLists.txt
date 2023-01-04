cmake_minimum_required(VERSION 3.24.2)

project(TheSnakeGame LANGUAGES C CXX)


set(
    PDCursesPath
    ${CMAKE_CURRENT_SOURCE_DIR}/../libs/PDCurses-3.8
)

set(
    Libraries
    ${PDCursesPath}/pdcurses.a
)

set(
    IncludePath
    ${PDCursesPath}
)

set(
    SourceFiles
    main.c
)

set(
    HeaderFiles
    myGame.h
)

add_executable(Main ${SourceFiles} ${HeaderFiles})
target_include_directories(Main PUBLIC ${IncludePath})
target_link_libraries(Main ${Libraries})