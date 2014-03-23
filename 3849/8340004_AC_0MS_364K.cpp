#define _CRT_SECURE_NO_WARNINGS

#include<cstdio>
#include<algorithm>
#include<cassert>

const int INDEX_MAX=10000;
struct Coord{
	int row,col;
};
Coord coords[INDEX_MAX];

void initializeCoords(){
	coords[0].row=0;
	coords[0].col=0;
	coords[1].row=0;
	coords[1].col=1;
	for(int index=2;index<INDEX_MAX;index++){
		if(coords[index-1].row>=0 && coords[index-1].col>0){
			if(coords[index-1].row<coords[index-1].col){
				coords[index].row=coords[index-1].row+1;
				coords[index].col=coords[index-1].col;
			}
			else{
				coords[index].row=coords[index-1].row;
				coords[index].col=coords[index-1].col-1;
			}
		}
		else if(coords[index-1].row>0 && coords[index-1].col<=0){
			coords[index].row=coords[index-1].row-1;
			coords[index].col=coords[index-1].col-1;
		}
		else if(coords[index-1].row<=0 && coords[index-1].col<0){
			if(coords[index-1].row>coords[index-1].col){
				coords[index].row=coords[index-1].row-1;
				coords[index].col=coords[index-1].col;
			}
			else{
				coords[index].row=coords[index-1].row;
				coords[index].col=coords[index-1].col+1;
			}
		}
		else if(coords[index-1].row<0 && coords[index-1].col>=0){
			if(coords[index-1].col==0){
				coords[index].row=coords[index-1].row;
				coords[index].col=coords[index-1].col+1;
			}
			else{
				coords[index].row=coords[index-1].row+1;
				coords[index].col=coords[index-1].col+1;
			}
		}
	}
}

const int BOARD_SIZE=200;
const int ZERO_ROW=100,ZERO_COL=100;
const int RESOURCE_MAX=5;
int board[BOARD_SIZE][BOARD_SIZE];
int resourceCount[RESOURCE_MAX+1];

int get(int row,int col){
	return board[ZERO_ROW+row][ZERO_COL+col];
}

void set(int row,int col,int value){
	board[ZERO_ROW+row][ZERO_COL+col]=value;
}

void initializeBoard(){
	for(int i=0;i<BOARD_SIZE;i++)
		std::fill(board[i],board[i]+BOARD_SIZE,0);
	std::fill(resourceCount,resourceCount+RESOURCE_MAX+1,0);
}

int getResource(Coord coord){
	bool possible[RESOURCE_MAX+1];
	std::fill(possible,possible+RESOURCE_MAX+1,true);
	possible[get(coord.row,coord.col+1)]=false;
	possible[get(coord.row+1,coord.col+1)]=false;
	possible[get(coord.row+1,coord.col)]=false;
	possible[get(coord.row,coord.col-1)]=false;
	possible[get(coord.row-1,coord.col-1)]=false;
	possible[get(coord.row-1,coord.col)]=false;
	int countMin=INT_MAX;
	int result=1;
	for(int resource=1;resource<=RESOURCE_MAX;resource++){
		if(possible[resource] && resourceCount[resource]<countMin){
			result=resource;
			countMin=resourceCount[resource];
		}
	}
	return result;
}

void fillBoard(){
	initializeBoard();
	for(int index=0;index<INDEX_MAX;index++){
		Coord coord=coords[index];
		int resource=getResource(coord);
		set(coord.row,coord.col,resource);
		resourceCount[resource]++;
	}
}

int main(){
	initializeCoords();
	fillBoard();
#ifdef _DEBUG
	for(int index=0;index<100;index++)
		printf("%d\t",get(coords[index].row,coords[index].col));
	printf("\n");
#endif
	int caseNum;
	scanf("%d",&caseNum);
	while(caseNum--){
		int index;
		scanf("%d",&index);
		printf("%d\n",get(coords[index-1].row,coords[index-1].col));
	}
	return 0;
}