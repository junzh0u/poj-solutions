#define _CRT_SECURE_NO_WARNINGS

#include<cstdio>
#include<algorithm>
#include<functional>

const int SIZE=10000;
int p,q,r,s;
int dividend[2*SIZE],divisor[2*SIZE],divideTime;

int main(){
	while(scanf("%d%d%d%d",&p,&q,&r,&s)!=EOF){
		divideTime=divideTime=0;
		for(int i=0;i<q;i++){
			dividend[divideTime]=p-i;
			divisor[divideTime++]=q-i;
		}
		for(int i=0;i<s;i++){
			dividend[divideTime]=s-i;
			divisor[divideTime++]=r-i;
		}
		std::sort(dividend,dividend+divideTime,std::greater<int>());
		std::sort(divisor,divisor+divideTime,std::greater<int>());
		double result=1.0;
		for(int i=0;i<divideTime;i++){
			result*=dividend[i];
			result/=divisor[i];
		}
		printf("%.5f\n",result);
	}
	return 0;
}