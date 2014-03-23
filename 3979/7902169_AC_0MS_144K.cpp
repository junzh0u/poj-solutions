#define _CRT_SECURE_NO_WARNINGS

#include<cstdio>
#include<cmath>

int gcd(int a,int b){
	if(a>b)
		return gcd(b,a);
	if(b%a==0)
		return a;
	return gcd(b%a,a);
}

int main(){
	char l[8];
	while(scanf("%s",l)!=EOF){
		int a=l[0]-'0',b=l[2]-'0',c=l[4]-'0',d=l[6]-'0';
		int e=a*d,f=b*d;
		if(l[3]=='+')
			e+=c*b;
		else
			e-=c*b;
		if(e==0)
			printf("0\n");
		else{
			int g=gcd(abs(e),f);
			printf("%d",e/g);
			if(f!=g)
				printf("/%d",f/g);
			printf("\n");
		}
	}
	return 0;
}