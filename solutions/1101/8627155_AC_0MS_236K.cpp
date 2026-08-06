#define _CRT_SECURE_NO_WARNINGS

#include<cstdio>
#include<cstring>
#include<algorithm>
//#include<queue>
#include<cassert>

#undef _DEBUG

const int WIDTH_MAX=100,HEIGHT_MAX=100;
const char PIECE='X',BEGIN='B',END='E',EMPTY=' ';
const int QUEUE_SIZE_MAX=100000;

struct Coord{
	int row,col;
};

int id=0;
int width,height;
char board[WIDTH_MAX][HEIGHT_MAX];
Coord begin,end;

enum Direction{
	NONE,UP,DOWN,LEFT,RIGHT
};

struct Status{
	Coord coord;
	Direction direction;
	int segmentNum;
};

//std::queue<Status> bfs;
bool visited[WIDTH_MAX][HEIGHT_MAX];
Status queue[QUEUE_SIZE_MAX];
int queueBegin,queueEnd;

void head(Status current,Direction direction){
	int rowIncrement,colIncrement;
	switch(direction){
	case UP:
		rowIncrement=-1;
		colIncrement=0;
		break;
	case DOWN:
		rowIncrement=1;
		colIncrement=0;
		break;
	case LEFT:
		rowIncrement=0;
		colIncrement=-1;
		break;
	case RIGHT:
		rowIncrement=0;
		colIncrement=1;
		break;
	default:
		assert(true);
	}
	Status next;
	next.coord=current.coord;
	next.direction=direction;
	next.segmentNum=current.segmentNum+1;
	while(true){
		next.coord.col+=colIncrement;
		next.coord.row+=rowIncrement;
		if(next.coord.col<0 || next.coord.col>width+1 || next.coord.row<0 || next.coord.row>height+1)
			return;
		if(visited[next.coord.row][next.coord.col])
			return;
		if(board[next.coord.row][next.coord.col]==PIECE)
			return;
		//bfs.push(next);
		queue[queueEnd++]=next;
		visited[next.coord.row][next.coord.col]=true;
		if(board[next.coord.row][next.coord.col]==END)
			return;
	}
}

int connect(){
	//while(!bfs.empty())
	//	bfs.pop();
	for(int row=0;row<=height+1;row++)
		std::fill(visited[row],visited[row]+width+2,false);
	queueBegin=queueEnd=0;
	Status beginStatus;
	beginStatus.coord=begin;
	beginStatus.direction=NONE;
	beginStatus.segmentNum=0;
	//bfs.push(beginStatus);
	queue[queueEnd++]=beginStatus;
	visited[begin.row][begin.col]=true;
	//while(!bfs.empty()){
	while(queueBegin<queueEnd){
		//Status current=bfs.front();
		//bfs.pop();
		Status current=queue[queueBegin++];
		if(board[current.coord.row][current.coord.col]==END)
			return current.segmentNum;
		switch(current.direction){
		case NONE:
			head(current,UP);
			head(current,DOWN);
			head(current,LEFT);
			head(current,RIGHT);
			break;
		case UP:
		case DOWN:
			head(current,LEFT);
			head(current,RIGHT);
			break;
		case LEFT:
		case RIGHT:
			head(current,UP);
			head(current,DOWN);
			break;
		default:
			assert(true);
		}
	}
	return -1;
}

int main(){
	while(true){
		scanf("%d%d",&width,&height);
		if(!width && !height)
			return 0;
		for(int row=0;row<=height+1;row++){
			std::fill(board[row],board[row]+width+2,EMPTY);
			board[row][width+2]='\0';
		}
		getchar();
		for(int row=1;row<=height;row++){
			gets(board[row]+1);
			board[row][strlen(board[row])]=EMPTY;
		}
#ifdef _DEBUG
		puts("Board:");
		for(int row=0;row<=height+1;row++)
			puts(board[row]);
#endif
		int pairId=0;
		printf("Board #%d:\n",++id);
		while(true){
			scanf("%d%d%d%d",&begin.col,&begin.row,&end.col,&end.row);
			if(!begin.col && !begin.row && !end.col && !end.row)
				break;
			board[end.row][end.col]=END;
#ifdef _DEBUG
			puts("Board:");
			for(int row=0;row<=height+1;row++)
				puts(board[row]);
#endif
			int segmentNum=connect();
			printf("Pair %d: ",++pairId);
			if(segmentNum<0)
				printf("impossible.\n");
			else
				printf("%d segments.\n",segmentNum);
			board[end.row][end.col]=PIECE;
		}
		printf("\n");
	}
}