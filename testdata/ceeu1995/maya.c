/* Problem Set: Central European Regional ACM ICPC 1995 */
/* Problem Name: Maya Calendar */
/* Author: Michal Koucky */
/* Date: 1998/01/24 */
/* Used Method: simple maya algorithm */

#include <stdio.h>
#include <string.h>

char *haabm[]={ "pop", "no", "zip", "zotz", "tzec", "xul", "yoxkin", 
                "mol", "chen", "yax", "zac", "ceh", "mac", "kankin", 
                "muan", "pax", "koyab", "cumhu", "uayet"}; 
char *tzolkinm[]={ "imix", "ik", "akbal", "kan", "chicchan", "cimi",
                "manik", "lamat", "muluk", "ok", "chuen", "eb", "ben", 
                "ix", "mem", "cib", "caban", "eznab", "canac", "ahau"};


int haab2int(int year, char *month, int day)
{

    int m;

    for(m=0;m<19;m++)if(!strcmp(month,haabm[m]))break;

    return year * 365 + m * 20 + day;

}

int main()
{

    int n;
    int hy,hd,ndays;
    char hm[20];

    scanf("%d",&n);

    printf("%i\n",n);

    while(n--){


        scanf("%d.%s%d",&hd,hm,&hy);
    
        ndays = haab2int(hy,hm,hd);


        printf("%i %s %i\n",1+(ndays % 13),tzolkinm[ndays % 20],ndays / 260);


    }


    return 0;
}

