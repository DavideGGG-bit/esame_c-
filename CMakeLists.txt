cmake_minimum_required(VERSION 3.10)
project(conto_corrente)

set(CMAKE_CXX_STANDARD 17)

add_subdirectory(lib)
add_subdirectory(test)

# Eseguibile principale
add_executable(main main.cpp)
target_include_directories(main PRIVATE lib)
target_link_libraries(main conto_corrente_lib)
