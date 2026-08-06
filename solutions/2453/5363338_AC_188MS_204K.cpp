#include<cstdio>

int NumOfOne(int n){
	int count=0;
	for(;n>0;n/=2)
		if(n%2==1)
			count++;
	return count;
}

int main(){
	while(true){
		int n;
		scanf("%d",&n);
		if(n==0)
			break;
		int num=NumOfOne(n);
		for(n=n+1;;n++)
			if(NumOfOne(n)==num){
				printf("%d\n",n);
				break;
			}
	}
	return 0;
}