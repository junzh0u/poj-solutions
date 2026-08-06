#include<cstdio>

inline int Question(int check,int remain){
	const int	THREE_ANSWER[10]={0,3,6,9,2,5,8,1,4,7},
				SEVEN_ANSWER[10]={0,7,4,1,8,5,2,9,6,3},
				NINE_ANSWER	[10]={0,1,2,3,4,5,6,7,8,9};
	switch(check){
		case 3:return THREE_ANSWER[remain];
		case 7:return SEVEN_ANSWER[remain];
		case 9:return NINE_ANSWER[remain];
		default:return -1;
	}
}
int main(){
	const int SIX[6]={7,3,9,7,3,9},SEVEN[7]={9,7,3,9,7,3,9};
	int caseNum;
	scanf("%d",&caseNum);
	for(int caseID=1;caseID<=caseNum;caseID++){
		char id[8];
		scanf("%s",id);
		const int *check=id[6]=='\0'?SIX:SEVEN;
		int checkSum=0,questionIndex;
		for(int i=0;id[i]!='\0';i++){
			if(id[i]=='?')
				questionIndex=i;
			else
				checkSum+=(id[i]-'0')*check[i];
		}
		id[questionIndex]='0'+Question(check[questionIndex],checkSum%10);
		printf("Scenario #%d:\n%s\n\n",caseID,id);
	}
	return 0;
}