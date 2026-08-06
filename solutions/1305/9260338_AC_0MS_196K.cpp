#define _CRT_SECURE_NO_WARNINGS
#include<cstdio>
#include<cmath>
#include<algorithm>

const int N_MAX=1000000;
bool inTriple[N_MAX+1];

bool isRelativelyPrime(int a,int b,int c){
	for(int i=2;i<=a;i++)
		if(a%i==0 && b%i==0 && c%i==0)
			return false;
	return true;
}

int main(){
	int n;
	while(scanf("%d",&n)!=EOF){
		std::fill(inTriple,inTriple+n+1,false);
		int tripleNum=0;
		for(int a=1;a<=n;a++){
			for(int b=a+1;b<=n && a*a+b*b<=n*n;b++){
				int c=(int)sqrt(double(a*a+b*b));
				if(a*a+b*b==c*c){
					inTriple[a]=inTriple[b]=inTriple[c]=true;
					if(isRelativelyPrime(a,b,c))
						tripleNum++;
				}
			}
		}
		int inTripleCount=0;
		for(int i=1;i<=n;i++)
			if(inTriple[i])
				inTripleCount++;
		printf("%d %d\n",tripleNum,n-inTripleCount);
	}
	return 0;
}