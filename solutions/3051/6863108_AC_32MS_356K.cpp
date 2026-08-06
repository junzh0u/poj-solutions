#include<cstdio>
#include<algorithm>

//#define DEBUG

int width,height;
char **map;
bool **visited;

const char PASTURE='*';
const int DIRECT_NUM=4;
const int ROW_STEP[DIRECT_NUM]={-1,	0,	1,	0};
const int COL_STEP[DIRECT_NUM]={0,	1,	0,	-1};

int visit(int row,int col){
	if(row<0 || row>=height || col<0 || col>=width)
		return 0;
	if(map[row][col]!=PASTURE || visited[row][col])
		return 0;
	visited[row][col]=true;
	int count=1;
	for(int direct=0;direct<DIRECT_NUM;direct++)
		count+=visit(row+ROW_STEP[direct],col+COL_STEP[direct]);
	return count;
}

#ifdef DEBUG
void state(){
	printf("----------\n");
	for(int row=0;row<height;row++){
		for(int col=0;col<width;col++){
			if(map[row][col]==PASTURE)
				printf("%c",visited[row][col]?'#':PASTURE);
			else
				printf("%c",map[row][col]);
		}
		printf("\n");
	}
}
#endif

int main(){
	scanf("%d%d",&width,&height);

	map=new char*[height];
	visited=new bool*[height];
	for(int row=0;row<height;row++){
		map[row]=new char[width+1];
		scanf("%s",map[row]);
		visited[row]=new bool[width];
		std::fill(visited[row],visited[row]+width,false);
	}

#ifdef DEBUG
	state();
#endif

	int sizeMax=0;
	for(int row=0;row<height;row++)
		for(int col=0;col<width;col++){
			int size=visit(row,col);
#ifdef DEBUG
			if(size>0){
				printf("%d\n",size);
				state();
			}
#endif
			if(sizeMax<size)
				sizeMax=size;
		}
	printf("%d\n",sizeMax);

	for(int row=0;row<height;row++)
		delete[] map[row];
	delete[] map;
	return 0;
}