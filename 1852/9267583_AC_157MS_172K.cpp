#define _CRT_SECURE_NO_WARNINGS
#include<cstdio>
#include<algorithm>

int main(){
	int caseNum;
	scanf("%d",&caseNum);
	while(caseNum--){
		int length,antNum;
		scanf("%d%d",&length,&antNum);
		int earliest=0,latest=0;
		while(antNum--){
			int position;
			scanf("%d",&position);
			int early=position,late=length-position;
			if(early>late)
				std::swap(early,late);
			earliest=std::max(earliest,early);
			latest=std::max(latest,late);
		}
		printf("%d %d\n",earliest,latest);
	}
	return 0;
}