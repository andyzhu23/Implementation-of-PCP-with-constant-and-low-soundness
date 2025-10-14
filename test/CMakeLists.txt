# test/CMakeLists.txt

# Add test executable
add_executable(get_radius_nodes_test get_radius_nodes_test.cpp)

# Add the test definition
add_test(NAME Get_Radius_Nodes_Test COMMAND get_radius_nodes_test)

# Link to the main project's libraries or headers
target_include_directories(get_radius_nodes_test PRIVATE ${CMAKE_SOURCE_DIR}/include)

# Add test executable
add_executable(power_operation_test power_operation_test.cpp)

# Add the test definition
add_test(NAME Power_Operation_Test COMMAND power_operation_test)

# Link to the main project's libraries or headers
target_include_directories(power_operation_test PRIVATE ${CMAKE_SOURCE_DIR}/include)

