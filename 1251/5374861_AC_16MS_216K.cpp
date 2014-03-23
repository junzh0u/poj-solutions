#include<iostream>
#include<algorithm>
using namespace std;

int main(){
	while(true){
		int size;
		cin>>size;
		if(size==0)
			break;
		int **cost=new int*[size];
		for(int i=0;i<size;i++){
			cost[i]=new int[size];
			fill(cost[i],cost[i]+size,INT_MAX);
		}
		for(int i=0;i<size-1;i++){
			char from;
			int nextNum;
			cin>>from>>nextNum;
			while(nextNum-->0){
				char to;
				int roadCost;
				cin>>to>>roadCost;
				if(cost[from-'A'][to-'A']>roadCost)
					cost[to-'A'][from-'A']=cost[from-'A'][to-'A']=roadCost;
			}
		}
		bool *inGraph=new bool[size];
		fill(inGraph,inGraph+size,false);
		inGraph[0]=true;
		int graphSize=1,costSum=0;
		while(graphSize<size){
			int costMin=INT_MAX,to;
			for(int i=0;i<size;i++)
				if(inGraph[i])
					for(int j=0;j<size;j++)
						if(!inGraph[j] && costMin>cost[i][j]){
							costMin=cost[i][j];
							to=j;
						}
			inGraph[to]=true;
			graphSize++;
			costSum+=costMin;
		}
		cout<<costSum<<endl;
		delete[] inGraph;
		for(int i=0;i<size;i++)
			delete[] cost[i];
		delete[] cost;
	}
	return 0;
}