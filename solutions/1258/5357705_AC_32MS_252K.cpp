#include<cstdio>
#include<cstdlib>
using namespace std;

int main(){
	int farmNum;
	while(scanf("%d",&farmNum)!=EOF){
		int **distance=new int*[farmNum];
		bool *inGraph=new bool[farmNum];
		for(int i=0;i<farmNum;i++){
			distance[i]=new int[farmNum];
			for(int j=0;j<farmNum;j++)
				scanf("%d",distance[i]+j);
			inGraph[i]=false;
		}
		inGraph[0]=true;
		int minLengthSum=0;
		for(int graphSize=1;graphSize<farmNum;graphSize++){
			int shortestSide=INT_MAX,to;
			for(int i=0;i<farmNum;i++)
				if(inGraph[i])
					for(int j=0;j<farmNum;j++)
						if(j!=i && !inGraph[j] && shortestSide>distance[i][j]){
							shortestSide=distance[i][j];
							to=j;
						}
			minLengthSum+=shortestSide;
			inGraph[to]=true;
		}
		printf("%d\n",minLengthSum);
		delete[] inGraph;
		for(int i=0;i<farmNum;i++)
			delete[] distance[i];
		delete[] distance;
	}
	return 0;
}