#include<cstdio>

int main(){
	while(true){
		int n,m;
		scanf("%d%d",&n,&m);
		if(n==0 && m==0)
			break;
		int newM=(m<n-m)?m:(n-m);
		__int64 result=1;
		for(int i=n,j=2;i>n-newM;i--){
			result*=i;
			while(j<=newM && result%j==0)
				result/=j++;
		}
		printf("%d things taken %d at a time is %I64d exactly.\n",n,m,result);
	}
	return 0;
}