#include <string.h>

void *ft_memset(void *s, int c, size_t n)
{
	size_t	i; // If i were an unsigned int, UINT_MAX < SIZE_MAX on 64 bit platforms, causing trouble for large values of n
	unsigned char	*buf;

	i = 0;
	buf = (unsigned char *) s;

	while (i < n) // If were an unsigned int, UINT_MAX < SIZE_MAX on 64 bit platforms, causing trouble for large values of n
	{
		*(buf + i) = (unsigned char) c; // Equivalent to buf[i] =
		i++;
	}
	return (s); // Return the original pointer
}
