#include<cstdio>
#include<cmath>

int main(){
	const double E=2.7182818284590452354;
	const double PI=2*asin(1.0);
	int caseNum;
	scanf("%d",&caseNum);
	while(caseNum-->0){
		int num;
		scanf("%d",&num);
		if(num==1)
			printf("1\n");
		else
			printf("%d\n",(int)ceil(log10(num/E)*num+0.5*log10(2.0*PI*num)));
	}
	return 0;
}