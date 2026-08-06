#define _CRT_SECURE_NO_WARNINGS
//#define DEBUG

#include<cstdio>

const int POISE_NUM=20;

unsigned POW_3[POISE_NUM];
void generatePow3Table(){
	POW_3[0]=1;
	for(int exp=1;exp<POISE_NUM;exp++)
		POW_3[exp]=POW_3[exp-1]*3;
}

void print(int* digits,int digitNum,int sign){
	bool first=true;
	for(int i=0;i<digitNum;i++){
		if(digits[i]==sign){
			if(first)
				first=false;
			else
				printf(",");
			printf("%u",POW_3[i]);
		}
	}
	if(first)
		printf("empty");
}

int main(){
	generatePow3Table();
	int caseNum;
	scanf("%d",&caseNum);
	while(caseNum--){
		unsigned weight;
		scanf("%u",&weight);
#ifdef DEBUG
		printf("# Weight = %u\n",weight);
#endif
		int digits[POISE_NUM+1],digitNum=0;
		while(weight){
			digits[digitNum++]=weight%3;
			weight/=3;
		}
		digits[digitNum]=0;
#ifdef DEBUG
		printf("# Digits based on 3:\n#");
		for(int i=0;i<digitNum;i++)
			printf(" %d",digits[i]);
		printf("\n");
#endif
		for(int i=0;i<digitNum;i++){
			if(digits[i]>=2){
				digits[i]-=3;
				digits[i+1]++;
			}
		}
		if(digits[digitNum])
			digitNum++;
#ifdef DEBUG
		printf("# After balanced:\n#");
		for(int i=0;i<digitNum;i++)
			printf(" %d",digits[i]);
		printf("\n");
#endif
		print(digits,digitNum,-1);
		printf(" ");
		print(digits,digitNum,1);
		printf("\n");
	}
	return 0;
}