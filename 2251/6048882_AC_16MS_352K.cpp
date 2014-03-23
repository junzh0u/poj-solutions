#define _CRT_SECURE_NO_WARNINGS

#include<cstdio>
#include<queue>

#define DEBUG

const int LEVEL_NUM_MAX=30,ROW_NUM_MAX=30,COL_NUM_MAX=30,INF=-1;

bool dungoen[LEVEL_NUM_MAX][ROW_NUM_MAX][COL_NUM_MAX];
int time[LEVEL_NUM_MAX][ROW_NUM_MAX][COL_NUM_MAX];
int levelNum,rowNum,colNum;

struct Position{
	int level,row,col;
};
Position start,end;

typedef Position Direction;
const int DIRECTION_NUM=6;
const Direction DIRECTIONS[DIRECTION_NUM]={
	{1,0,0},{-1,0,0},{0,1,0},{0,-1,0},{0,0,1},{0,0,-1}
};

int main(){
	while(true){
		scanf("%d%d%d",&levelNum,&rowNum,&colNum);
		if(levelNum==0 && rowNum==0 && colNum==0)
			break;
		Position iter;
		for(iter.level=0;iter.level<levelNum;iter.level++)
			for(iter.row=0;iter.row<rowNum;iter.row++){
				char line[COL_NUM_MAX+1];
				scanf("%s",line);
				for(iter.col=0;iter.col<colNum;iter.col++){
					if(line[iter.col]=='#')
						dungoen[iter.level][iter.row][iter.col]=false;
					else{
						dungoen[iter.level][iter.row][iter.col]=true;
						if(line[iter.col]=='S')
							start=iter;
						else if(line[iter.col]=='E')
							end=iter;
					}
					time[iter.level][iter.row][iter.col]=INF;
				}
			}
		time[start.level][start.row][start.col]=0;
		int escapeTime=INF;
		std::queue<Position> BFS;
		BFS.push(start);
		while(!BFS.empty() && escapeTime==INF){
			Position current=BFS.front();
			BFS.pop();
			for(int i=0;i<DIRECTION_NUM;i++){
				Position next={	current.level+DIRECTIONS[i].level,
								current.row+DIRECTIONS[i].row,
								current.col+DIRECTIONS[i].col};
				if(	next.level>=0 && next.level<levelNum &&
					next.row>=0 && next.row<rowNum &&
					next.col>=0 && next.col<colNum &&
					dungoen[next.level][next.row][next.col] &&
					time[next.level][next.row][next.col]==INF){
					time[next.level][next.row][next.col]=time[current.level][current.row][current.col]+1;
					if(next.level==end.level && next.row==end.row && next.col==end.col){
						escapeTime=time[next.level][next.row][next.col];
						break;
					}
					BFS.push(next);
				}
			}
		}
		if(escapeTime!=INF)
			printf("Escaped in %d minute(s).\n",escapeTime);
		else
			printf("Trapped!\n");
	}
	return 0;
}