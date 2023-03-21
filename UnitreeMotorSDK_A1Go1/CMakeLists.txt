cmake_minimum_required(VERSION 2.8.3)
project(UnitreeMotorSDK)

set(CMAKE_CXX_FLAGS "$ENV{CXXFLAGS} -O3 -march=native -Wall")

include_directories(
    include
)

link_directories(
    lib
)

add_executable(test_motor src/test_motor.cpp)
target_link_libraries(test_motor libunitreeMotorSDK_Linux64.so)

add_executable(changeID src/changeID.cpp)
target_link_libraries(changeID libunitreeMotorSDK_Linux64.so)
