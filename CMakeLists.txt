cmake_minimum_required(VERSION 2.8)
project(pedsignal)
add_subdirectory(daemonize)
add_subdirectory(pigpio)
find_package (Threads)
set(CMAKE_C_FLAGS -pthread)
set(CMAKE_EXE_LINKER_FLAGS -pthread)
include_directories( . pigpio/src daemonize )
add_executable(pedsignal main.c
    pedsignal.c
    states.c
    statemac.c
    timer.c
    gpio_signals
)
target_link_libraries(pedsignal daemonize pigpio ${CMAKE_THREAD_LIBS_INIT})
