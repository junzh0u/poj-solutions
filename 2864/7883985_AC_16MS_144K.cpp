#define _CRT_SECURE_NO_WARNINGS

#include<cstdio>

const int ALUMNI_NUM_MAX=100;

int main(){
	while(true){
		int alumniNum,dinnerNum;
		scanf("%d%d",&alumniNum,&dinnerNum);
		if(!alumniNum && !dinnerNum)
			return 0;
		int attend[ALUMNI_NUM_MAX];
		for(int alumni=0;alumni<alumniNum;alumni++)
			attend[alumni]=1;
		while(dinnerNum--){
			for(int alumni=0;alumni<alumniNum;alumni++){
				int temp;
				scanf("%d",&temp);
				attend[alumni]&=temp;
			}
		}
		int result=0;
		for(int alumni=0;alumni<alumniNum;alumni++)
			result|=attend[alumni];
		printf(result?"yes\n":"no\n");
	}
}