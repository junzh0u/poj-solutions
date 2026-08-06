#define _CRT_SECURE_NO_WARNINGS
#include<cstdio>

enum Status{
	OUT,EMPTY,TAKEN
};

const int NONE=-1;
const int HEIGHT=7,WIDTH=7,POS_NUM=33;
const int ROWS[POS_NUM]={0,0,0,1,1,1,2,2,2,2,2,2,2,3,3,3,3,3,3,3,4,4,4,4,4,4,4,5,5,5,6,6,6};
const int COLS[POS_NUM]={2,3,4,2,3,4,0,1,2,3,4,5,6,0,1,2,3,4,5,6,0,1,2,3,4,5,6,2,3,4,2,3,4};
int POS_BOARD[HEIGHT][WIDTH];
const int DIR_NUM=4;
const int ROW_SHIFT[DIR_NUM]={-1,0,1,0};
const int COL_SHIFT[DIR_NUM]={0,1,0,-1};

#ifdef _DEBUG
void printBoard(Status board[HEIGHT][WIDTH]){
	for(int row=0;row<HEIGHT;row++){
		for(int col=0;col<WIDTH;col++){
			switch(board[row][col]){
			case OUT:
				printf(" ");break;
			case EMPTY:
				printf("O");break;
			case TAKEN:
				printf("X");break;
			}
		}
		printf("\n");
	}
}
#endif

Status& status(Status board[HEIGHT][WIDTH],int pos){
	return board[ROWS[pos]][COLS[pos]];
}

bool inBoard(int row,int col){
	return row>=0 && row<HEIGHT && col>=0 && col<WIDTH;
}

int main(){
	printf("HI Q OUTPUT\n");
	for(int pos=0;pos<POS_NUM;pos++)
		POS_BOARD[ROWS[pos]][COLS[pos]]=pos;
	int caseNum;
	scanf("%d",&caseNum);
	while(caseNum--){
		Status board[HEIGHT][WIDTH];
		for(int row=0;row<HEIGHT;row++)
			for(int col=0;col<WIDTH;col++)
				board[row][col]=OUT;
		for(int pos=0;pos<POS_NUM;pos++)
			status(board,pos)=EMPTY;
		while(true){
			int pos;
			scanf("%d",&pos);
			if(!pos)
				break;
			status(board,pos-1)=TAKEN;
		}
		while(true){
			bool anyChange=false;
			for(int pos=POS_NUM-1;pos>=0;pos--){
				if(status(board,pos)!=EMPTY)
					continue;
				int row=ROWS[pos],col=COLS[pos];
				int posJumpFrom=NONE,posJumpMid=NONE;
				for(int dir=0;dir<DIR_NUM;dir++){
					int rowFrom=row+2*ROW_SHIFT[dir],colFrom=col+2*COL_SHIFT[dir];
					int rowMid=row+ROW_SHIFT[dir],colMid=col+COL_SHIFT[dir];
					if(inBoard(rowFrom,colFrom) && board[rowFrom][colFrom]==TAKEN && board[rowMid][colMid]==TAKEN){
						int posFrom=POS_BOARD[rowFrom][colFrom];
						if(posJumpFrom==NONE || posJumpFrom<posFrom){
							posJumpFrom=posFrom;
							posJumpMid=POS_BOARD[rowMid][colMid];
						}
					}
				}
				if(posJumpFrom!=NONE){
					status(board,posJumpFrom)=EMPTY;
					status(board,posJumpMid)=EMPTY;
					status(board,pos)=TAKEN;
					anyChange=true;
					break;
				}
			}
			if(!anyChange)
				break;
		}
		int result=0;
		for(int pos=0;pos<POS_NUM;pos++)
			if(status(board,pos)==TAKEN)
				result+=pos+1;
		printf("%d\n",result);
	}
	printf("END OF OUTPUT\n");
	return 0;
}