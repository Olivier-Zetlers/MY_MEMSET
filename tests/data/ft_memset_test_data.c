#include "../headers/ft_memset_test_without_json_V2.h"

unsigned int count_test_cases(void);

// Test cases
test_case test_cases[] = {
	{		"Typical case 01",		100,		0,		100		}, // Format { 2 tabs data ... 2 tabs }
	{		"Typical case 02",		50,		64,		50		},
	{		"Edge case 01",			10,		65,		0		},
        {		"Edge case 02",			0,		97,		0		},
	{		"Edge case 03",			1000,		97,		1000		},
	{		"Edge case 04",			1000,		0,		1000		},
	{		"Edge case 05",			1000,		127,		1000		},
	{		"Edge case 06",			1000,		128,		1000		},
	{		"Stress case 01",		100000000,	97,		100000000	},
}; 


unsigned int count_test_cases(void)
{
	return (sizeof(test_cases)/sizeof(test_case)); // sizeof expects pointer to data structure, not test_cases[] notation
}
