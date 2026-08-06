#include<cstdio>
#include<string>
using namespace std;

const char BOARD='#',BLANK='.';

int PlanCount(int boardSize,char **board,bool *colAllow,int rowBegin,int pieceRemain){
	if(pieceRemain==0)
		return 1;
	if(rowBegin+pieceRemain>boardSize)
		return 0;
	if(rowBegin>=boardSize)
		return 0;
	int count=PlanCount(boardSize,board,colAllow,rowBegin+1,pieceRemain);
	for(int col=0;col<boardSize;col++)
		if(board[rowBegin][col]==BOARD && colAllow[col]){
			colAllow[col]=false;
			count+=PlanCount(boardSize,board,colAllow,rowBegin+1,pieceRemain-1);
			colAllow[col]=true;
		}
	return count;
}

int main(){
	while(true){
		int boardSize,pieceNum;
		scanf("%d%d",&boardSize,&pieceNum);
		if(boardSize==-1 && pieceNum==-1)
			break;
		char **board=new char*[boardSize];
		for(int row=0;row<boardSize;row++){
			board[row]=new char[boardSize+1];
			scanf("%s",board[row]);
		}
		bool *colAllow=new bool[boardSize];
		for(int col=0;col<boardSize;col++)
			colAllow[col]=true;
		printf("%d\n",PlanCount(boardSize,board,colAllow,0,pieceNum));
		delete[] colAllow;
		for(int row=0;row<boardSize;row++)
			delete[] board[row];
		delete[] board;
	}
	return 0;
}