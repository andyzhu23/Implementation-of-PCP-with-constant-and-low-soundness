# test/CMakeLists.txt
#
# Author: Andy Zhu
# @date    2025-10-16 14:12:59
# @version 1.0.0
#

add_executable(test_get_neighbors test_get_neighbors.cpp ../../src/pcp/BitPCP.cpp ../../src/util.cpp)
add_test(NAME Test_Get_Neighbors COMMAND test_get_neighbors)
target_include_directories(test_get_neighbors PRIVATE ${CMAKE_SOURCE_DIR}/include)

add_executable(test_powering_operation test_powering_operation.cpp ../../src/core/powering_operation.cpp ../../src/pcp/BitPCP.cpp ../../src/pcp/PoweringPCP.cpp ../../src/util.cpp)
add_test(NAME Test_Powering_Operation COMMAND test_powering_operation)
target_include_directories(test_powering_operation PRIVATE ${CMAKE_SOURCE_DIR}/include)

add_executable(test_to_expander test_to_expander.cpp ../../src/core/to_expander.cpp ../../src/pcp/BitPCP.cpp ../../src/util.cpp)
add_test(NAME Test_To_Expander COMMAND test_to_expander)
target_include_directories(test_to_expander PRIVATE ${CMAKE_SOURCE_DIR}/include)

add_executable(test_reduce_degree test_reduce_degree.cpp ../../src/core/reduce_degree.cpp ../../src/pcp/BitPCP.cpp ../../src/util.cpp)
add_test(NAME Test_Reduce_Degree COMMAND test_reduce_degree)
target_include_directories(test_reduce_degree PRIVATE ${CMAKE_SOURCE_DIR}/include)