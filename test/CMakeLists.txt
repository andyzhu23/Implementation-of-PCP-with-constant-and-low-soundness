cmake_minimum_required(VERSION 3.12)

# 1. Define the option
option(BUILD_E2E_TESTS "Build end-to-end tests (may be slow)" ON)

# 2. Group all source files into a single object or static library
add_library(pcp_core STATIC
    ../../src/pcp/BinaryCSP.cpp
    ../../src/pcp/BinaryDomain.cpp
    ../../src/constraint/BinaryConstraint.cpp
    ../../src/analyzer/PCPAnalyzer.cpp
    ../../src/analyzer/SoundnessApproximater.cpp
    ../../src/pcpp/PseudoPCPP/PseudoTester.cpp
    ../../src/pcpp/PseudoPCPP/CSPSolver.cpp
    ../../src/util/disjoint_set_union.cpp
    ../../src/util/visit_guard.cpp
    ../../src/finite_field/UnivariatePolynomial.cpp
    ../../src/finite_field/FiniteFieldElement.cpp
    ../../src/finite_field/Polynomial.cpp
    ../../src/finite_field/Monomial.cpp
    ../../src/finite_field/NumberTheoreticTransform.cpp
    ../../src/core/to_expander.cpp
    ../../src/core/reduce_degree.cpp
    ../../src/three_color/ThreeColor.cpp
    ../../src/three_color/generators.cpp
    ../../src/three_csp/ThreeCSP.cpp
    ../../src/pcpp/HadamardPCPP/HadamardTester.cpp
    ../../src/pcpp/HadamardPCPP/Hadamard.cpp
    ../../src/pcpp/TesterFactory.cpp
    ../../src/pcpp/ReedMullerPCPP/ReedMullerPCPP.cpp
)

# 3. Propagate include paths automatically via the library
target_include_directories(pcp_core PUBLIC ${CMAKE_SOURCE_DIR}/include)

# 4. Use a clean, reusable macro to generate tests instantly
macro(add_pcp_test test_name source_file)
    add_executable(${test_name} ${source_file})
    target_link_libraries(${test_name} PRIVATE pcp_core)
    add_test(NAME ${test_name} COMMAND ${test_name})
endmacro()

# 5. Define your unit tests cleanly (One-liners)
add_pcp_test(test_get_neighbors ./unit/test_get_neighbors.cpp)
add_pcp_test(test_PCPAnalyzer ./unit/test_PCPAnalyzer.cpp)
add_pcp_test(test_SoundnessApproximater ./unit/test_SoundnessApproximater.cpp)
add_pcp_test(test_PseudoTester ./unit/test_PseudoTester.cpp)
add_pcp_test(test_CSPSolver ./unit/test_CSPSolver.cpp)
add_pcp_test(test_UnivariatePolynomial ./unit/test_UnivariatePolynomial.cpp)
add_pcp_test(test_Polynomial ./unit/test_Polynomial.cpp)
add_pcp_test(test_NumberTheoreticTransform ./unit/test_NumberTheoreticTransform.cpp)
add_pcp_test(test_Hadamard ./unit/test_Hadamard.cpp)
add_pcp_test(test_HadamardTester ./unit/test_HadamardTester.cpp)
add_pcp_test(test_ThreeCSP ./unit/test_ThreeCSP.cpp)
add_pcp_test(test_three_color_generators ./unit/test_three_color_generators.cpp)
add_pcp_test(test_three_color_to_BinaryCSP ./unit/test_three_color_to_BinaryCSP.cpp)
add_pcp_test(test_random_picker ./unit/test_random_picker.cpp)

# 6. Handle edge cases needing custom definitions
add_pcp_test(test_reduce_degree ./unit/test_reduce_degree.cpp)
target_compile_definitions(test_reduce_degree PRIVATE ENFORCE_CONSISTENCY)

# 7. Use the condition variable for slow, end-to-end tests
if(BUILD_E2E_TESTS)
    add_pcp_test(test_to_expander ./unit/test_to_expander.cpp)
endif()
