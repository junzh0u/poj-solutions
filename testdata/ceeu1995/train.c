/*
	Problem: 	 Transportation
	Author:		 Jan Kotas
	Algorithm:   backtrack with restrains
	Complexity:	 exp
*/


#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int n; /* capacity */
int m; /* number of stations */
int count; /* number of orders */

struct order
{
	int from; /* from station */
	int to; /* to station */
	int passangers; /* no of passangers */
	int price; /* price */
	int remaining; /* sum of prices of this and all remainings orders in the sequence */
}
orders[32];

int train[16];

int best;

void try_it(
	int start, /* index of order to start with */
	int earnings) /* price of already allocated orders */
{
	if(earnings > best)
		best = earnings;

	for( ; start < count; start++)
	{
		int i;

		if(earnings + orders[start].remaining < best)
			return;

		for(i = orders[start].from; i < orders[start].to; i++)
		{
			if((train[i] += orders[start].passangers) > n)
				goto skip;
		}

		try_it(start + 1, earnings + orders[start].price);
		i--;	

skip:
		for( ; i >= orders[start].from; i--)
			train[i] -= orders[start].passangers;
	}
}

int price_cmp(const void *a, const void *b)
{
	return ((struct order *)b)->price - ((struct order *)a)->price;
}

int main(void)
{
	int i, s;

	for(;;)
	{
		scanf("%d%d%d", &n, &m, &count);

		if(!n && !m && !count) break;

		for(i = 0; i < count; i++)
		{
			scanf("%d%d%d", &orders[i].from, &orders[i].to, &orders[i].passangers);
			orders[i].price = (orders[i].to - orders[i].from) * orders[i].passangers;
		}

		qsort(orders, count, sizeof(orders[0]), price_cmp);

		for(s = 0, i = count - 1; i >= 0; i--)
			orders[i].remaining = (s += orders[i].price);

		memset(train, 0, sizeof(train[0]) * (m + 1));

		best = 0;

		try_it(0, 0);

		printf("%d\n", best);
	}
	return 0;
}
