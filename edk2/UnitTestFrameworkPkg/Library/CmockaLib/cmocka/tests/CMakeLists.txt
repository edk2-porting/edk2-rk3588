project(tests C)

set(TEST_EXCEPTION_HANDLER TRUE)
if (CMAKE_BUILD_TYPE)
    string(TOLOWER "${CMAKE_BUILD_TYPE}" CMAKE_BUILD_TYPE_LOWER)
    if (CMAKE_BUILD_TYPE_LOWER STREQUAL "undefinedsanitizer")
        set(TEST_EXCEPTION_HANDLER FALSE)
    endif()
endif()

set(CMOCKA_TESTS
    test_alloc
    test_group_setup_assert
    test_group_setup_fail
    test_fixtures
    test_group_fixtures
    test_groups
    test_float_macros
    test_double_macros
    test_assert_macros
    test_assert_macros_fail
    test_basics
    test_skip
    test_strmatch
    test_setup_fail
    test_ordering
    test_ordering_fail
    test_returns
    test_returns_fail
    test_wildcard
    test_skip_filter
    test_cmockery
    )

if (TEST_EXCEPTION_HANDLER)
    list(APPEND CMOCKA_TESTS test_exception_handler)
endif()

foreach(_CMOCKA_TEST ${CMOCKA_TESTS})
    add_cmocka_test(${_CMOCKA_TEST}
                    SOURCES ${_CMOCKA_TEST}.c
                    COMPILE_OPTIONS ${DEFAULT_C_COMPILE_FLAGS}
                    LINK_LIBRARIES cmocka::static
                    LINK_OPTIONS ${DEFAULT_LINK_FLAGS})
    target_include_directories(${_CMOCKA_TEST} PRIVATE ${cmocka_BINARY_DIR})

    add_cmocka_test_environment(${_CMOCKA_TEST})
endforeach()

### Special Cases
if (${CMAKE_C_COMPILER_ID} MATCHES "(GNU|Clang)")
    set_source_files_properties(test_cmockery.c PROPERTIES COMPILE_FLAGS "-Wno-deprecated-declarations")
endif()

### Exceptions

# test_skip
set_tests_properties(
    test_skip
        PROPERTIES
        PASS_REGULAR_EXPRESSION
        "\\[  SKIPPED \\] test_check_skip"
)

# test_assert_macros_fail
set_tests_properties(
    test_assert_macros_fail
        PROPERTIES
        PASS_REGULAR_EXPRESSION
        "\\[  FAILED  \\] 1 test"
)

# test_ordering ensure proper failures
set_tests_properties(
    test_ordering_fail
        PROPERTIES
        PASS_REGULAR_EXPRESSION
        "\\[  FAILED  \\] 7 test"
)

# test_returns_fail ensure proper failures
set_tests_properties(
    test_returns_fail
        PROPERTIES
        PASS_REGULAR_EXPRESSION
        "\\[  FAILED  \\] 3 test"
)

# test_exception_handler
if (TEST_EXCEPTION_HANDLER)
    if (WIN32)
        set_tests_properties(test_exception_handler
                                 PROPERTIES
                                     PASS_REGULAR_EXPRESSION
                                         "EXCEPTION_ACCESS_VIOLATION occurred at")
    else()
        set_tests_properties(test_exception_handler
                             PROPERTIES
                                 PASS_REGULAR_EXPRESSION
                                     "Test failed with exception: (Segmentation fault|Segmentation Fault|11|Illegal instruction)")
    endif (WIN32)
endif (TEST_EXCEPTION_HANDLER)

set_tests_properties(
    test_setup_fail
        PROPERTIES
        WILL_FAIL
        1
)

set_tests_properties(
    test_group_setup_assert
        PROPERTIES
        WILL_FAIL
        1
)

set_tests_properties(
    test_group_setup_fail
        PROPERTIES
        WILL_FAIL
        1
)

add_test(test_setup_fail_1_failed ${TARGET_SYSTEM_EMULATOR} test_setup_fail)
add_cmocka_test_environment(test_setup_fail_1_failed)
set_tests_properties(
    test_setup_fail_1_failed
        PROPERTIES
        PASS_REGULAR_EXPRESSION
        "\\[  ERROR   \\] int_test_ignored"
)

add_test (test_setup_fail_1_passed ${TARGET_SYSTEM_EMULATOR} test_setup_fail)
add_cmocka_test_environment(test_setup_fail_1_passed)
set_tests_properties(
    test_setup_fail_1_passed
        PROPERTIES
        PASS_REGULAR_EXPRESSION
        "\\[  PASSED  \\] 1 test\\(s\\)."
)

add_test (test_setup_fail_match_failed ${TARGET_SYSTEM_EMULATOR} test_setup_fail)
add_cmocka_test_environment(test_setup_fail_match_failed)
set_tests_properties(
    test_setup_fail_match_failed
        PROPERTIES
        PASS_REGULAR_EXPRESSION
        "\\[  ERROR   \\] int_test_ignored"
)

add_test (test_setup_fail_match_passed ${TARGET_SYSTEM_EMULATOR} test_setup_fail)
add_cmocka_test_environment(test_setup_fail_match_passed)
set_tests_properties(
    test_setup_fail_match_passed
        PROPERTIES
        PASS_REGULAR_EXPRESSION
        "\\[       OK \\] int_test_success"
)

### Output formats

# test output of success, failure, skip, fixture failure
set(OUTPUT_TESTS
    test_basics
    test_assert_macros_fail
    test_groups
    test_skip
    test_setup_fail)

set(TEST_OUTPUT_FMTS
    tap
    subunit
    xml)

set(test_basics_tap_out
    "^1\\.\\.2"
    "ok 1 - null_test_success"
    "ok 2 - int_test_success"
    "# ok - tests")
set(test_assert_macros_fail_tap_out
    "^1\\.\\.1"
    "not ok 1 - test_assert_return_code_fail"
    "#[^\n\r]+[\n\r]#[^\n\r]+[\n\r]# not ok - tests")
set(test_groups_tap_out
    "^1\\.\\.1"
    "ok 1 - null_test_success"
    "# ok - test_group1"
    "1\\.\\.1"
    "ok 1 - int_test_success"
    "# ok - test_group2")
set(test_skip_tap_out
    "not ok 1 # SKIP")
set(test_setup_fail_tap_out
    "not ok 1 - int_test_ignored Could not run test: Test setup failed")

set(test_basics_subunit_out
    "^test: null_test_success"
    "success: null_test_success")
set(test_assert_macros_fail_subunit_out
    "failure: test_assert_return_code_fail \\[")
set(test_groups_subunit_out
    "^test: null_test_success"
    "success: null_test_success")
set(test_skip_subunit_out
    "^test: test_check_skip"
    "skip: test_check_skip")
set(test_setup_fail_subunit_out
    "error: int_test_ignored \\[ Could not run test: Test setup failed \\]")

set(test_basics_xml_out
    "<testsuite name=\"tests\" time=\"[0-9.]+\" tests=\"2\" failures=\"0\" errors=\"0\" skipped=\"0\" >"
    "<testcase name=\"null_test_success\" time=\"[0-9.]+\" >.*</testcase>")
set(test_assert_macros_fail_xml_out
    "<testcase name=\"test_assert_return_code_fail\" time=\"[0-9.]+\" >"
    "<failure>")
set(test_groups_xml_out
    "^<\\?xml version=\"1.0\" encoding=\"UTF-8\" \\?>"
    "<testsuites>"
    "<testsuite name=\"test_group1\" time=\"[0-9.]+\" tests=\"1\" failures=\"0\" errors=\"0\" skipped=\"0\" >"
    "<testcase name=\"null_test_success\" time=\"[0-9.]+\" >"
    "</testcase>"
    "</testsuite>"
    ".*<testsuite name=\"test_group2\" time=\"[0-9.]+\" tests=\"1\" failures=\"0\" errors=\"0\" skipped=\"0\" >"
    "<testcase name=\"int_test_success\" time=\"[0-9.]+\" >"
    "</testcase>"
    "</testsuite>"
    "</testsuites>")
set(test_skip_xml_out
    "<testcase name=\"test_check_skip\" time=\"[0-9.]+\" >"
    "<skipped/>")
set(test_setup_fail_xml_out
    "<testcase name=\"int_test_ignored\" time=\"[0-9.]+\" >"
    "<failure><!\\[CDATA\\[Test setup failed\\]\\]></failure>")

foreach(_TEST_OUTPUT_FMT ${TEST_OUTPUT_FMTS})
    foreach(_OUTPUT_TEST ${OUTPUT_TESTS})
        set(TEST_NAME ${_OUTPUT_TEST}_${_TEST_OUTPUT_FMT})
        add_test(${TEST_NAME} ${TARGET_SYSTEM_EMULATOR} ${_OUTPUT_TEST})
        add_cmocka_test_environment(${TEST_NAME})

        set_property(
            TEST
            ${TEST_NAME}
            APPEND
            PROPERTY
                ENVIRONMENT CMOCKA_MESSAGE_OUTPUT=${_TEST_OUTPUT_FMT}
            )

        list(LENGTH ${TEST_NAME}_out len)
        list(GET ${TEST_NAME}_out 0 output)
        if(len GREATER 1)
            list(REMOVE_AT ${TEST_NAME}_out 0)
            foreach(line ${${TEST_NAME}_out})
                set(output "${output}[ \n\r]+${line}")
            endforeach()
        endif()

        set_tests_properties(
            ${TEST_NAME}
            PROPERTIES
            PASS_REGULAR_EXPRESSION
            ${output}
        )
    endforeach()
endforeach()
