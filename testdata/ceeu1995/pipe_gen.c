#include <stdio.h>
#include <stdlib.h>

void main()
{
	int k, n, i;
	double x, y;

	for(k = 1000; k >= 0; k--)
	{
		n = 2 + rand() % 19;

		printf("%d\n", n);

		x = (20. / RAND_MAX) * rand() - 10.;
		y = (20. / RAND_MAX) * rand() - 10.;

		for(i = 0; i < n; i++)
		{
			x += .1 + (10. / RAND_MAX) * rand();
			y += (3. / RAND_MAX) * rand() - 1.5;

			printf("%.2f %.2f\n", x, y);
		}
	}

	printf("0\n");
}
