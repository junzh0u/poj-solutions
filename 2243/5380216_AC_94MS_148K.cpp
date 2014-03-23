#include<cstdio>
#include<queue>
//#include<algorithm>
using namespace std;

//#define DEBUG

#define BOARD_SIZE 8

struct Position{
	int x,y;
};

int main(){
	const int DIRECTION_NUM=8;
	const int X_INCREMENT[DIRECTION_NUM]={-2,-2,-1,-1,1,1,2,2},Y_INCREMENT[DIRECTION_NUM]={-1,1,-2,2,-2,2,-1,1};

	char sourceString[3],destinationString[3];
	bool visitedFromSource[BOARD_SIZE][BOARD_SIZE],visitedFromDestination[BOARD_SIZE][BOARD_SIZE];
	int stepUsed[BOARD_SIZE][BOARD_SIZE],stepRemain[BOARD_SIZE][BOARD_SIZE];
	while(scanf("%s%s",sourceString,destinationString)!=EOF){
		if(strcmp(sourceString,destinationString)==0){
			printf("To get from %s to %s takes 0 knight moves.\n",sourceString,destinationString);
			continue;
		}
		for(int row=0;row<BOARD_SIZE;row++){
			for(int col=0;col<BOARD_SIZE;col++){
				visitedFromSource[row][col]=visitedFromDestination[row][col]=false;
				stepUsed[row][col]=stepRemain[row][col]=0;
			}
		}
		Position source={sourceString[0]-'a',sourceString[1]-'1'},destination={destinationString[0]-'a',destinationString[1]-'1'};
		visitedFromSource[source.x][source.y]=true;
		stepUsed[source.x][source.y]=0;
		visitedFromDestination[destination.x][destination.y]=true;
		stepRemain[destination.x][destination.y]=0;
		queue<Position> BFSFromSource,BFSFromDestination;
		BFSFromSource.push(source);
		BFSFromDestination.push(destination);
		queue<Position> *BFS=&BFSFromSource,*oppsite=&BFSFromDestination;
		bool *visitedBFS=&visitedFromSource[0][0],*visitedOppsite=&visitedFromDestination[0][0];
		int *stepBFS=&stepUsed[0][0],*stepOppsite=&stepRemain[0][0];
		bool found=false;
		int stepTotal=0;
		while(!found && !BFS->empty()){
			Position current=BFS->front();
			BFS->pop();
			//vector<Position> nexts(current.Next());
			//for(int i=0;i<nexts.size();i++){
			for(int direction=0;direction<DIRECTION_NUM;direction++){
				//Position next(nexts[i]);
				Position next={current.x+X_INCREMENT[direction],current.y+Y_INCREMENT[direction]};
				if(next.x<0 || next.x>=BOARD_SIZE || next.y<0 || next.y>=BOARD_SIZE)
					continue;
				if(!visitedBFS[next.x*BOARD_SIZE+next.y]){
					stepBFS[next.x*BOARD_SIZE+next.y]=stepBFS[current.x*BOARD_SIZE+current.y]+1;
					visitedBFS[next.x*BOARD_SIZE+next.y]=true;
					if(visitedOppsite[next.x*BOARD_SIZE+next.y]){
						stepTotal=stepBFS[next.x*BOARD_SIZE+next.y]+stepOppsite[next.x*BOARD_SIZE+next.y];
						found=true;
						break;
					}
					BFS->push(next);
				}
			}
			swap(BFS,oppsite);
			swap(visitedBFS,visitedOppsite);
			swap(stepBFS,stepOppsite);
		}
#ifdef DEBUG
		printf("From Source:\n");
		for(int row=0;row<BOARD_SIZE;row++){
			for(int col=0;col<BOARD_SIZE;col++)
				printf("%d ",stepUsed[row][col]);
			printf("\n");
		}
		printf("From Destination:\n");
		for(int row=0;row<BOARD_SIZE;row++){
			for(int col=0;col<BOARD_SIZE;col++)
				printf("%d ",stepRemain[row][col]);
			printf("\n");
		}
#endif
		printf("To get from %s to %s takes %d knight moves.\n",sourceString,destinationString,stepTotal);
	}
	return 0;
}