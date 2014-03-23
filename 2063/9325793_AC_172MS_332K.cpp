#define _CRT_SECURE_NO_WARNINGS
#include<cstdio>
#include<algorithm>

const int AMOUNT_MAX_K=50000,YEAR_MAX=40,BOND_NUM_MAX=10;

int amount,yearNum,bondNum;
int values[BOND_NUM_MAX],interests[BOND_NUM_MAX];
int interestMax[AMOUNT_MAX_K+1];

int main(){
	int caseNum;
	scanf("%d",&caseNum);
	while(caseNum--){
		scanf("%d%d%d",&amount,&yearNum,&bondNum);
		for(int i=0;i<bondNum;i++)
			scanf("%d%d",values+i,interests+i);
		for(int year=0;year<yearNum;year++){
			std::fill(interestMax,interestMax+amount/1000+1,0);
			for(int amountK=1;amountK<=amount/1000;amountK++)
				for(int bond=0;bond<bondNum;bond++)
					if(amountK>=values[bond]/1000)
						interestMax[amountK]=std::max(interestMax[amountK],interestMax[amountK-values[bond]/1000]+interests[bond]);
			amount+=*std::max_element(interestMax,interestMax+amount/1000+1);
		}
		printf("%d\n",amount);
	}
	return 0;
}