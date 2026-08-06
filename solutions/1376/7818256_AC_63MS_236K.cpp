#define _CRT_SECURE_NO_WARNINGS

#include<cstdio>
#include<cassert>
#include<queue>

const int HEIGHT_MAX=50,WIDTH_MAX=50,STEP_MAX=3;
enum Orientation{
	ORI_BEGIN=0,ORI_NORTH=0,ORI_EAST,ORI_SOUTH,ORI_WEST,ORI_END
};

int height,width;
int timeArrive[HEIGHT_MAX][WIDTH_MAX][ORI_END];
bool block[HEIGHT_MAX][WIDTH_MAX];

struct Position{
	int row,col;
	bool operator==(const Position &a) const{
		return row==a.row && col==a.col;
	}
	Position forward(Orientation ori){
		Position next={row,col};
		switch(ori){
		case ORI_NORTH:
			next.row--;break;
		case ORI_EAST:
			next.col++;break;
		case ORI_SOUTH:
			next.row++;break;
		case ORI_WEST:
			next.col--;break;
		default:
			assert(true);
		}
		return next;
	}
	bool legal(){
		if(row-1<0 || col-1<0 || row>=height || col>=width)
			return false;
		return !block[row-1][col-1] && !block[row-1][col] && !block[row][col-1] && !block[row][col];
	}
};
struct Status{
	Position pos;
	Orientation ori;
	bool operator<(const Status &a) const{
		return timeArrive[pos.row][pos.col][ori]>timeArrive[a.pos.row][a.pos.col][a.ori];
	}
};

Status begin;
Position dest;

int turnCost(Orientation from,Orientation to){
	if(abs(from-to)==2)
		return 2;
	return 1;
}

int spfa(){
	for(int row=0;row<height;row++)
		for(int col=0;col<width;col++)
			for(Orientation ori=ORI_BEGIN;ori!=ORI_END;ori=(Orientation)(ori+1))
				timeArrive[row][col][ori]=INT_MAX;
	timeArrive[begin.pos.row][begin.pos.col][begin.ori]=0;
	std::priority_queue<Status> queue;
	queue.push(begin);
	while(!queue.empty()){
		Status current=queue.top();
		if(current.pos==dest)
			return timeArrive[current.pos.row][current.pos.col][current.ori];
		queue.pop();
		Status next=current;
		for(Orientation ori=ORI_BEGIN;ori!=ORI_END;ori=(Orientation)(ori+1)){
			if(ori==current.ori)
				continue;
			next.ori=ori;
			if(timeArrive[next.pos.row][next.pos.col][next.ori]>timeArrive[current.pos.row][current.pos.col][current.ori]+turnCost(next.ori,current.ori)){
				timeArrive[next.pos.row][next.pos.col][next.ori]=timeArrive[current.pos.row][current.pos.col][current.ori]+turnCost(next.ori,current.ori);
				queue.push(next);
			}
		}
		next.ori=current.ori;
		for(int step=0;step<STEP_MAX;step++){
			next.pos=next.pos.forward(next.ori);
			if(!next.pos.legal())
				break;
			if(timeArrive[next.pos.row][next.pos.col][next.ori]>timeArrive[current.pos.row][current.pos.col][current.ori]+1){
				timeArrive[next.pos.row][next.pos.col][next.ori]=timeArrive[current.pos.row][current.pos.col][current.ori]+1;
				queue.push(next);
			}
		}
	}
	return -1;
}

bool input(){
	scanf("%d%d",&height,&width);
	if(height==0 && width==0)
		return false;
	for(int row=0;row<height;row++)
		for(int col=0;col<width;col++)
			scanf("%d",block[row]+col);
#ifdef _DEBUG
	printf("# Maps:\n");
	for(int row=0;row<height;row++){
		for(int col=0;col<width;col++)
			printf("%d ",block[row][col]);
		printf("\n");
	}
#endif
	char orientation[6];
	scanf("%d%d%d%d%s",&begin.pos.row,&begin.pos.col,&dest.row,&dest.col,orientation);
	switch(orientation[0]){
	case 'n':
		begin.ori=ORI_NORTH;break;
	case 'w':
		begin.ori=ORI_WEST;break;
	case 's':
		begin.ori=ORI_SOUTH;break;
	case 'e':
		begin.ori=ORI_EAST;break;
	default:
		assert(true);
	}
#ifdef _DEBUG
	printf("# From (%d,%d) head %s to (%d,%d)\n",begin.pos.row,begin.pos.col,orientation,dest.row,dest.col);
#endif
	return true;
}

int main(){
	while(input())
		printf("%d\n",spfa());
	return 0;
}