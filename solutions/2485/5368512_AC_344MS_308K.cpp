#include<cstdio>
#include<functional>

int main(){
	int caseNum;
	scanf("%d",&caseNum);
	while(caseNum-->0){
		int num;
		scanf("%d",&num);
		int **distance=new int*[num];
		for(int i=0;i<num;i++){
			distance[i]=new int[num];
			for(int j=0;j<num;j++)
				scanf("%d",distance[i]+j);
		}
		bool *inGraph=new bool[num];
		inGraph[0]=true;
		int graphSize=1;
		for(int i=1;i<num;i++)
			inGraph[i]=false;
		int max=0;
		while(graphSize<num){
			int side=INT_MAX,to;
			for(int i=0;i<num;i++)
				if(inGraph[i])
					for(int j=0;j<num;j++)
						if(!inGraph[j] && side>distance[i][j]){
							side=distance[i][j];
							to=j;
						}
			inGraph[to]=true;
			graphSize++;
			if(max<side)
				max=side;
		}
		printf("%d\n",max);
		delete[] inGraph;
		for(int i=0;i<num;i++)
			delete[] distance[i];
		delete[] distance;
	}
	return 0;
}