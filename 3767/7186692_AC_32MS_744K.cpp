#define _CRT_SECURE_NO_WARNINGS

#include<cstdio>
#include<algorithm>

struct Road{
	int from,to,cost;
};

const int CITY_NUM_MAX=600,ROAD_NUM_MAX=10000,LEADER_NUM_MAX=2,COST_INF=-1;
int cityNum,roadNum;
Road roads[ROAD_NUM_MAX];
int costs[CITY_NUM_MAX][CITY_NUM_MAX];
int cityLeaders[CITY_NUM_MAX];
int sameCampCost[LEADER_NUM_MAX][CITY_NUM_MAX];

int main(){
	while(true){
		scanf("%d",&cityNum);
		if(cityNum==0)
			return 0;
		scanf("%d",&roadNum);
		for(int cityFrom=0;cityFrom<cityNum;cityFrom++)
			std::fill(costs[cityFrom],costs[cityFrom]+cityNum,COST_INF);
		for(int roadIndex=0;roadIndex<roadNum;roadIndex++){
			scanf("%d%d%d",&roads[roadIndex].from,&roads[roadIndex].to,&roads[roadIndex].cost);
			roads[roadIndex].from--;
			roads[roadIndex].to--;
			costs[roads[roadIndex].from][roads[roadIndex].to]=
				costs[roads[roadIndex].to][roads[roadIndex].from]=
				roads[roadIndex].cost;
		}
		for(int cityIndex=0;cityIndex<cityNum;cityIndex++){
			scanf("%d",cityLeaders+cityIndex);
			cityLeaders[cityIndex]--;
		}
		for(int leader=0;leader<LEADER_NUM_MAX;leader++)
			std::fill(sameCampCost[leader],sameCampCost[leader]+cityNum,COST_INF);
		sameCampCost[0][0]=sameCampCost[1][1]=0;
		for(int leader=0;leader<LEADER_NUM_MAX;leader++){
			int *campCost=sameCampCost[leader];
			for(int round=0;round<cityNum-1;round++){
				bool anyChange=false;
				for(int roadIndex=0;roadIndex<roadNum;roadIndex++){
					Road road=roads[roadIndex];
					int from=road.from,to=road.to,cost=road.cost;
					for(int direction=0;direction<2;direction++){
						if(	cityLeaders[from]==leader &&
							cityLeaders[to]==leader &&
							campCost[from]!=COST_INF &&
							(	campCost[to]==COST_INF ||
								campCost[to]>campCost[from]+cost)){
							campCost[to]=campCost[from]+cost;
							anyChange=true;
						}
						std::swap(from,to);
					}
				}
				if(!anyChange)
					break;
			}
		}
		int costMin=COST_INF;
		for(int cityIndexA=0;cityIndexA<cityNum;cityIndexA++)
			if(cityLeaders[cityIndexA]==0){
				int costA=sameCampCost[0][cityIndexA];
				if(costA!=COST_INF){
					for(int cityIndexB=0;cityIndexB<cityNum;cityIndexB++)
						if(cityLeaders[cityIndexB]==1){
							int costB=sameCampCost[1][cityIndexB];
							if(	costB!=COST_INF &&
								costs[cityIndexA][cityIndexB]!=COST_INF &&
								(	costMin==COST_INF ||
									costMin>costA+costs[cityIndexA][cityIndexB]+costB)){
								costMin=costA+costs[cityIndexA][cityIndexB]+costB;
							}
						}
				}
			}
		printf("%d\n",costMin);
	}
}