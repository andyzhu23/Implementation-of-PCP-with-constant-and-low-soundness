# test/CMakeLists.txt

# Add test executable for get_radius_nodes
add_executable(test_get_radius_nodes test_get_radius_nodes.cpp)
add_test(NAME Test_Get_Radius_Nodes COMMAND test_get_radius_nodes)
target_include_directories(test_get_radius_nodes PRIVATE ${CMAKE_SOURCE_DIR}/include)

# Add test executable for power_operation
add_executable(test_power_operation test_power_operation.cpp)
add_test(NAME Test_Power_Operation COMMAND test_power_operation)
target_include_directories(test_power_operation PRIVATE ${CMAKE_SOURCE_DIR}/include)

add_executable(test_three_col test_three_col.cpp ../../src/three_col.cpp)
add_test(NAME Test_Three_Col COMMAND test_three_col)
target_include_directories(test_three_col PRIVATE ${CMAKE_SOURCE_DIR}/include)

