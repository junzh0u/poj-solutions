#define _CRT_SECURE_NO_WARNINGS

#include<cstdio>
#include<algorithm>

const int LAKE_NUM_MAX=10;

int lakeNum;
struct Lake{
	int index,degree;
	bool operator<(const Lake& a){
		return degree>a.degree;
	}
};
Lake lakes[LAKE_NUM_MAX];
bool connected[LAKE_NUM_MAX][LAKE_NUM_MAX];

bool constructGraph(Lake* lakeBegin,Lake* lakeEnd){
	if(lakeBegin==lakeEnd)
		return true;
	std::sort(lakeBegin,lakeEnd);
	if(lakeBegin+lakeBegin->degree>=lakeEnd)
		return false;
	for(Lake* lake=lakeBegin+1;lake<=lakeBegin+lakeBegin->degree;lake++){
		connected[lakeBegin->index][lake->index]=true;
		connected[lake->index][lakeBegin->index]=true;
		lake->degree--;
		if(lake->degree<0)
			return false;
	}
	return constructGraph(lakeBegin+1,lakeEnd);
}

void initializeGraph(){
	for(int from=0;from<lakeNum;from++)
		for(int to=0;to<lakeNum;to++)
			connected[from][to]=false;
}

int main(){
	int caseNum;
	scanf("%d",&caseNum);
	while(caseNum--){
		scanf("%d",&lakeNum);
		for(int i=0;i<lakeNum;i++){
			lakes[i].index=i;
			scanf("%d",&lakes[i].degree);
		}
		initializeGraph();
		if(constructGraph(lakes,lakes+lakeNum)){
			printf("YES\n");
			for(int from=0;from<lakeNum;from++){
				for(int to=0;to<lakeNum;to++)
					printf("%d ",connected[from][to]);
				printf("\n");
			}
		}
		else
			printf("NO\n");
		printf("\n");
	}
	return 0;
}