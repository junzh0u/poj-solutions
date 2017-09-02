#include <stdio.h>
#include <stdlib.h>

void main()
{
	int k, m, n, count, a, b;

	for(k = 256; k >= 0; k--)
	{
		n = 1 + rand() % 5000;
		m = 1 + rand() % 7;
		count = rand() % 23;

		printf("%d %d %d\n", n, m, count);

		for( ; count > 0; count--)
		{
again:
			a = rand() % (m + 1);
			b = rand() % (m + 1);

			if(a >= b) goto again;

			printf("%d %d %d\n", a, b, 1 + rand() % 1000);
		}
	}

	printf("0 0 0\n");
}
