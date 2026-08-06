#include<cstdio>

const int HEIGHT_MAX=100,WIDTH_MAX=100;

bool flooded[HEIGHT_MAX][WIDTH_MAX]={false};
bool visited[HEIGHT_MAX][WIDTH_MAX]={false};
int height,width;

int exploreLake(int row,int col){
	if(row<0 || row>=HEIGHT_MAX || col<0 || col>=WIDTH_MAX)
		return 0;
	if(!flooded[row][col] || visited[row][col])
		return 0;
	int size=1;
	visited[row][col]=true;
	size+=exploreLake(row-1,col);
	size+=exploreLake(row+1,col);
	size+=exploreLake(row,col-1);
	size+=exploreLake(row,col+1);
	return size;
}

int main(){
	int floodedCellNum;
	scanf("%d%d%d",&height,&width,&floodedCellNum);
	for(int i=0;i<floodedCellNum;i++){
		int row,col;
		scanf("%d%d",&row,&col);
		flooded[row-1][col-1]=true;
	}
	int lakeSizeMax=0;
	for(int row=0;row<height;row++)
		for(int col=0;col<width;col++)
			if(flooded[row][col] && !visited[row][col]){
				int lakeSize=exploreLake(row,col);
				if(lakeSizeMax<lakeSize)
					lakeSizeMax=lakeSize;
			}
	printf("%d\n",lakeSizeMax);
	return 0;
}