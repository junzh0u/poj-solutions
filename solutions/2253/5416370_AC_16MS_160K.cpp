#include<cstdio>
#include<cmath>

struct Stone{
	int x,y;
};

double Distance(const Stone &left,const Stone &right){
	double dX=left.x-right.x,dY=left.y-right.y;
	return sqrt(dX*dX+dY*dY);
}

int main(){
	int scenarioID=0;
	while(true){
		int stoneNum;
		scanf("%d",&stoneNum);
		if(stoneNum==0)
			break;
		Stone *stones=new Stone[stoneNum];
		bool *inTree=new bool[stoneNum];
		for(int i=0;i<stoneNum;i++){
			scanf("%d%d",&stones[i].x,&stones[i].y);
			inTree[i]=false;
		}
		inTree[0]=true;
		//int treeSize=1;
		double distanceFrog=0.0;
		while(!inTree[1]){
			double distanceMin=2000.0;
			int to;
			for(int i=0;i<stoneNum;i++)
				if(inTree[i])
					for(int j=0;j<stoneNum;j++)
						if(!inTree[j]){
							double distance=Distance(stones[i],stones[j]);
							if(distanceMin>distance){
								distanceMin=distance;
								to=j;
							}
						}
			inTree[to]=true;
			if(distanceFrog<distanceMin)
				distanceFrog=distanceMin;
		}
		printf("Scenario #%d\nFrog Distance = %.3f\n\n",++scenarioID,distanceFrog);
		delete[] inTree;
		delete[] stones;
	}
	return 0;
}