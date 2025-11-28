
add_executable(
    test_get_neighbors 
    test_get_neighbors.cpp 
    ../../src/pcp/BitPCP.cpp 
    ../../src/pcp/SimplePCP.cpp 
    ../../src/util.cpp 
    ../../src/constraint/BinaryConstraint.cpp
    ../../src/constraint/BitConstraint.cpp
    ../../src/constraint/PoweringConstraint.cpp
)
add_test(NAME Test_Get_Neighbors COMMAND test_get_neighbors)
target_include_directories(test_get_neighbors PRIVATE ${CMAKE_SOURCE_DIR}/include)

add_executable(
    test_PCPAnalyzer 
    test_PCPAnalyzer.cpp 
    ../../src/pcp/BitPCP.cpp 
    ../../src/pcp/SimplePCP.cpp 
    ../../src/constraint/BinaryConstraint.cpp
    ../../src/constraint/BitConstraint.cpp
    ../../src/util.cpp
    ../../src/analyzer/PCPAnalyzer.cpp
)
add_test(NAME Test_PCPAnalyzer COMMAND test_PCPAnalyzer)
target_include_directories(test_PCPAnalyzer PRIVATE ${CMAKE_SOURCE_DIR}/include)

add_executable(
    test_powering_operation 
    test_powering_operation.cpp 
    ../../src/core/powering_operation.cpp 
    ../../src/pcp/BitPCP.cpp 
    ../../src/pcp/SimplePCP.cpp 
    ../../src/pcp/PoweringPCP.cpp 
    ../../src/util.cpp
    ../../src/constraint/BinaryConstraint.cpp
    ../../src/constraint/BitConstraint.cpp
    ../../src/constraint/PoweringConstraint.cpp
)
add_test(NAME Test_Powering_Operation COMMAND test_powering_operation)
target_include_directories(test_powering_operation PRIVATE ${CMAKE_SOURCE_DIR}/include)

add_executable(
    test_to_expander 
    test_to_expander.cpp 
    ../../src/core/to_expander.cpp 
    ../../src/pcp/BitPCP.cpp 
    ../../src/pcp/SimplePCP.cpp 
    ../../src/util.cpp
    ../../src/constraint/BinaryConstraint.cpp
    ../../src/constraint/BitConstraint.cpp
    ../../src/constraint/PoweringConstraint.cpp
)
add_test(NAME Test_To_Expander COMMAND test_to_expander)
target_include_directories(test_to_expander PRIVATE ${CMAKE_SOURCE_DIR}/include)

add_executable(
    test_reduce_degree 
    test_reduce_degree.cpp 
    ../../src/core/reduce_degree.cpp 
    ../../src/pcp/BitPCP.cpp 
    ../../src/pcp/SimplePCP.cpp 
    ../../src/util.cpp
    ../../src/constraint/BinaryConstraint.cpp
    ../../src/constraint/BitConstraint.cpp
    ../../src/constraint/PoweringConstraint.cpp
)
add_test(NAME Test_Reduce_Degree COMMAND test_reduce_degree)
target_include_directories(test_reduce_degree PRIVATE ${CMAKE_SOURCE_DIR}/include)

add_executable(
    test_Hadamard
    test_Hadamard.cpp
    ../../src/pcpp/Hadamard.cpp
)
add_test(NAME Test_Hadamard COMMAND test_Hadamard)
target_include_directories(test_Hadamard PRIVATE ${CMAKE_SOURCE_DIR}/include)

add_executable(
    test_Tester
    test_Tester.cpp
    ../../src/pcpp/Tester.cpp
    ../../src/pcpp/Hadamard.cpp
    ../../src/three_color/three_color.cpp
    ../../src/pcp/BitPCP.cpp
    ../../src/constraint/PoweringConstraint.cpp
    ../../src/util.cpp
)
add_test(NAME Test_Tester COMMAND test_Tester)
target_include_directories(test_Tester PRIVATE ${CMAKE_SOURCE_DIR}/include)


add_executable(
    test_reduce_alphabet
    test_reduce_alphabet.cpp
    ../../src/core/reduce_alphabet.cpp
    ../../src/pcpp/Tester.cpp
    ../../src/pcpp/Hadamard.cpp
    ../../src/pcp/BitPCP.cpp
    ../../src/pcp/PoweringPCP.cpp
    ../../src/three_color/three_color.cpp
    ../../src/util.cpp
    ../../src/constraint/PoweringConstraint.cpp
    ../../src/constraint/BitConstraint.cpp
)
add_test(NAME Test_Reduce_Alphabet COMMAND test_reduce_alphabet)
target_include_directories(test_reduce_alphabet PRIVATE ${CMAKE_SOURCE_DIR}/include)


add_executable(
    test_three_color_generators
    test_three_color_generators.cpp
    ../../src/pcp/BitPCP.cpp
    ../../src/pcp/PoweringPCP.cpp
    ../../src/pcpp/Tester.cpp
    ../../src/pcpp/Hadamard.cpp
    ../../src/util.cpp
    ../../src/three_color/generators.cpp
    ../../src/constraint/PoweringConstraint.cpp
    ../../src/three_color/three_color.cpp
)
add_test(NAME Test_Three_Color_Generators COMMAND test_three_color_generators)
target_include_directories(test_three_color_generators PRIVATE ${CMAKE_SOURCE_DIR}/include)