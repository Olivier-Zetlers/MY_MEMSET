#include <errno.h>
#include <stdio.h> // For fprintf()
#include <stdlib.h> // For malloc(), free()
#include <string.h> // For memset(), strerror()
#include "../headers/ft_memset_test_without_json_V2.h"

#define COMMAND_SYNTAX_ERROR 1
#define INVALID_TEST_DATA_ERROR 2

void *ft_memset(void *s, int c, size_t n); // Protype of ft_memset() - Avoids #including libft.h
unsigned int count_test_cases(void); // Function defined in ft_memset_test_data.c
void test_memset(struct test_case *test_cases, unsigned int nb_test_cases);

// MAIN FUNCTION
int main(int argc, char *argv[])
{
        unsigned int nb_test_cases; // Number of test cases in data file
        
        // Handle syntax errors
        if (argc != 1)
        {
                fprintf(stderr, "Wrong command syntax\n");
                printf("Please use syntax: %s\n", argv[0]);
                return (COMMAND_SYNTAX_ERROR);
        }
        
        // Count test cases
        nb_test_cases = count_test_cases();
        printf("Found %u test cases in ft_memset_test_test_cases.c\n\n", nb_test_cases);
        if (nb_test_cases == 0)
        {
                fprintf(stderr, "ft_memset_test_data.c contains 0 test case");
                return (INVALID_TEST_DATA_ERROR);
        }
        
        // Traverse test_cases array and perform testing
        test_memset(test_cases, nb_test_cases);

        return (0);
}

void test_memset(struct test_case *test_cases, unsigned int nb_test_cases)
{
        char *buf_libc;
        char *buf_libft;
        int res;
        
        for (unsigned int i = 0; i < nb_test_cases; i++)
        {
                // Announce test to be performed
                printf("PERFORMING NOW: %s\n", test_cases[i].test_case_name);
                printf("Buffer size: %zu\n", test_cases[i].buffer_size);
                printf("Constant byte: %d\n", test_cases[i].constant_byte);
                printf("Number of bytes written to buffer: %zu\n", test_cases[i].number_of_bytes);

                // Allocate memory for 2 testing buffers
                buf_libc = (char *) malloc(test_cases[i].buffer_size * sizeof(char));
                if (buf_libc == NULL)
                {
                        fprintf(stderr, "Could not allocate memory for buf_libc in %s. Skipping this test case.\n", test_cases[i].test_case_name);
                        continue;
                }
                buf_libft = (char *) malloc(test_cases[i].buffer_size * sizeof(char));
                if (buf_libft == NULL)
                {
                                fprintf(stderr, "Could not allocate memory for buf_libft in %s. Skipping this test case.\n", test_cases[i].test_case_name);
                                free(buf_libc);
                                continue;
                }
                
                // Apply memset and ft_memset to both testing buffers with same arguments
                memset(buf_libc, test_cases[i].constant_byte, test_cases[i].number_of_bytes);
                ft_memset(buf_libft, test_cases[i].constant_byte, test_cases[i].number_of_bytes);
                
                // Compare both testing buffers
                res = memcmp(buf_libc, buf_libft, test_cases[i].number_of_bytes);
                if (res != 0)
                        fprintf(stderr, "%s failed.\n\n", test_cases[i].test_case_name);
                else
                        printf("%s passed.\n\n", test_cases[i].test_case_name);
                free(buf_libc);
                free(buf_libft);
        }
}
