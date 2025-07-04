#colab
find_package(GTest REQUIRED)

add_executable(runAllTests main.cpp test_contocorrente.cpp)
target_include_directories(runAllTests PRIVATE ${GTEST_INCLUDE_DIRS} ../lib)
target_link_libraries(runAllTests ${GTEST_BOTH_LIBRARIES} conto_corrente_lib)

#locale
add_executable(runAllTests main.cpp test_contocorrente.cpp)
target_include_directories(runAllTests PRIVATE /usr/include/gtest ../lib)
target_link_libraries(runAllTests gtest gtest_main conto_corrente_lib pthread)