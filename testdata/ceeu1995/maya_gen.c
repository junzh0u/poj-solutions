/* Input Generator */
/* Problem Set: Central European Regional ACM ICPC 1995 */
/* Problem Name: Maya Calendar */
/* Author: Michal Koucky */
/* Date: 1998/01/24 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

char *haabm[]={ "pop", "no", "zip", "zotz", "tzec", "xul", "yoxkin", 
                "mol", "chen", "yax", "zac", "ceh", "mac", "kankin", 
                "muan", "pax", "koyab", "cumhu","uayet"}; 

int main()
{

    int i,j;

    printf("734\n");
    printf("10. zac 0\n");
    printf("0. pop 0\n");
    printf("10. zac 1995\n");
    printf("15. kankin 4999\n");

    for(i=0;i<19;i++)
      for(j=0;j<5 || (i<18 && j<20);j++)
            printf("%i. %s 186\n",j,haabm[i]);

    for(i=0;i<19;i++)
      for(j=0;j<5 || (i<18 && j<20);j++)
            printf("%i. %s %i\n",j,haabm[i],(i+1)*j);


    return 0;
}

