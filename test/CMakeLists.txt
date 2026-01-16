option(BUILD_E2E_TESTS "Build end-to-end tests (may be slow)" ON)

add_executable(
    test_get_neighbors 
    ./unit/test_get_neighbors.cpp 
    ../../src/pcp/BitPCP.cpp
    ../../src/pcp/BitDomain.cpp
    ../../src/util.cpp 

    ../../src/constraint/BitConstraint.cpp
 
)
add_test(NAME Test_Get_Neighbors COMMAND test_get_neighbors)
target_include_directories(test_get_neighbors PRIVATE ${CMAKE_SOURCE_DIR}/include)

add_executable(
    test_PCPAnalyzer 
    ./unit/test_PCPAnalyzer.cpp 
    ../../src/pcp/BitPCP.cpp 
    ../../src/pcp/BitDomain.cpp

    ../../src/constraint/BitConstraint.cpp
    ../../src/util.cpp
    ../../src/analyzer/PCPAnalyzer.cpp
)
add_test(NAME Test_PCPAnalyzer COMMAND test_PCPAnalyzer)
target_include_directories(test_PCPAnalyzer PRIVATE ${CMAKE_SOURCE_DIR}/include)

add_executable(
    test_to_expander 
    ./unit/test_to_expander.cpp 
    ../../src/core/to_expander.cpp 
    ../../src/pcp/BitPCP.cpp 
    ../../src/pcp/BitDomain.cpp
    ../../src/util.cpp

    ../../src/constraint/BitConstraint.cpp
 
)
add_test(NAME Test_To_Expander COMMAND test_to_expander)
target_include_directories(test_to_expander PRIVATE ${CMAKE_SOURCE_DIR}/include)

add_executable(
    test_reduce_degree 
    ./unit/test_reduce_degree.cpp 
    ../../src/core/reduce_degree.cpp 
    ../../src/pcp/BitPCP.cpp 
    ../../src/pcp/BitDomain.cpp
    ../../src/util.cpp

    ../../src/constraint/BitConstraint.cpp
 
)
add_test(NAME Test_Reduce_Degree COMMAND test_reduce_degree)
target_include_directories(test_reduce_degree PRIVATE ${CMAKE_SOURCE_DIR}/include)

add_executable(
    test_Hadamard
    ./unit/test_Hadamard.cpp
    ../../src/pcpp/Hadamard.cpp
)
add_test(NAME Test_Hadamard COMMAND test_Hadamard)
target_include_directories(test_Hadamard PRIVATE ${CMAKE_SOURCE_DIR}/include)

add_executable(
    test_Tester
    ./unit/test_Tester.cpp
    ../../src/pcpp/Tester.cpp
    ../../src/pcpp/Hadamard.cpp
    ../../src/three_color/ThreeColor.cpp
    ../../src/three_csp/ThreeCSP.cpp
    ../../src/pcp/BitPCP.cpp
    ../../src/pcp/BitDomain.cpp
 
    ../../src/util.cpp
)
add_test(NAME Test_Tester COMMAND test_Tester)
target_include_directories(test_Tester PRIVATE ${CMAKE_SOURCE_DIR}/include)

add_executable(
    test_ThreeCSP
    ./unit/test_ThreeCSP.cpp
    ../../src/three_csp/ThreeCSP.cpp
    ../../src/pcp/BitPCP.cpp
    ../../src/pcp/BitDomain.cpp
    ../../src/constraint/BitConstraint.cpp
 
    ../../src/util.cpp
)
add_test(NAME Test_ThreeCSP COMMAND test_ThreeCSP)
target_include_directories(test_ThreeCSP PRIVATE ${CMAKE_SOURCE_DIR}/include)

add_executable(
    test_three_color_generators
    ./unit/test_three_color_generators.cpp
    ../../src/pcp/BitPCP.cpp
    ../../src/pcp/BitDomain.cpp
    ../../src/pcpp/Tester.cpp
    ../../src/pcpp/Hadamard.cpp
    ../../src/three_csp/ThreeCSP.cpp
    ../../src/util.cpp
    ../../src/three_color/generators.cpp
 
    ../../src/three_color/ThreeColor.cpp
)
add_test(NAME Test_Three_Color_Generators COMMAND test_three_color_generators)
target_include_directories(test_three_color_generators PRIVATE ${CMAKE_SOURCE_DIR}/include)


add_executable(
    test_three_color_to_bitpcp
    ./unit/test_three_color_to_bitpcp.cpp
    ../../src/pcpp/Tester.cpp
    ../../src/pcpp/Hadamard.cpp
    ../../src/three_csp/ThreeCSP.cpp
    ../../src/pcp/BitPCP.cpp
    ../../src/pcp/BitDomain.cpp
    ../../src/util.cpp
    ../../src/three_color/ThreeColor.cpp
 
    ../../src/constraint/BitConstraint.cpp
)
add_test(NAME Test_Three_Color_To_BitPCP COMMAND test_three_color_to_bitpcp)
target_include_directories(test_three_color_to_bitpcp PRIVATE ${CMAKE_SOURCE_DIR}/include)
    
add_executable(
    test_random_picker
    ./unit/test_random_picker.cpp
    ../../src/util.cpp
)
add_test(NAME Test_Random_Picker COMMAND test_random_picker)
target_include_directories(test_random_picker PRIVATE ${CMAKE_SOURCE_DIR}/include)

add_executable(
    test_gap_amplification
    ./e2e/test_gap_amplification.cpp
     ../../src/three_csp/ThreeCSP.cpp
    ../../src/core/gap_amplification.cpp

    ../../src/core/reduce_degree.cpp
    ../../src/core/to_expander.cpp
    ../../src/core/three_color_gap_amplification.cpp
    ../../src/three_color/ThreeColor.cpp
    ../../src/three_color/generators.cpp
    ../../src/pcp/BitPCP.cpp
    ../../src/pcp/BitDomain.cpp
    ../../src/pcpp/Hadamard.cpp
    ../../src/pcpp/Tester.cpp
    ../../src/util.cpp
    ../../src/analyzer/PCPAnalyzer.cpp

    ../../src/constraint/BitConstraint.cpp
 
)
add_test(NAME Test_Gap_Amplification COMMAND test_gap_amplification)
target_include_directories(test_gap_amplification PRIVATE ${CMAKE_SOURCE_DIR}/include)

if(BUILD_E2E_TESTS)
    add_executable(
        test_gap_amplification_completeness
        ./e2e/test_gap_amplification_completeness.cpp
        ../../src/three_csp/ThreeCSP.cpp
        ../../src/core/gap_amplification.cpp

        ../../src/core/reduce_degree.cpp
        ../../src/core/to_expander.cpp
        ../../src/core/three_color_gap_amplification.cpp
        ../../src/three_color/ThreeColor.cpp
        ../../src/three_color/generators.cpp
        ../../src/pcp/BitPCP.cpp
        ../../src/pcp/BitDomain.cpp
        ../../src/pcpp/Hadamard.cpp
        ../../src/pcpp/Tester.cpp
        ../../src/util.cpp
        ../../src/analyzer/PCPAnalyzer.cpp

        ../../src/constraint/BitConstraint.cpp
    
    )
    add_test(NAME Test_Gap_Amplification_Completeness COMMAND test_gap_amplification_completeness)
    target_include_directories(test_gap_amplification_completeness PRIVATE ${CMAKE_SOURCE_DIR}/include)
    target_compile_definitions(test_gap_amplification_completeness PRIVATE ENFORCE_CONSISTENCY)
endif()

if(BUILD_E2E_TESTS)
    add_executable(
        test_three_color_gap_amplification_amplification
        ./e2e/test_three_color_gap_amplification_amplification.cpp
        ../../src/core/three_color_gap_amplification.cpp
        ../../src/core/gap_amplification.cpp
    
        ../../src/core/reduce_degree.cpp
        ../../src/core/to_expander.cpp
        ../../src/three_color/ThreeColor.cpp
        ../../src/three_csp/ThreeCSP.cpp
        ../../src/three_color/generators.cpp
        ../../src/pcp/BitPCP.cpp
        ../../src/pcp/BitDomain.cpp
        ../../src/pcpp/Hadamard.cpp
        ../../src/pcpp/Tester.cpp
        ../../src/util.cpp
        ../../src/analyzer/PCPAnalyzer.cpp
    
        ../../src/constraint/BitConstraint.cpp
     
    )
    add_test(NAME Test_Three_Color_Gap_Amplification_Amplification COMMAND test_three_color_gap_amplification_amplification)
    target_include_directories(test_three_color_gap_amplification_amplification PRIVATE ${CMAKE_SOURCE_DIR}/include)
endif()

if(BUILD_E2E_TESTS)
    add_executable(
        test_three_color_gap_amplification_completeness
        ./e2e/test_three_color_gap_amplification_completeness.cpp
        ../../src/core/three_color_gap_amplification.cpp
        ../../src/core/gap_amplification.cpp
    
        ../../src/core/reduce_degree.cpp
        ../../src/core/to_expander.cpp
        ../../src/three_color/ThreeColor.cpp
        ../../src/three_color/generators.cpp
        ../../src/pcp/BitPCP.cpp
        ../../src/pcp/BitDomain.cpp
        ../../src/pcpp/Hadamard.cpp
        ../../src/pcpp/Tester.cpp
        ../../src/three_csp/ThreeCSP.cpp
        ../../src/util.cpp
        ../../src/analyzer/PCPAnalyzer.cpp
    
        ../../src/constraint/BitConstraint.cpp
     
    )
    add_test(NAME Test_Three_Color_Gap_Amplification_Completeness COMMAND test_three_color_gap_amplification_completeness)
    target_include_directories(test_three_color_gap_amplification_completeness PRIVATE ${CMAKE_SOURCE_DIR}/include)
endif()