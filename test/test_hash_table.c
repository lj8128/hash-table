#include "hash_table.h"
#include "test_setup_and_cleanup.h"
#include "test_execution.h"
#include "status_test_framework.h"
#include <stdio.h>
#include <stdlib.h>

TestResult test_hello_world() {
    printf("Hello, World!\n");
    return TEST_FAILED;
}

RoutineResult be_routine() {
    printf("I come before!\n");
    return ROUTINE_SUCCEEDED;
}

RoutineResult after_routine() {
    printf("I come after!\n");
    return ROUTINE_SUCCEEDED;
}

int main() {
    int res = SUCCESS;

    TestCatalog* catalog = malloc(sizeof(TestCatalog));
    init_test_catalog(catalog);

    register_test(catalog, "test_hello_world", test_hello_world);
    register_before_each_routine(catalog, be_routine);
    register_after_each_routine(catalog, after_routine);
    run_specific_test(catalog, "test_hello_world");
    run_all_tests(catalog);

    return res;
}
