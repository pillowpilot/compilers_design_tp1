cmake_minimum_required(VERSION 3.9)

project(compilers1 CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_BUILD_TYPE RelWithDebugInfo)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wno-long-long -pedantic")
set(CMAKE_CXX_FLAGS_DEBUG "-g")
set(CMAKE_CXX_FLAGS_RELEASE "-O2")

find_package(GTest REQUIRED)

include_directories(include) # Add include/ to all targets include directories

# Libraries
add_library(ShuntingYard src/SimpleAlgorithm)

add_library(TransitionTable src/TransitionTable)

add_library(NFA src/NFA)
target_link_libraries(NFA TransitionTable)

add_library(DFA src/DFA)
target_link_libraries(DFA NFA TransitionTable)

add_library(Powerset src/Powerset)
target_link_libraries(Powerset NFA DFA)

add_library(Thompson src/Thompson)
target_link_libraries(Thompson NFA)

add_library(Hopcroft src/Hopcroft)
target_link_libraries(Hopcroft DFA)

# Executables
add_executable(Main src/CompilersTP1)
target_link_libraries(Main ShuntingYard NFA DFA Powerset Thompson Hopcroft ${GTEST_LIBRARIES})

# Tests
enable_testing()

add_executable(ShuntingYardTests tests/SimpleAlgorithm_test)
target_link_libraries(ShuntingYardTests ShuntingYard ${GTEST_LIBRARIES})
add_test(ShuntingYardTests ShuntingYardTests)

add_executable(TransitionTableTests tests/TransitionTable_test)
target_link_libraries(TransitionTableTests TransitionTable ${GTEST_LIBRARIES})
add_test(TransitionTableTests TransitionTableTests)

add_executable(NFATests tests/NFA_test)
target_link_libraries(NFATests NFA ${GTEST_LIBRARIES})
add_test(NFATests NFATests)

add_executable(DFATests tests/DFA_test)
target_link_libraries(DFATests DFA ${GTEST_LIBRARIES})
add_test(DFATests DFATests)

add_executable(ThompsonTests tests/Thompson_test)
target_link_libraries(ThompsonTests Thompson ${GTEST_LIBRARIES})
add_test(ThompsonTests ThompsonTests)

add_executable(PowersetTests tests/Powerset_test)
target_link_libraries(PowersetTests Powerset ${GTEST_LIBRARIES})
add_test(PowersetTests PowersetTests)

add_executable(HopcroftTests tests/Hopcroft_test)
target_link_libraries(HopcroftTests Hopcroft ${GTEST_LIBRARIES})
add_test(HopcroftTests HopcroftTests)
