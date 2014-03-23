#define _CRT_SECURE_NO_WARNINGS

#include<cstdio>

const int HEIGHT_MAX=100,WIDTH_MAX=100;

int boardHeight,boardWidth,brushHeight,brushWidth;
int painting[HEIGHT_MAX][WIDTH_MAX];
int board[HEIGHT_MAX][WIDTH_MAX];

void paint(int row,int col){
	for(int r=row;r<row+brushHeight;r++)
		for(int c=col;c<col+brushWidth;c++)
			board[r][c]=1-board[r][c];
}

int tryPaint(){
	int steps=0;
	for(int row=0;row<boardHeight;row++)
		for(int col=0;col<boardWidth;col++)
			if(board[row][col]!=painting[row][col]){
				if(row+brushHeight<=boardHeight && col+brushWidth<=boardWidth){
					paint(row,col);
					steps++;
				}
				else
					return -1;
			}
	return steps;
}

int main(){
	while(true){
		scanf("%d%d%d%d",&boardHeight,&boardWidth,&brushHeight,&brushWidth);
		if(!boardHeight && !boardWidth && !brushHeight && !brushWidth)
			return 0;
		for(int row=0;row<boardHeight;row++){
			char line[WIDTH_MAX+1];
			scanf("%s",line);
			for(int col=0;col<boardWidth;col++){
				painting[row][col]=line[col]-'0';
				board[row][col]=0;
			}
		}
		printf("%d\n",tryPaint());
	}
}