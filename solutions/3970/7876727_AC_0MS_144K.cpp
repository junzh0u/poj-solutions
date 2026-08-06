#define _CRT_SECURE_NO_WARNINGS

#include<cstdio>

const int PAY_LIMIT=1000000;

int gcd(int a,int b){
	if(a<b)
		return gcd(b,a);
	if(a%b)
		return gcd(b,a%b);
	return b;
}

int lcs(int a,int b){
	return a/gcd(a,b)*b;
}

int main(){
	while(true){
		int teamNum;
		scanf("%d",&teamNum);
		if(!teamNum)
			return 0;
		int pay=1;
		while(teamNum--){
			int employNum;
			scanf("%d",&employNum);
			pay=lcs(pay,employNum);
		}
		if(pay>=PAY_LIMIT)
			printf("Too much money to pay!\n");
		else
			printf("The CEO must bring %d pounds.\n",pay);
	}
}