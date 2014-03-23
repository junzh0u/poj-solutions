#define _CRT_SECURE_NO_WARNINGS
#include<cstdio>

const int NODE_NUM_MAX=20;

int main(){
	int nodeNum;
	scanf("%d",&nodeNum);
	int edge[NODE_NUM_MAX][NODE_NUM_MAX];
	for(int from=0;from<nodeNum;from++)
		for(int to=0;to<nodeNum;to++)
			scanf("%d",edge[from]+to);
	int trafficMax=0;
	for(int subnetwork=0;subnetwork<(1<<(nodeNum-1));subnetwork++){
		int traffic=0;
		for(int from=0;from<nodeNum;from++)
			if((subnetwork>>from)&1)
				for(int to=0;to<nodeNum;to++)
					if(!((subnetwork>>to)&1))
						traffic+=edge[from][to];
		if(trafficMax<traffic)
			trafficMax=traffic;
	}
	printf("%d\n",trafficMax);
	return 0;
}