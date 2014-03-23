#include<cstdio>

const int LIMIT=1000000;
int wayNum[LIMIT+1]={0,1,2};

int main(){
	for(int i=4;i<=LIMIT;i+=2){
		wayNum[i-1]=wayNum[i-2];
		wayNum[i]=(wayNum[i-2]+wayNum[i/2])%1000000000;
	}
	int n;
	scanf("%d",&n);
	printf("%d\n",wayNum[n]);
	return 0;
}