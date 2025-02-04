#include <assert.h>
#include <errno.h>
#include <limits.h> // For LONG_MAX
#include <stdint.h> // For SIZE_MAX
#include <stdio.h> // For fopen(), fprintf()
#include <stdlib.h> // For malloc(), free()
#include <string.h> // For memset(), strerror()
#include "/home/oz/tools/cJSON/cJSON.h" // For cJSON_Parse(), cJSON_GetArraySize(), cJSON_Delete()
#include "/home/oz/MY_PROJECTS/MY_LIBFT/my_libft.h" // For ft_memset()

// Define Structure for storing test cases
struct test_case {
	char *test_case_name;
	size_t buffer_size;
	int constant_byte;
	size_t number_of_bytes; // Don't forget this semi-colon
}; // Don't forget this semi-colon

#define COMMAND_SYNTAX_ERROR 1
#define JSON_FILE_OPENING_ERROR 2
#define JSON_STRING_PARSING_ERROR 3
#define JSON_INVALID_ARRAY_ERROR 4
#define JSON_TEST_CASE_EXTRACTION_ERROR 5

char *read_JSON_file(const char * file_name);
struct test_case *test_case_extraction(cJSON *root, int size);
void test_memset(struct test_case *test_cases);

// MAIN FUNCTION
int main(int argc, char *argv[])
{
	char *JSON_string_temp; // For storing data read in JSON file as a string
	int array_size; // Number of test cases in JSON file
	cJSON *root; // For storing JSON tree that was parsed from JSON file
	struct test_case *test_cases; // For storing all test cases
	
	// Handle syntax erros
	if (argc != 2)
	{
		fprintf(stderr, "Wrong command syntax\n");
		printf("Please use syntax: ./program filename.json\n");
		return (COMMAND_SYNTAX_ERROR);
	}
	
	// Read JSON file into buffer (JSON_string_temp needs to be freed)
	JSON_string_temp = read_JSON_file(argv[1]);
	if (JSON_string_temp == NULL)
	{
		fprintf(stderr, "Couldn't read JSON file %s into buffer: %s\n", argv[1], strerror(errno));
		return (JSON_FILE_OPENING_ERROR);
	} 
	
	// Parse JSON string
	root = cJSON_Parse(JSON_string_temp); // Parses buffer storing JSON data read from file entered on CLI (needs freeing with cJSON_Delete)
	if (root == NULL)
	{
		fprintf(stderr, "Couldn't parse JSON string \n");
		free(JSON_string_temp);
		return (JSON_STRING_PARSING_ERROR);
	}
	free(JSON_string_temp); // Freeing string read from JSON file
	
	array_size = cJSON_GetArraySize(root);
	printf("Found %d test cases in %s\n\n", array_size, argv[1]);
	if (array_size == 0)
	{
		fprintf(stderr, "Parsed JSON is not a valid cJSON array");
		cJSON_Delete(root);
		return (JSON_INVALID_ARRAY_ERROR);
	}

	// Extract test cases from root and store them in test_cases
	test_cases = test_case_extraction(root, array_size); // test_cases needs to be freed
	if (test_cases == NULL)
	{
		fprintf(stderr, "Couldn't extract test cases from %s", argv[1]);
		cJSON_Delete(root);
		return (JSON_TEST_CASE_EXTRACTION_ERROR);
	}

	// Traverse test_cases array and perform testing
	test_memset(test_cases);

	// Cleanup
	cJSON_Delete(root); // ⚠️  the test_cases array points to strings that are owned by the cJSON nodes that are freed by this line
	//  the strings inside the cJSON structure become invalid after cJSON_Delete(root)
	free(test_cases);
	return (0);
}



// Read JSON data into buffer
// WARNING: this function transfers pointer ownership and requires freeing
char *read_JSON_file(const char * file_name)
{
	char *JSON_string;
	FILE *fp;
	long JSON_length; // Type long because ftell() returns a long

	// Open the JSON file
	fp = fopen(file_name, "rb"); // 'fp' stands for "file pointer", "rb" stands for "read only" as a binary stream
	if (fp == NULL)
	{
			fprintf(stderr, "Couldn't open %s: %s\n", file_name, strerror(errno));
			return NULL;
	}
	
	// Compute the length of the JSON file
	if (fseek(fp, 0, SEEK_END) !=0)
	{
		fprintf(stderr, "Couldn't reposition file pointer to the end of %s: %s\n", file_name, strerror(errno));
		fclose(fp);
		return (NULL);
	}
	JSON_length = ftell(fp); // ⚠️ This version of ftell() supposes that a long can support the file size
	if (JSON_length == -1L)
	{
		fprintf(stderr, "Couldn't obtain the current file position indicator: %s\n", strerror(errno));
		fclose(fp);
		return (NULL);
	}
	if (fseek(fp, 0, SEEK_SET) !=0) // Reset file position pointer to byte 0 before reading from file stream
	{
		fprintf(stderr, "Couldn't reposition file pointer to the beginning of %s: %s\n", file_name, strerror(errno));
		fclose(fp);
		return (NULL);
	}
	
	//Allocate memory on the heap to store the data of the JSON file
	if (LONG_MAX > SIZE_MAX)
		return (NULL);
	JSON_string = (char *) malloc(JSON_length + 1);
	if (JSON_string == NULL)
	{
		fprintf(stderr, "Couldn't allocate memory: %s\n", strerror(errno));
		fclose(fp); // Don't forget to close the file pointer to opened file
		return (NULL);
	}
	
	//Read the data of the JSON file into the buffer
	if (fread(JSON_string, sizeof(char), JSON_length, fp) < JSON_length)
	{
		fprintf(stderr, "Couldn't read JSON data into buffer: %s\n", strerror(errno));
		fclose(fp);
		free(JSON_string);
		return (NULL);
	}
	JSON_string[JSON_length] = '\0';
	fclose(fp);
	return (JSON_string); // JSON_data will need to be freed
	}


// Transfers pointer ownership of test_cases to caller
struct test_case *test_case_extraction(cJSON *root, int size)
{
	// Declare loop control variable i with type int because it counts the 'size' elements of 'root' JSON array ('size' is of type int)
	int i;

	// Declare counter of invalid test cases
	int f; 

	// Allocate array of type 'struct test_case' of size 'size + 1' (REMINDER: 'size' is the number of items in the root JSON array)
	struct test_case *test_cases = (struct test_case *) malloc(sizeof(struct test_case) * (size + 1));
	if (test_cases == NULL)
	{
		fprintf(stderr, "Couldn't allocate memory for test_cases: %s\n", strerror(errno));
		return (NULL); 
	}
	
	// Iterate in the root JSON array, extract data and populate the array test_cases (one array item by test case)
	cJSON *element; // Declares unitialized pointer to a cJSON node that will serve as list pointer
	i = 0;
	f = 0;
	cJSON_ArrayForEach(element, root) // Macro translates to for(element = ((root != NULL) : root->child ? NULL); element != NULL; element = element->child)
	{
		cJSON *test_case_name = cJSON_GetObjectItem(element, "Test case name");
		if (test_case_name == NULL || cJSON_IsString(test_case_name) == 0)
		{
			fprintf(stderr, "%d-th test case (0-based index) of JSON file is invalid: test_case_name is not a string\n", i);
			i++;
			f++;
			continue;
		}
		cJSON *buffer_size = cJSON_GetObjectItem(element, "Buffer size");
		if (buffer_size == NULL || cJSON_IsNumber(buffer_size) == 0)
		{
			fprintf(stderr, "%d-th test-case (0-based index) of JSON file is invalid: buffer_size is not a number\n", i);
			i++;
			f++;
			continue;
		}
		cJSON *constant_byte = cJSON_GetObjectItem(element, "Constant byte value (in decimal)"); // Reminder: memset expects an 'int' for the corresponding argument
		if (constant_byte == NULL || cJSON_IsNumber(constant_byte) == 0)
		{
			fprintf(stderr, "%d-th test case (0-based index) of JSON file is invalid: constant_byte is not a number\n\n", i);
			i++;
			f++;
			continue;
		}
		cJSON *number_of_bytes = cJSON_GetObjectItem(element, "Number of bytes to copy"); // Reminder: memset expects a 'size_t' for the corresponding argument
		if (number_of_bytes == NULL || cJSON_IsNumber(number_of_bytes) == 0)
		{
			fprintf(stderr, "%d-th test case (0-based index) of JSON file is invalid: number_of_bytes not being a number\n", i);
			i++;
			f++;
			continue;
		}
		if (number_of_bytes->valuedouble > buffer_size->valuedouble)
		{
			fprintf(stderr, "%d-th test case (0-based index) of JSON file is invalid: number_of_bytes exceeds buffer size\n", i);
			i++;
			f++;
			continue;
		}
		test_cases[i].test_case_name = (char *) test_case_name->valuestring; // ⚠️  cJSON node test_case_name keeps ownership of string
		test_cases[i].buffer_size = (size_t) buffer_size->valuedouble;
		test_cases[i].constant_byte = (int) constant_byte->valuedouble;
		test_cases[i].number_of_bytes = (size_t) number_of_bytes->valuedouble;
		i++;
	}
	
	printf("Found %d invalid test cases\n\n", f); 
	if (f == size)
		return (NULL); 

	// NULL-terminate test_cases array for coming traversal
	test_cases[i+1].test_case_name = NULL;

	return (test_cases);
}

void test_memset(struct test_case *test_cases)
{
	char *buf_libc;
	char *buf_libft;
	int res;
	
	for (int i = 0; test_cases[i].test_case_name != NULL; i++)
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
