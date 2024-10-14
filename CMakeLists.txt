cmake_minimum_required(VERSION 3.28)
project(NIST)

set(CMAKE_CXX_STANDARD 17)
SET(CMAKE_EXE_LINKER_FLAGS "--std=c++14 -latomic -fsanitize=address,undefined ")
add_executable(NIST main.cpp
        src/frequency.c
        src/cusum.c
        src/blockFrequency.c
        src/runs.c
        src/rank.c
        src/longestRunOfOnes.c
        src/discreteFourierTransform.c
        src/nonOverlappingTemplateMatchings.c
        src/overlappingTemplateMatchings.c
        src/universal.c
        src/approximateEntropy.c
        src/randomExcursions.c
        src/randomExcursionsVariant.c
        src/linearComplexity.c
        src/serial.c
        src/cephes.c
        src/matrix.c
        src/dfft.c
)
