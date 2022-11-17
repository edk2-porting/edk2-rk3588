project(cmocka-examples C)

set_source_files_properties(calculator.c
                            allocate_module.c
                            assert_module.c
                            PROPERTIES
                                COMPILE_DEFINITIONS
                                    UNIT_TESTING=1)


### The most simple test
add_cmocka_test(simple_test
                SOURCES simple_test.c
                COMPILE_OPTIONS ${DEFAULT_C_COMPILE_FLAGS}
                LINK_LIBRARIES cmocka::cmocka)
add_cmocka_test_environment(simple_test)

### Allocate module test
add_cmocka_test(allocate_module_test
                SOURCES allocate_module.c allocate_module_test.c
                LINK_LIBRARIES cmocka::cmocka)
add_cmocka_test_environment(allocate_module_test)

set_tests_properties(allocate_module_test
                     PROPERTIES
                        WILL_FAIL 1)

### Assert macro test
add_cmocka_test(assert_macro_test
                SOURCES assert_macro.c assert_macro_test.c
                LINK_LIBRARIES cmocka::cmocka)
add_cmocka_test_environment(assert_macro_test)

set_tests_properties(assert_macro_test
                     PROPERTIES
                        WILL_FAIL 1)

### Assert module test
add_cmocka_test(assert_module_test
                SOURCES assert_module.c assert_module_test.c
                LINK_LIBRARIES cmocka::cmocka)
add_cmocka_test_environment(assert_module_test)

set_tests_properties(assert_module_test
                     PROPERTIES
                        WILL_FAIL 1)

if (NOT WIN32)
    add_subdirectory(mock)
endif()
