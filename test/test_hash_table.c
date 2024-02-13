#include "hash_table.h"
#include "test_setup_and_cleanup.h"
#include "test_execution.h"
#include "status_test_framework.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#define TEST_TABLE_SLOTS 701

HashTable* test_table;

static RoutineResult before_each() {
    test_table = calloc(1, sizeof(HashTable));
    return init_table(test_table, TEST_TABLE_SLOTS) < 0?
           ROUTINE_FAILED : ROUTINE_SUCCEEDED;
}

static RoutineResult after_each() {
    return delete_table(&test_table) < 0?
           ROUTINE_FAILED : ROUTINE_SUCCEEDED;
}

static TestResult test_entry_functions() {
    TestResult res = TEST_PASSED;

    int* val_1 = calloc(1, sizeof(int));
    int* val_2 = calloc(1, sizeof(int));
    int* val_3 = calloc(1, sizeof(int));

    *val_1 = 1;
    *val_2 = 2;
    *val_3 = 3;

    insert_entry(test_table, "key_1", (void*) val_1);
    insert_entry(test_table, "key_2", (void*) val_2);
    insert_entry(test_table, "key_3", (void*) val_3);

    if(test_table->size != 3) {
        print_freq("Table size should be correctly incremented!");
        res = TEST_FAILED;
        goto out;
    }

    TableEntry* returned_entry = search_for_entry(test_table, "key_2");
    if(returned_entry == 0
       || strcmp(returned_entry->key, "key_2") != 0
       || *((int*)returned_entry->value) != 2) {
        print_freq("Table should insert and search entry correctly!");
        res = TEST_FAILED;
        goto out;
    }

    delete_entry(test_table, returned_entry);
    if(test_table->size != 2) {
        print_freq("Table size should be correctly decremented!");
        res = TEST_FAILED;
        goto out;
    }

    if(search_for_entry(test_table, "key_2") != 0) {
        print_freq("Entry should have been deleted!");
        res = TEST_FAILED;
        goto out;
    }

out:
    free(val_1);
    free(val_2);
    free(val_3);
    return res;
}

int main() {
    int res = SUCCESS;

    TestCatalog* catalog = malloc(sizeof(TestCatalog));
    init_test_catalog(catalog);

    register_test(catalog, "test_entry_functions", test_entry_functions);

    register_before_each_routine(catalog, before_each);
    register_after_each_routine(catalog, after_each);

    run_all_tests(catalog);
    delete_test_catalog(&catalog);

    return res;
}

// int run_specific_test(TestCatalog* catalog, const char* test_name);
// int run_all_tests(TestCatalog* catalog);

// int init_test_catalog(TestCatalog* catalog);
// int register_test(TestCatalog* catalog, const char* test_name, TestResult (*test_function)());
// int register_before_each_routine(TestCatalog* catalog, RoutineResult (*before_each_routine)());
// int register_after_each_routine(TestCatalog* catalog, RoutineResult (*after_each_routine)());
// int delete_test_catalog(TestCatalog** catalog_ref);

// typedef enum {
//     TEST_PASSED = 0,
//     TEST_FAILED 
// } TestResult;

// typedef enum {
//     ROUTINE_SUCCEEDED = 0,
//     ROUTINE_FAILED 
// } RoutineResult;