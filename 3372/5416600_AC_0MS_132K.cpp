#include<cstdio>

int main(){
	int n;
	while(scanf("%d",&n)!=EOF){
		while((n&1)==0)
			n>>=1;
		printf(n==1?"YES\n":"NO\n");
	}
	return true;
}