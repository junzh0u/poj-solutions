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

int distance(const Coord& from,const Coord& to){
	if(from.col>to.col)
		return distance(to,from);
	if(from.row<to.row)
		return std::max(to.row-from.row,to.col-from.col);
	else
		return from.row-to.row+to.col-from.col;
}

int main(){
	initializeCoords();
	while(true){
		int from,to;
		scanf("%d%d",&from,&to);
		if(!from && !to)
			return 0;
		printf("The distance between cells %d and %d is %d.\n",from,to,distance(coords[from-1],coords[to-1]));
	}
}