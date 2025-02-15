#ifndef MEMSET_TEST_WITHOUT_JSON_V2_H
#define MEMSET_TEST_WITHOUT_JSON_V2_H

// Define Structure for storing test cases
#include <stddef.h> // For size_t

typedef struct test_case {
        char *test_case_name;
        size_t buffer_size;
        int constant_byte;
        size_t number_of_bytes; // Don't forget this semi-colon
} test_case; // Don't forget this semi-colon

extern test_case test_cases[]; // For explanations, see NOTION > extern > IN C > PRACTICAL APPLICATIONS > Struct definitions

#endif
