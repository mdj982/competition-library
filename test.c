#include <stdio.h>
#include <stdint.h>
#include <string.h>

int main(void)
{
	uint32_t x = 10000000;
	uint32_t ax = &x;
	uint32_t y = *((uint32_t*)ax);
	printf("%d %d\n", *(&x), y);
	return 0;
}
