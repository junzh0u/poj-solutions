#define _CRT_SECURE_NO_WARNINGS

#include<cstdio>
#include<climits>
#include<algorithm>

const char ADD='t',MULTIPLY='x';
const int VERTEX_NUM_MAX=50;

int main(){
	int	vertexNum;
	scanf("%d",&vertexNum);

	char	edges[VERTEX_NUM_MAX+1];
	int		vertices[VERTEX_NUM_MAX];
	for(int i=0;i<vertexNum;i++)
		scanf("%s%d",edges+i,vertices+i);

	int	max[VERTEX_NUM_MAX+1][VERTEX_NUM_MAX];
	int	min[VERTEX_NUM_MAX+1][VERTEX_NUM_MAX];
	for(int begin=0;begin<vertexNum;begin++)
		max[1][begin]=min[1][begin]=vertices[begin];

	for(int length=2;length<=vertexNum;length++){
		for(int begin=0;begin<vertexNum;begin++){
			max[length][begin]=INT_MIN;
			min[length][begin]=INT_MAX;
			for(int left=1;left<length;left++){
				int mid=(begin+left)%vertexNum;
				if(edges[mid]==ADD){
					max[length][begin]=std::max(max[length][begin],max[left][begin]+max[length-left][mid]);
					min[length][begin]=std::min(min[length][begin],min[left][begin]+min[length-left][mid]);
				}
				else{
					int newMax=std::max(	std::max(	max[left][begin]*max[length-left][mid],
						max[left][begin]*min[length-left][mid]),
						std::max(	min[left][begin]*max[length-left][mid],
						min[left][begin]*min[length-left][mid]));
					max[length][begin]=std::max(max[length][begin],newMax);
					int	newMin=std::max(	std::min(	max[left][begin]*max[length-left][mid],
						max[left][begin]*min[length-left][mid]),
						std::min(	min[left][begin]*max[length-left][mid],
						min[left][begin]*min[length-left][mid]));
					min[length][begin]=std::min(min[length][begin],newMin);
				}
			}
		}
	}

	int	bestBegin[VERTEX_NUM_MAX],bestCount=0,scoreMax=INT_MIN;
	for(int begin=0;begin<vertexNum;begin++){
		if(scoreMax<max[vertexNum][begin]){
			scoreMax=max[vertexNum][begin];
			bestCount=0;
			bestBegin[bestCount++]=begin;
		}
		else if(scoreMax==max[vertexNum][begin])
			bestBegin[bestCount++]=begin;
	}

	printf("%d\n",scoreMax);
	for(int i=0;i<bestCount;i++)
		printf("%d ",bestBegin[i]+1);
	printf("\n");

	return 0;
}