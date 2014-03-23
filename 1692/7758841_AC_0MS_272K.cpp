#define _CRT_SECURE_NO_WARNINGS
#define DEBUG

#include<cstdio>
#include<algorithm>

const int NUM_MAX=100;

int firstNum,first[NUM_MAX];
int secondNum,second[NUM_MAX];
int indexInSecond[NUM_MAX+1][NUM_MAX+1],indexInFirst[NUM_MAX+1][NUM_MAX+1];
int matchNum[NUM_MAX+1][NUM_MAX+1];

int main(){
	int caseNum;
	scanf("%d",&caseNum);
	while(caseNum--){
		scanf("%d%d",&firstNum,&secondNum);
		for(int i=0;i<firstNum;i++)
			scanf("%d",first+i);
		for(int i=0;i<secondNum;i++)
			scanf("%d",second+i);

		for(int f=0;f<firstNum;f++){
			indexInSecond[f][0]=-1;
			for(int s=1;s<secondNum;s++){
				if(first[f]==second[s-1])
					indexInSecond[f][s]=s-1;
				else
					indexInSecond[f][s]=indexInSecond[f][s-1];
			}
		}
		for(int s=0;s<secondNum;s++){
			indexInFirst[0][s]=-1;
			for(int f=1;f<firstNum;f++){
				if(first[f-1]==second[s])
					indexInFirst[f][s]=f-1;
				else
					indexInFirst[f][s]=indexInFirst[f-1][s];
			}
		}
		
		for(int f=0;f<firstNum;f++)
			matchNum[f][0]=0;
		for(int s=0;s<secondNum;s++)
			matchNum[0][s]=0;

		for(int f=1;f<=firstNum;f++){
			for(int s=1;s<=secondNum;s++){
				matchNum[f][s]=std::max(matchNum[f-1][s],matchNum[f][s-1]);
				if(first[f-1]!=second[s-1]){
					int sPrev=indexInSecond[f-1][s-1],fPrev=indexInFirst[f-1][s-1];
					if(sPrev!=-1 && fPrev!=-1 && matchNum[f][s]<matchNum[fPrev][sPrev]+2)
						matchNum[f][s]=matchNum[fPrev][sPrev]+2;
				}
			}
		}
		
		printf("%d\n",matchNum[firstNum][secondNum]);
	}
	return 0;
}