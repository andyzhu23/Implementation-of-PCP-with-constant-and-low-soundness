# test/CMakeLists.txt

add_executable(test_get_neighbors test_get_neighbors.cpp ../../src/BitPCP.cpp)
add_test(NAME Test_Get_Neighbors COMMAND test_get_neighbors)
target_include_directories(test_get_neighbors PRIVATE ${CMAKE_SOURCE_DIR}/include)

add_executable(test_powering_operation test_powering_operation.cpp ../../src/core.cpp ../../src/BitPCP.cpp ../../src/PoweringPCP.cpp)
add_test(NAME Test_Powering_Operation COMMAND test_powering_operation)
target_include_directories(test_powering_operation PRIVATE ${CMAKE_SOURCE_DIR}/include)
