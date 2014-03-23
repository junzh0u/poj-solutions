#include<cstdio>
#include<algorithm>
using namespace std;

//#define DEBUG

void analyzePossibility(bool possible[10],const int row,const int col,const int board[9][9]){
	fill(possible,possible+10,true);
	for(int i=0;i<9;i++)
		possible[board[row][i]]=possible[board[i][col]]=false;
	int smallRow=row/3,smallCol=col/3;
	for(int i=smallRow*3;i<smallRow*3+3;i++)
		for(int j=smallCol*3;j<smallCol*3+3;j++)
			possible[board[i][j]]=false;
}

bool fillBoard(int board[9][9],const int position){
	if(position>=9*9)
		return true;
	int row=position/9,col=position%9;
	if(board[row][col]!=0)
		return fillBoard(board,position+1);
	bool possible[10];
	analyzePossibility(possible,row,col,board);
	for(int i=1;i<=9;i++)
		if(possible[i]){
			board[row][col]=i;
			if(fillBoard(board,position+1))
				return true;
		}
	board[row][col]=0;
	return false;
}

int main(){
	int caseNum;
	scanf("%d",&caseNum);
	while(caseNum-->0){
		int board[9][9];
		for(int row=0;row<9;row++){
			getchar();
			for(int col=0;col<9;col++)
				board[row][col]=getchar()-'0';
		}
#ifdef DEBUG
		for(int row=0;row<9;row++){
			for(int col=0;col<9;col++)
				putchar(board[row][col]+'0');
			putchar('\n');
		}
#endif
		fillBoard(board,0);
		for(int row=0;row<9;row++){
			for(int col=0;col<9;col++)
				putchar(board[row][col]+'0');
			putchar('\n');
		}
	}
	return 0;
}