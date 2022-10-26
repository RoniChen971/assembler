cmake_minimum_required(VERSION 3.17)
project(assembler C)

set(CMAKE_C_STANDARD 11)

include_directories(.)

add_executable(assembler
        assembler.c
        Constants.h
        ext.c
        ext.h
        FirstTrans.c
        SecondTrans.c
        str.c
        symTable.c
        symTable.h
        Table.c
        Table.h
        TxtFunc.c
        TxtFunc.h)
