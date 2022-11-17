/*
 * Copyright 2019 Arnaud Gelas <arnaud.gelas@sensefly.com>
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

/* Use the unit test allocators */
#define UNIT_TESTING 1

#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <cmocka.h>

/* A test case that does check if float is equal. */
static void float_test_success(void **state)
{
    (void)state; /* unused */

    assert_float_equal(0.5f, 1.f / 2.f, 0.000001f);
    assert_float_not_equal(0.5, 0.499f, 0.000001f);
}

int main(void) {
    const struct CMUnitTest tests[] = {
        cmocka_unit_test(float_test_success),
    };

    return cmocka_run_group_tests(tests, NULL, NULL);
}
