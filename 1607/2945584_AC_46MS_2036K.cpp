#include<cstdio>

double Extend(int cardNum){
	if(cardNum==1)
		return 0.5;
	return 0.5/cardNum+Extend(cardNum-1);
}

int main(){
	printf("Cards  Overhang\n");
	int cardNum;
	while(scanf("%d",&cardNum)!=EOF)
		printf("%5d%10.3lf\n",cardNum,Extend(cardNum));
}