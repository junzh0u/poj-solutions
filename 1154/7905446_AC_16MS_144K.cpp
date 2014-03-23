#define _CRT_SECURE_NO_WARNINGS

#include<cstdio>
#include<algorithm>

const int HEIGHT_MAX=20,WIDTH_MAX=20;

int height,width,depthMax=0;
char map[HEIGHT_MAX][WIDTH_MAX+1];
bool visited[26];

void visit(int row,int col,int depth){
	if(visited[map[row][col]-'A'])
		return;
	visited[map[row][col]-'A']=true;
	if(row-1>=0)
		visit(row-1,col,depth+1);
	if(row+1<height)
		visit(row+1,col,depth+1);
	if(col-1>=0)
		visit(row,col-1,depth+1);
	if(col+1<width)
		visit(row,col+1,depth+1);
	if(depthMax<depth)
		depthMax=depth;
	visited[map[row][col]-'A']=false;
}

int main(){
	scanf("%d%d",&height,&width);
	for(int row=0;row<height;row++)
		scanf("%s",map[row]);
	std::fill(visited,visited+26,false);
	visit(0,0,1);
	printf("%d\n",depthMax);
	return 0;
}