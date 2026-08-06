#include<cstdio>
#include<cmath>

const int INF=-1;

struct Coordinate{
	int x,y;
};

double Distance(Coordinate left,Coordinate right){
	double dX=left.x-right.x,dY=left.y-right.y;
	return sqrt(dX*dX+dY*dY);
}

int GetRoot(int id,int *father){
	if(father[id]==INF || father[id]==id)
		return father[id];
	return father[id]=GetRoot(father[id],father);
}

int main(){
	int computerNum,communicateRange;
	scanf("%d%d",&computerNum,&communicateRange);
	Coordinate *computerPosition=new Coordinate[computerNum];
	int *father=new int[computerNum];
	for(int i=0;i<computerNum;i++){
		scanf("%d%d",&computerPosition[i].x,&computerPosition[i].y);
		father[i]=INF;
	}
	char operation[2];
	while(scanf("%s",operation)!=EOF){
		if(operation[0]=='O'){
			int repairID;
			scanf("%d",&repairID);repairID--;
			father[repairID]=repairID;
			for(int i=0;i<computerNum;i++)
				if(i!=repairID && GetRoot(i,father)!=INF && Distance(computerPosition[i],computerPosition[repairID])-communicateRange<=1e-6)
					father[GetRoot(i,father)]=repairID;
		}
		else{
			int leftID,rightID;
			scanf("%d%d",&leftID,&rightID);leftID--;rightID--;
			printf((GetRoot(leftID,father)==GetRoot(rightID,father) && GetRoot(leftID,father)!=INF)?"SUCCESS\n":"FAIL\n");
		}
	}
	delete[] father;
	delete[] computerPosition;
	return 0;
}