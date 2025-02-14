#include "ft_memset_test_without_json.h"

struct test_case test_cases[] =
	//		char *test_case_name	size_t buffer_size	int constant_byte	size_t number_of_bytes
	{
		{	
			"Typical case 01",	100,			0,			100
		},
		{
			NULL,			0,			0,			0
		}
	};
		
