cmake_minimum_required(VERSION 3.26)
project(DookuLoader C)

set(CMAKE_C_STANDARD 11)

include_directories(/usr/x86_64-w64-mingw32/include)

add_executable(run
        main.c
        Core/Http.c
        Core/Win32.c
        Core/Dooku.c
        Core/Hash.c
        Core/Encrypt.h
        Core/Encrypt.c
)
