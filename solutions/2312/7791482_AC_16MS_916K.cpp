#define _CRT_SECURE_NO_WARNINGS
#define DEBUG

#include<cstdio>
#include<queue>
#include<algorithm>
#include<cassert>

const int HEIGHT_MAX=500,WIDTH_MAX=500;
const char YOU='Y',TARGET='T',STEEL='S',BRICK='B',RIVER='R',EMPTY='E';
enum Direction{
	DIR_BEGIN=0,UP=0,DOWN,LEFT,RIGHT,DIR_END
};

int height,width;
char map[HEIGHT_MAX][WIDTH_MAX+1];
int turns[HEIGHT_MAX][WIDTH_MAX];

struct Pos{
	int row,col;
	Pos move(Direction dir){
		Pos next={row,col};
		switch(dir){
		case UP:
			next.row--;break;
		case DOWN:
			next.row++;break;
		case LEFT:
			next.col--;break;
		case RIGHT:
			next.col++;break;
		default:
			// Should not run through
			assert(true);
		}
		return next;
	}
	bool operator>(const Pos& another) const{
		return turns[row][col]>turns[another.row][another.col];
	}
};

Pos you,target;

int moveCost(Pos pos){
	if(pos.row<0 || pos.row>=height || pos.col<0 || pos.col>=width)
		return INT_MAX;
	switch(map[pos.row][pos.col]){
	case YOU:
	case TARGET:
	case EMPTY:
		return 1;
	case STEEL:
	case RIVER:
		return INT_MAX;
	case BRICK:
		return 2;
	default:
		// Should not run through
		assert(true);
		return INT_MAX;
	}
}

void spfa(){
	std::priority_queue<Pos,std::vector<Pos>,std::greater<Pos>> queue;
	queue.push(you);
	turns[you.row][you.col]=0;
	while(!queue.empty()){
		Pos current=queue.top();
		if(current.row==target.row && current.col==target.col)
			break;
		queue.pop();
		for(Direction dir=DIR_BEGIN;dir!=DIR_END;dir=(Direction)(dir+1)){
			Pos next=current.move(dir);
			int cost=moveCost(next);
			if(cost==INT_MAX)
				continue;
			if(turns[next.row][next.col]>turns[current.row][current.col]+cost){
				turns[next.row][next.col]=turns[current.row][current.col]+cost;
				queue.push(next);
			}
		}
	}
}

Pos find(char mark){
	Pos pos;
	for(pos.row=0;pos.row<height;pos.row++)
		for(pos.col=0;pos.col<width;pos.col++)
			if(map[pos.row][pos.col]==mark)
				return pos;
	// Should not run through
	assert(true);
	return pos;
}

int main(){
	while(true){
		// Input
		scanf("%d%d",&height,&width);
		if(height==0 && width==0)
			return 0;
		for(int row=0;row<height;row++)
			scanf("%s",map[row]);
		// Find YOU and TARGET
		you=find(YOU);
		target=find(TARGET);
		// SPFA
		for(int row=0;row<height;row++)
			std::fill(turns[row],turns[row]+width,INT_MAX);
		spfa();
		if(turns[target.row][target.col]==INT_MAX)
			printf("-1\n");
		else
			printf("%d\n",turns[target.row][target.col]);
	}
}