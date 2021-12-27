cmake_minimum_required(VERSION 3.20)
project(testLab C)

set(CMAKE_C_STANDARD 11)

add_executable(testLab
        00_tmpTest.c
        01_testBits.c
        02_testDefing.c
        03_find.c
        04_malloc.c
        05_getInt.c
        06_polandCalc.c
        07_quickSort.c)
