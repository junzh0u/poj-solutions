#include<cstdio>

const int POTION_NUM_MAX=150000;
int strength[POTION_NUM_MAX+1];

int main(){
	int potionNum;
	scanf("%d",&potionNum);
	for(int i=0;i<potionNum;i++)
		scanf("%d",strength+i);
	strength[potionNum]=0;
	int sign=1,sum=0;
	for(int i=0;i<potionNum;i++)
		if(sign==1){
			if(strength[i+1]<strength[i]){
				sum+=strength[i];
				sign=-sign;
			}
		}
		else{
			if(strength[i+1]>strength[i]){
				sum-=strength[i];
				sign=-sign;
			}
		}
	printf("%d\n",sum);
	return 0;
}