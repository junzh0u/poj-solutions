#include<cstdio>
#include<algorithm>
using namespace std;


int main(){
	const int PRE_PROCESS_SIZE=40000;
	bool prime[PRE_PROCESS_SIZE];
	fill(prime,prime+PRE_PROCESS_SIZE,true);
	prime[0]=prime[1]=false;
	for(int i=2;i<PRE_PROCESS_SIZE;i++)
		if(prime[i])
			for(int j=2;i*j<PRE_PROCESS_SIZE;j++)
				prime[i*j]=false;
	while(true){
		int n;
		scanf("%d",&n);
		if(n==0)
			break;
		int count=1;
		for(int i=2;i*i<=n;i++){
			if(prime[i] && n%i==0){
				count*=i-1;
				n/=i;
				while(n%i==0){
					count*=i;
					n/=i;
				}
			}
		}
		if(n>1)
			count*=n-1;
		printf("%d\n",count);
	}
	return 0;
}