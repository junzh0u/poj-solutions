#include<cstdio>

int main(){
	double n;
	while(scanf("%lf",&n)!=EOF){
		while(n>18.0)
			n/=18.0;
		if(n<=9.0)
			printf("Stan wins.\n");
		else
			printf("Ollie wins.\n");
	}
	return 0;
}