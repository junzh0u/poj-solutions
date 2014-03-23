#define _CRT_SECURE_NO_WARNINGS

#include<cstdio>
#include<algorithm>
#include<cmath>

//#define DEBUG

struct Vertex{
	int x,y;
	//double x,y;
};

double distance(const Vertex &left,const Vertex &right){
	return sqrt(pow(double(left.x-right.x),2)+pow(double(left.y-right.y),2));
}

int vertexNum,edgeNum;
Vertex *vertices;
bool **connected;

double *minDistance;
int graphSize;

void link(int index){
	minDistance[index]=0;
	graphSize++;
	for(int next=0;next<vertexNum;next++){
		double temp=distance(vertices[index],vertices[next]);
		if(minDistance[next]<0 || minDistance[next]>temp)
			minDistance[next]=temp;
	}
	for(int next=0;next<vertexNum;next++)
		if(connected[index][next] && minDistance[next]>0.0)
			link(next);
}

double prim(){
	minDistance=new double[vertexNum];
	std::fill(minDistance,minDistance+vertexNum,-1.0);
	graphSize=0;

	link(0);

#ifdef DEBUG
	printf("index\tminDistance\n");
	for(int index=0;index<vertexNum;index++)
		printf("%d\t%.2f\n",index,minDistance[index]);
#endif

	double length=0.0;
	while(graphSize<vertexNum){
		int next=-1;
		for(int index=0;index<vertexNum;index++)
			if(minDistance[index]>0)
				if(next<0 || minDistance[next]>minDistance[index])
					next=index;
		length+=minDistance[next];
		link(next);

#ifdef DEBUG
		printf("index\tminDistance\n");
		for(int index=0;index<vertexNum;index++)
			printf("%d\t%.2f\n",index,minDistance[index]);
#endif

	}
	delete[] minDistance;
	return length;
}

int main(){
	scanf("%d%d",&vertexNum,&edgeNum);

	vertices=new Vertex[vertexNum];
	for(int index=0;index<vertexNum;index++)
		scanf("%d%d",&vertices[index].x,&vertices[index].y);
		//scanf("%lf%lf",&vertices[index].x,&vertices[index].y);

	connected=new bool*[vertexNum];
	for(int from=0;from<vertexNum;from++){
		connected[from]=new bool[vertexNum];
		std::fill(connected[from],connected[from]+vertexNum,false);
	}

	for(int index=0;index<edgeNum;index++){
		int from,to;
		scanf("%d%d",&from,&to);
		from--;
		to--;
		connected[from][to]=connected[to][from]=true;
	}

	printf("%.2f\n",prim());

	for(int from=0;from<vertexNum;from++)
		delete[] connected[from];
	delete[] connected;
	delete[] vertices;
	return 0;
}