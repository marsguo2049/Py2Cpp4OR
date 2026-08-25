cmake_minimum_required(VERSION 3.15)

project(Py2Cpp4OR LANGUAGES CXX)

add_library(py2cpp4or_gvns STATIC
    algorithms/cpp/gvns/src/generic_core.cpp
)
add_library(py2cpp4or::gvns ALIAS py2cpp4or_gvns)

target_include_directories(py2cpp4or_gvns
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/algorithms/cpp/gvns/include>
)
target_compile_features(py2cpp4or_gvns PUBLIC cxx_std_14)
set_target_properties(py2cpp4or_gvns PROPERTIES
    CXX_EXTENSIONS OFF
)

if(MSVC)
    target_compile_options(py2cpp4or_gvns PRIVATE /W4 /permissive-)
else()
    target_compile_options(py2cpp4or_gvns PRIVATE
        -Wall -Wextra -Wpedantic
    )
endif()

include(CTest)
if(BUILD_TESTING)
    add_executable(py2cpp4or_gvns_toy_tests
        tests/cpp/gvns/toy_gvns_tests.cpp
    )
    target_link_libraries(py2cpp4or_gvns_toy_tests
        PRIVATE py2cpp4or::gvns
    )
    target_compile_features(py2cpp4or_gvns_toy_tests PRIVATE cxx_std_14)
    set_target_properties(py2cpp4or_gvns_toy_tests PROPERTIES
        CXX_EXTENSIONS OFF
    )

    if(MSVC)
        target_compile_options(py2cpp4or_gvns_toy_tests PRIVATE
            /W4 /permissive-
        )
    else()
        target_compile_options(py2cpp4or_gvns_toy_tests PRIVATE
            -Wall -Wextra -Wpedantic
        )
    endif()

    add_test(NAME py2cpp4or_gvns_toy_tests
             COMMAND py2cpp4or_gvns_toy_tests)
endif()
