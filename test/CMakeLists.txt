option(BUILD_E2E_TESTS "Build end-to-end tests (may be slow)" ON)

add_executable(
    test_get_neighbors 
    ./unit/test_get_neighbors.cpp 
    ../../src/pcp/BitPCP.cpp
    ../../src/pcp/BitDomain.cpp

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
    ../../src/analyzer/PCPAnalyzer.cpp
)
add_test(NAME Test_PCPAnalyzer COMMAND test_PCPAnalyzer)
target_include_directories(test_PCPAnalyzer PRIVATE ${CMAKE_SOURCE_DIR}/include)

add_executable(
    test_SoundnessApproximater
    ./unit/test_SoundnessApproximater.cpp
    ../../src/analyzer/SoundnessApproximater.cpp
    ../../src/pcp/BitPCP.cpp
    ../../src/pcp/BitDomain.cpp
    ../../src/constraint/BitConstraint.cpp
)
add_test(NAME Test_SoundnessApproximater COMMAND test_SoundnessApproximater)
target_include_directories(test_SoundnessApproximater PRIVATE ${CMAKE_SOURCE_DIR}/include)

add_executable(
    test_PseudoTester
    ./unit/test_PseudoTester.cpp
    ../../src/pcpp/PseudoPCPP/PseudoTester.cpp
    ../../src/pcpp/PseudoPCPP/CSPSolver.cpp
    ../../src/analyzer/SoundnessApproximater.cpp
    ../../src/pcp/BitPCP.cpp
    ../../src/pcp/BitDomain.cpp
    ../../src/constraint/BitConstraint.cpp
    ../../src/util/disjoint_set_union.cpp
    ../../src/util/visit_guard.cpp
)
add_test(NAME Test_PseudoTester COMMAND test_PseudoTester)
target_include_directories(test_PseudoTester PRIVATE ${CMAKE_SOURCE_DIR}/include)

add_executable(
    test_CSPSolver
    ./unit/test_CSPSolver.cpp
    ../../src/pcpp/PseudoPCPP/CSPSolver.cpp
    ../../src/pcp/BitPCP.cpp
    ../../src/pcp/BitDomain.cpp
    ../../src/constraint/BitConstraint.cpp
    ../../src/util/disjoint_set_union.cpp
)
add_test(NAME Test_CSPSolver COMMAND test_CSPSolver)
target_include_directories(test_CSPSolver PRIVATE ${CMAKE_SOURCE_DIR}/include)

add_executable(
    test_UnivariatePolynomial
    ./unit/test_UnivariatePolynomial.cpp
    ../../src/finite_field/UnivariatePolynomial.cpp
    ../../src/finite_field/FiniteFieldElement.cpp
)
add_test(NAME Test_UnivariatePolynomial COMMAND test_UnivariatePolynomial)
target_include_directories(test_UnivariatePolynomial PRIVATE ${CMAKE_SOURCE_DIR}/include)

add_executable(
    test_Polynomial
    ./unit/test_Polynomial.cpp
    ../../src/finite_field/Polynomial.cpp
    ../../src/finite_field/Monomial.cpp
    ../../src/finite_field/FiniteFieldElement.cpp
)
add_test(NAME Test_Polynomial COMMAND test_Polynomial)
target_include_directories(test_Polynomial PRIVATE ${CMAKE_SOURCE_DIR}/include)

add_executable(
    test_to_expander 
    ./unit/test_to_expander.cpp 
    ../../src/core/to_expander.cpp 
    ../../src/pcp/BitPCP.cpp 
    ../../src/pcp/BitDomain.cpp
    ../../src/three_color/ThreeColor.cpp
    ../../src/three_csp/ThreeCSP.cpp
    ../../src/pcpp/HadamardPCPP/HadamardTester.cpp
    ../../src/pcpp/HadamardPCPP/Hadamard.cpp
    ../../src/pcpp/TesterFactory.cpp
    ../../src/pcpp/ReedMullerPCPP/ReedMullerPCPP.cpp
    ../../src/pcpp/PseudoPCPP/PseudoTester.cpp
    ../../src/pcpp/PseudoPCPP/CSPSolver.cpp
    ../../src/analyzer/SoundnessApproximater.cpp
    ../../src/util/disjoint_set_union.cpp
    ../../src/util/visit_guard.cpp
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
    ../../src/util/disjoint_set_union.cpp
    ../../src/util/visit_guard.cpp

    ../../src/constraint/BitConstraint.cpp
 
)
add_test(NAME Test_Reduce_Degree COMMAND test_reduce_degree)
target_include_directories(test_reduce_degree PRIVATE ${CMAKE_SOURCE_DIR}/include)
target_compile_definitions(test_reduce_degree PRIVATE ENFORCE_CONSISTENCY)

add_executable(
    test_Hadamard
    ./unit/test_Hadamard.cpp
    ../../src/pcpp/HadamardPCPP/Hadamard.cpp
)
add_test(NAME Test_Hadamard COMMAND test_Hadamard)
target_include_directories(test_Hadamard PRIVATE ${CMAKE_SOURCE_DIR}/include)

add_executable(
    test_HadamardTester
    ./unit/test_HadamardTester.cpp
    ../../src/pcpp/HadamardPCPP/HadamardTester.cpp
    ../../src/pcpp/HadamardPCPP/Hadamard.cpp
    ../../src/three_color/ThreeColor.cpp
    ../../src/three_csp/ThreeCSP.cpp
    ../../src/pcpp/TesterFactory.cpp
    ../../src/pcpp/ReedMullerPCPP/ReedMullerPCPP.cpp
    ../../src/pcpp/PseudoPCPP/PseudoTester.cpp
    ../../src/pcpp/PseudoPCPP/CSPSolver.cpp
    ../../src/analyzer/SoundnessApproximater.cpp
    ../../src/pcp/BitPCP.cpp
    ../../src/pcp/BitDomain.cpp
    ../../src/util/disjoint_set_union.cpp
    ../../src/util/visit_guard.cpp
    ../../src/constraint/BitConstraint.cpp
)
add_test(NAME Test_HadamardTester COMMAND test_HadamardTester)
target_include_directories(test_HadamardTester PRIVATE ${CMAKE_SOURCE_DIR}/include)

add_executable(
    test_ThreeCSP
    ./unit/test_ThreeCSP.cpp
    ../../src/three_csp/ThreeCSP.cpp
    ../../src/pcp/BitPCP.cpp
    ../../src/pcp/BitDomain.cpp
    ../../src/constraint/BitConstraint.cpp
    ../../src/util/disjoint_set_union.cpp
    ../../src/util/visit_guard.cpp
)
add_test(NAME Test_ThreeCSP COMMAND test_ThreeCSP)
target_include_directories(test_ThreeCSP PRIVATE ${CMAKE_SOURCE_DIR}/include)

add_executable(
    test_three_color_generators
    ./unit/test_three_color_generators.cpp
    ../../src/pcp/BitPCP.cpp
    ../../src/pcp/BitDomain.cpp
    ../../src/pcpp/HadamardPCPP/HadamardTester.cpp
    ../../src/pcpp/HadamardPCPP/Hadamard.cpp
    ../../src/three_csp/ThreeCSP.cpp
    ../../src/pcpp/TesterFactory.cpp
    ../../src/pcpp/ReedMullerPCPP/ReedMullerPCPP.cpp
    ../../src/pcpp/PseudoPCPP/PseudoTester.cpp
    ../../src/pcpp/PseudoPCPP/CSPSolver.cpp
    ../../src/analyzer/SoundnessApproximater.cpp
    ../../src/util/disjoint_set_union.cpp
    ../../src/util/visit_guard.cpp
    ../../src/three_color/generators.cpp
 
    ../../src/three_color/ThreeColor.cpp
    ../../src/constraint/BitConstraint.cpp
)
add_test(NAME Test_Three_Color_Generators COMMAND test_three_color_generators)
target_include_directories(test_three_color_generators PRIVATE ${CMAKE_SOURCE_DIR}/include)


add_executable(
    test_three_color_to_bitpcp
    ./unit/test_three_color_to_bitpcp.cpp
    ../../src/pcpp/HadamardPCPP/HadamardTester.cpp
    ../../src/pcpp/HadamardPCPP/Hadamard.cpp
    ../../src/three_csp/ThreeCSP.cpp
    ../../src/pcpp/TesterFactory.cpp
    ../../src/pcpp/ReedMullerPCPP/ReedMullerPCPP.cpp
    ../../src/pcpp/PseudoPCPP/PseudoTester.cpp
    ../../src/pcpp/PseudoPCPP/CSPSolver.cpp
    ../../src/analyzer/SoundnessApproximater.cpp
    ../../src/pcp/BitPCP.cpp
    ../../src/pcp/BitDomain.cpp
    ../../src/util/disjoint_set_union.cpp
    ../../src/util/visit_guard.cpp
    ../../src/three_color/ThreeColor.cpp
 
    ../../src/constraint/BitConstraint.cpp
)
add_test(NAME Test_Three_Color_To_BitPCP COMMAND test_three_color_to_bitpcp)
target_include_directories(test_three_color_to_bitpcp PRIVATE ${CMAKE_SOURCE_DIR}/include)
    
add_executable(
    test_random_picker
    ./unit/test_random_picker.cpp
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
    ../../src/pcpp/TesterFactory.cpp
    ../../src/pcpp/ReedMullerPCPP/ReedMullerPCPP.cpp
    ../../src/pcpp/PseudoPCPP/PseudoTester.cpp
    ../../src/pcpp/PseudoPCPP/CSPSolver.cpp
    ../../src/analyzer/SoundnessApproximater.cpp
    ../../src/pcp/BitPCP.cpp
    ../../src/pcp/BitDomain.cpp
    ../../src/pcpp/HadamardPCPP/Hadamard.cpp
    ../../src/pcpp/HadamardPCPP/HadamardTester.cpp
    ../../src/util/disjoint_set_union.cpp
    ../../src/util/visit_guard.cpp
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
        ../../src/pcpp/TesterFactory.cpp
        ../../src/pcpp/ReedMullerPCPP/ReedMullerPCPP.cpp
        ../../src/pcpp/PseudoPCPP/PseudoTester.cpp
        ../../src/pcpp/PseudoPCPP/CSPSolver.cpp
        ../../src/analyzer/SoundnessApproximater.cpp
        ../../src/pcp/BitPCP.cpp
        ../../src/pcp/BitDomain.cpp
        ../../src/pcpp/HadamardPCPP/Hadamard.cpp
        ../../src/pcpp/HadamardPCPP/HadamardTester.cpp
        ../../src/util/disjoint_set_union.cpp
        ../../src/util/visit_guard.cpp
        ../../src/analyzer/PCPAnalyzer.cpp

        ../../src/constraint/BitConstraint.cpp
    
    )
    add_test(NAME Test_Gap_Amplification_Completeness COMMAND test_gap_amplification_completeness)
    target_include_directories(test_gap_amplification_completeness PRIVATE ${CMAKE_SOURCE_DIR}/include)
    target_compile_definitions(test_gap_amplification_completeness PRIVATE ENFORCE_CONSISTENCY)
endif()

if(BUILD_E2E_TESTS)
    add_executable(
        test_three_color_gap_amplification_hadamard
        ./e2e/test_three_color_gap_amplification_hadamard.cpp
        ../../src/core/three_color_gap_amplification.cpp
        ../../src/core/gap_amplification.cpp
    
        ../../src/core/reduce_degree.cpp
        ../../src/core/to_expander.cpp
        ../../src/three_color/ThreeColor.cpp
        ../../src/three_csp/ThreeCSP.cpp
        ../../src/three_color/generators.cpp
        ../../src/pcpp/TesterFactory.cpp
        ../../src/pcpp/ReedMullerPCPP/ReedMullerPCPP.cpp
        ../../src/pcpp/PseudoPCPP/PseudoTester.cpp
        ../../src/pcpp/PseudoPCPP/CSPSolver.cpp
        ../../src/analyzer/SoundnessApproximater.cpp
        ../../src/pcp/BitPCP.cpp
        ../../src/pcp/BitDomain.cpp
        ../../src/pcpp/HadamardPCPP/Hadamard.cpp
        ../../src/pcpp/HadamardPCPP/HadamardTester.cpp
        ../../src/util/disjoint_set_union.cpp
        ../../src/analyzer/PCPAnalyzer.cpp
    
        ../../src/constraint/BitConstraint.cpp
     
    )
    add_test(NAME Test_Three_Color_Gap_Amplification_Hadamard COMMAND test_three_color_gap_amplification_hadamard   )
    target_include_directories(test_three_color_gap_amplification_hadamard PRIVATE ${CMAKE_SOURCE_DIR}/include)
    # target_compile_definitions(test_three_color_gap_amplification_hadamard PRIVATE ENFORCE_CONSISTENCY)
endif()

if(BUILD_E2E_TESTS)
    add_executable(
        test_three_color_gap_amplification_pseudo
        ./e2e/test_three_color_gap_amplification_pseudo.cpp
        ../../src/core/three_color_gap_amplification.cpp
        ../../src/core/gap_amplification.cpp
    
        ../../src/core/reduce_degree.cpp
        ../../src/core/to_expander.cpp
        ../../src/three_color/ThreeColor.cpp
        ../../src/three_csp/ThreeCSP.cpp
        ../../src/three_color/generators.cpp
        ../../src/pcpp/TesterFactory.cpp
        ../../src/pcpp/ReedMullerPCPP/ReedMullerPCPP.cpp
        ../../src/pcpp/PseudoPCPP/PseudoTester.cpp
        ../../src/pcpp/PseudoPCPP/CSPSolver.cpp
        ../../src/pcpp/HadamardPCPP/Hadamard.cpp
        ../../src/pcpp/HadamardPCPP/HadamardTester.cpp
        ../../src/analyzer/SoundnessApproximater.cpp
        ../../src/pcp/BitPCP.cpp
        ../../src/pcp/BitDomain.cpp
        ../../src/util/disjoint_set_union.cpp
        ../../src/analyzer/PCPAnalyzer.cpp
    
        ../../src/constraint/BitConstraint.cpp
     
    )
    add_test(NAME Test_Three_Color_Gap_Amplification_Pseudo COMMAND test_three_color_gap_amplification_pseudo   )
    target_include_directories(test_three_color_gap_amplification_pseudo PRIVATE ${CMAKE_SOURCE_DIR}/include)
    # target_compile_definitions(test_three_color_gap_amplification_pseudo PRIVATE ENFORCE_CONSISTENCY)
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
        ../../src/pcpp/TesterFactory.cpp
        ../../src/pcpp/ReedMullerPCPP/ReedMullerPCPP.cpp
        ../../src/pcpp/PseudoPCPP/PseudoTester.cpp
        ../../src/pcpp/PseudoPCPP/CSPSolver.cpp
        ../../src/pcp/BitPCP.cpp
        ../../src/pcp/BitDomain.cpp
        ../../src/pcpp/HadamardPCPP/Hadamard.cpp
        ../../src/pcpp/HadamardPCPP/HadamardTester.cpp
        ../../src/three_csp/ThreeCSP.cpp
        ../../src/util/disjoint_set_union.cpp
        ../../src/analyzer/SoundnessApproximater.cpp
        ../../src/analyzer/PCPAnalyzer.cpp
    
        ../../src/constraint/BitConstraint.cpp
     
    )
    add_test(NAME Test_Three_Color_Gap_Amplification_Completeness COMMAND test_three_color_gap_amplification_completeness)
    target_include_directories(test_three_color_gap_amplification_completeness PRIVATE ${CMAKE_SOURCE_DIR}/include)
endif()

if(BUILD_E2E_TESTS)
    add_executable(
        test_HadamardPCPP_soundness
        ./e2e/test_HadamardPCPP_soundness.cpp
        ../../src/three_color/ThreeColor.cpp
        ../../src/three_color/generators.cpp
        ../../src/three_csp/ThreeCSP.cpp
        ../../src/pcpp/TesterFactory.cpp
        ../../src/pcpp/ReedMullerPCPP/ReedMullerPCPP.cpp
        ../../src/pcpp/PseudoPCPP/PseudoTester.cpp
        ../../src/pcpp/PseudoPCPP/CSPSolver.cpp
        ../../src/pcp/BitPCP.cpp
        ../../src/pcp/BitDomain.cpp
        ../../src/core/gap_amplification.cpp
        ../../src/core/reduce_degree.cpp
        ../../src/core/to_expander.cpp
        ../../src/core/three_color_gap_amplification.cpp
        ../../src/pcpp/HadamardPCPP/Hadamard.cpp
        ../../src/pcpp/HadamardPCPP/HadamardTester.cpp
        ../../src/util/disjoint_set_union.cpp
        ../../src/util/visit_guard.cpp
        ../../src/analyzer/SoundnessApproximater.cpp
        ../../src/analyzer/PCPAnalyzer.cpp
        ../../src/constraint/BitConstraint.cpp
    )
    add_test(NAME Test_HadamardPCPP_Soundness COMMAND test_HadamardPCPP_soundness)
    target_include_directories(test_HadamardPCPP_soundness PRIVATE ${CMAKE_SOURCE_DIR}/include)
    target_compile_definitions(test_HadamardPCPP_soundness PRIVATE ENFORCE_CONSISTENCY)
endif()