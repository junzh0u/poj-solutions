/* Input Generator */
/* Problem Set: Central European Regional ACM ICPC 1995 */
/* Problem Name: Cipher */
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

void print_one_problem(int poc)
{
    int j,pj,i,n;

    n=randx(200,255);

    printf("%i\n",n);

    for(i=1;i<=n;i++)perm[i]=i;
    for(i=1;i<=n;i++){

       j=randx(n,255);
       pj = perm[j];
       perm[j]=perm[i];
       perm[i]=pj;
    }

    for(i=1;i<n;i++)printf("%i ",perm[i]);
    printf("%i\n",perm[n]);   

    j=randx(poc,511);
 
    while(j--){
        printf("%i ",randx(0x10000,0xFFFF));
        for(pj=randx(n,255);pj;pj--)printf("%c",getrandomchar());
        printf("\n");

    }

    printf("0\n");
}

void main()
{
	int i;
    
        initgetrandomchar();

        for(i=1;i<5;i++)print_one_problem(500);
        for(i=1;i<500;i++)print_one_problem(3);

	printf("0\n");
}

