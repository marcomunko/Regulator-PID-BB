cmake_minimum_required(VERSION 3.26)
project(regulator)

set(CMAKE_CXX_STANDARD 17)

add_executable(regulator main.cpp
        Pomieszczenie.cpp
        Pomieszczenie.h
        Symulacja.cpp
        Symulacja.h
        Grzejnik.cpp
        Grzejnik.h
        Regulator.h
        Utils.h)
