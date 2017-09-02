/* Input Generator */
/* Problem Set: Central European Regional ACM ICPC 1995 */
/* Problem Name: Joseph */
/* Author: Michal Koucky */
/* Date: 1998/01/26 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int tabc[256];

void initgetrandomchar()
{
    int i;
    for(i=0;i!=256;i++)tabc[i]=0;
    for(i='a';i<='z';i++)tabc[i]=1;
    for(i='A';i<='Z';i++)tabc[i]=1;
    for(i='0';i<='9';i++)tabc[i]=1;
    tabc[' ']=1;
}

char getrandomchar()
{

    int c;
    while((c=getchar())!=EOF)if(tabc[c])return c;
    return 'x';
}

int randx(int max,int mask)
{
    int k;
    max--;
    while((k=(rand() & mask))>max);
    return k+1;
}


int perm[256];

void main()
{
	int i;
    
        initgetrandomchar();

        for(i=1;i<14;i++)printf("%i\n",i);
        for(i=1;i<1000;i++)printf("%i\n",randx(13,15));

	printf("0\n");
}

