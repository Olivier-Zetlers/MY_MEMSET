ft_memset_test_with_json.c
Compile with: 
../../ft_memset.c
../libs/cJSON/cJSON.c
Call with: 
../data/ft_memset_test_cases.json (as CLI argument)


ft_memset_test_without_json_V1.c
Compile with: 
../../ft_memset.c
Comments: 
- Uses NULL as a sentinel value in 1st member of the last element of the test_cases array (not a good practice)
- Stores the test data within the .c file (not a good practice)


ft_memset_test_without_json_V2.c
Compile with:
../../ft_memset.c
../data/ft_memset_test_cases.c
Comments: 
- Stores the test data in separate .c data file (good practice)
- But the separate .c data file defines a function that counts test cases (not ideal practice)

