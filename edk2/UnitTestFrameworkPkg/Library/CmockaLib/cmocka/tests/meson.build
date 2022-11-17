tests = {
    'alloc': false,
    'group_setup_assert': true,
    'group_setup_fail': true,
    'fixtures': false,
    'group_fixtures': false,
    'groups': false,
    'float_macros': false,
    'assert_macros': false,
    'assert_macros_fail': true,
    'basics': false,
    'skip': false,
    'strmatch': false,
    'setup_fail': true,
    'ordering': false,
    'ordering_fail': true,
    'returns': false,
    'returns_fail': true,
    'wildcard': false,
    'skip_filter': false,
    'cmockery': false
}

foreach name, should_fail: tests
    exe = executable(name,
                     'test_@0@.c'.format(name),
                     include_directories: [cmocka_includes],
                     link_with: [libcmocka])
    test(name, exe, should_fail: should_fail)
endforeach
