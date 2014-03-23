#define _CRT_SECURE_NO_WARNINGS

#include<cstdio>
#include<algorithm>
#include<cmath>

const int VERTEX_NUM_MAX=50;

double innerX,innerY;

struct Vertex{
	int x,y;
	bool operator<(const Vertex& a){
		return atan2(y-innerY,x-innerX)<atan2(a.y-innerY,a.x-innerX);
	}
};

int vertexNum=0;
Vertex vertices[VERTEX_NUM_MAX];

int main(){
	while(scanf("%d%d",&vertices[vertexNum].x,&vertices[vertexNum].y)!=EOF)
		vertexNum++;
	innerX=(vertices[0].x+vertices[1].x)/2.0;
	innerY=(vertices[0].y+vertices[1].y)/2.0;
	std::sort(vertices,vertices+vertexNum);
	for(int begin=0;begin<vertexNum;begin++){
		if(vertices[begin].x==0 && vertices[begin].y==0){
			for(int i=0;i<vertexNum;i++)
				printf("(%d,%d)\n",vertices[(i+begin)%vertexNum].x,vertices[(i+begin)%vertexNum].y);
			break;
		}
	}
	return 0;
}