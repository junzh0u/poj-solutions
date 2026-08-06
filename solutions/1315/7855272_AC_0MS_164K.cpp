#define _CRT_SECURE_NO_WARNINGS

#include<cstdio>
#include<algorithm>

const int BOARD_SIZE_MAX=4;
const char	EMPTY='.',WALL='X';

int		size;
char	board[BOARD_SIZE_MAX][BOARD_SIZE_MAX+1];
int		illegal[BOARD_SIZE_MAX][BOARD_SIZE_MAX+1];

void	setIllegal(int row,int col,int shift){
	for(int r=row;r<size && board[r][col]!=WALL;r++)
		illegal[r][col]+=shift;
	for(int c=col;c<size && board[row][c]!=WALL;c++)
		illegal[row][c]+=shift;
}

int legalCount(int row,int col){
	if(col>=size)
		return legalCount(row+1,0);
	if(row>=size)
		return 0;
	int max=legalCount(row,col+1);
	if(board[row][col]!=WALL && illegal[row][col]==0){
		setIllegal(row,col,1);
		max=std::max(max,legalCount(row,col+1)+1);
		setIllegal(row,col,-1);
	}
	return max;
}

int main(){
	while(true){
		scanf("%d",&size);
		if(size==0)
			return 0;
		for(int row=0;row<size;row++){
			scanf("%s",board+row);
			std::fill(illegal[row],illegal[row]+size,0);
		}
		printf("%d\n",legalCount(0,0));
	}
}