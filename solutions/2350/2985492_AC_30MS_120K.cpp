#include<cstdio>

int main(){
	const int PEOPLE_NUM_MAX=1000;
	int caseNum;
	scanf("%d",&caseNum);
	while(caseNum-->0){
		int peopleNum;
		scanf("%d",&peopleNum);
		double grade[PEOPLE_NUM_MAX],average=0.0;
		for(int people=0;people<peopleNum;people++){
			scanf("%lf",&grade[people]);
			average+=grade[people];
		}
		average/=peopleNum;
		int above=0;
		for(people=0;people<peopleNum;people++)
			if(grade[people]>average)
				above++;
		printf("%.3lf%%\n",(100.0*above)/peopleNum);
	}
	return 0;
}