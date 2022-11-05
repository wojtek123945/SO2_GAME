cmake_minimum_required(VERSION 3.20)
project(GAME_SERVER C)

set(CMAKE_C_STANDARD 11)
set(THREADS_PREFER_PTHREAD_FLAG ON)

add_executable(GAME_SERVER Server.c lib/serverFunc.h lib/serverFunc.c lib/boardFunc.h lib/boardFunc.c lib/player-beast_structures.h lib/playerFunc.h lib/playerFunc.c)

find_package(Curses REQUIRED)
target_link_libraries(GAME_SERVER ${CURSES_LIBRARIES})

find_package(Threads REQUIRED)
target_link_libraries( GAME_SERVER ${CMAKE_THREAD_LIBS_INIT} )
