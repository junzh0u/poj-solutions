#include<cstdio>

//#define DEBUG

int main(){
	const int LIMIT=1300000;
	bool *isPrime=new bool[LIMIT];
	isPrime[0]=isPrime[1]=false;
	for(int i=2;i<LIMIT;i++)
		isPrime[i]=true;
	for(int i=2;i*i<=LIMIT;i++)
		if(isPrime[i])
			for(int j=2;i*j<LIMIT;j++)
				isPrime[i*j]=false;
#ifdef DEBUG
	for(int i=0;i<100;i++)
		if(isPrime[i])
			printf("%d ",i);
	printf("\n");
#endif
	while(true){
		int inGap;
		scanf("%d",&inGap);
		if(inGap==0)
			break;
		if(isPrime[inGap])
			printf("0\n");
		else{
			int less,greater;
			for(less=inGap-1;!isPrime[less];less--);
			for(greater=inGap+1;!isPrime[greater];greater++);
#ifdef DEBUG
			printf("%d-%d=",greater,less);
#endif
			printf("%d\n",greater-less);
		}
	}
	return 0;
}