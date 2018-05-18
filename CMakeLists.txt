cmake_minimum_required(VERSION 2.8)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
project(cta_clock)

add_executable(cta_clock model.cpp cta_model.cpp cta_clock.cpp)

add_library(matrix SHARED IMPORTED)
set_target_properties(matrix PROPERTIES IMPORTED_LOCATION ${PROJECT_SOURCE_DIR}/matrix/lib/librgbmatrix.so.1)

target_link_libraries(cta_clock -lpthread -lrt -lm -lcurlpp -lcurl matrix)