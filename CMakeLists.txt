cmake_minimum_required(VERSION 3.29)
project(KurvaKrestikiNoliki C)
set(CMAKE_C_STANDARD 17)
find_package(Curses REQUIRED)
include_directories(${CURSES_INCLUDE_DIR})

add_executable(KurvaKrestikiNoliki main.c)
#add_executable(KurvaKrestikiNoliki test.c)

target_link_libraries(KurvaKrestikiNoliki ${CURSES_LIBRARIES})