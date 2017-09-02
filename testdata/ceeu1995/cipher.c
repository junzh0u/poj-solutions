/* Problem Set: Central European Regional ACM ICPC 1995 */
/* Problem Name: Cipher */
/* Author: Michal Koucky */
/* Date: 1998/01/24 */
/* Used Method: permutation decomposition in cycles */

#include <stdio.h>
#include <string.h>

#define MAXLEN  256

int perm[MAXLEN];           /* original permutation */
int n;                      /* length of permutation */
int pnum[MAXLEN],           /* number of i-th element in its cycle */
    cyc[MAXLEN];            /* and its cycle number */
int *bicycles[MAXLEN];      /* pointer on first element of i-th cycle in cycles */
int cycles[MAXLEN+2];       /* ordered separate cycles */
int lcycl[MAXLEN];          /* length of i-th cycle */
int ncycles;                /* number of cycles */

char s[MAXLEN+200], t[MAXLEN+10]; /* input and output string */

void decompose()
/* decomposes permutation in arrays */
{
    int i,j,k,c=0;
    int *freec=cycles;

    for(i=1;i<=n;i++)cyc[i]=0;

    for(i=1;i<=n;i++)if(cyc[i]==0){
        c++;
        bicycles[c]=freec;

        k=0;
        for(j=i;cyc[j]==0;j=perm[j]){
            *freec++ = j;
            cyc[j]=c;
            pnum[j]=k++;            
        }

        lcycl[c]=k;        

    }

    ncycles=c;
}

void permutate(int k)
/* computes k-th permutation of s to t */
{
    int i,c;

    for(i=1;s[i-1];i++){
        c = cyc[i];
        
        t[ bicycles[c][ (pnum[i] + k) % lcycl[c]] - 1 ]=s[i-1];

    }

}

int main()
{
    int i,k;

    while(1){

        scanf("%d",&n);

        if(!n)break;

        for(i=1;i<=n;i++)scanf("%d",perm+i);

        decompose();

        while(1){
            scanf("%d",&k);
            if(!k)break;

            getchar();
            for(i=0;i<=n;i++)if((s[i]=getchar())=='\n')break;
            while(i<n)s[i++]=' ';
            s[n]=0;

            permutate(k);
            t[n]=0;
            printf("%s\n",t);
        }

        printf("\n");
    }

    return 0;
}

