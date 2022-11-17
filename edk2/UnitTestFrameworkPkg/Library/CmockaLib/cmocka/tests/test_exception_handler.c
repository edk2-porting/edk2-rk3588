#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <cmocka.h>

#include <stdlib.h>

struct test_segv {
    int x;
    int y;
};

static void test_segfault_recovery(void **state)
{
    struct test_segv *s = NULL;

    (void) state; /* unused */

    s->x = 1;
}

static void test_segfault_recovery1(void **state)
{
    test_segfault_recovery(state);
}

static void test_segfault_recovery2(void **state)
{
    test_segfault_recovery(state);
}

static void test_segfault_recovery3(void **state)
{
    test_segfault_recovery(state);
}

int main(void) {
    const struct CMUnitTest tests[] = {
        cmocka_unit_test(test_segfault_recovery1),
        cmocka_unit_test(test_segfault_recovery2),
        cmocka_unit_test(test_segfault_recovery3),
    };

    return cmocka_run_group_tests(tests, NULL, NULL);
}
