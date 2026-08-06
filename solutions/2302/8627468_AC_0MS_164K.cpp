#define _CRT_SECURE_NO_WARNINGS

#include<cstdio>

int card[5][5];
bool marked[5][5];
int numbers[75];

bool bingoRow(int row){
	for(int col=0;col<5;col++)
		if(!marked[row][col])
			return false;
	return true;
}

bool bingoCol(int col){
	for(int row=0;row<5;row++)
		if(!marked[row][col])
			return false;
	return true;
}

bool bingoDiagonal(int row,int col){
	if(row==col){
		for(int i=0;i<5;i++)
			if(!marked[i][i])
				return false;
		return true;
	}
	if(row+col==4){
		for(int i=0;i<5;i++)
			if(!marked[i][4-i])
				return false;
		return true;
	}
	return false;
}

int main(){
	int gameNum;
	scanf("%d",&gameNum);
	while(gameNum--){
		for(int row=0;row<5;row++){
			for(int col=0;col<5;col++){
				if(row==2 && col==2){
					marked[row][col]=true;
					continue;
				}
				scanf("%d",card[row]+col);
				marked[row][col]=false;
			}
		}
		for(int i=0;i<75;i++)
			scanf("%d",numbers+i);
		for(int step=1;step<=75;step++){
			int number=numbers[step-1];
			bool win=false,found=false;
			for(int row=0;row<5 && !found;row++){
				for(int col=0;col<5 && !found;col++){
					if(card[row][col]==number){
						found=true;
						marked[row][col]=true;
						win=bingoRow(row) || bingoCol(col) || bingoDiagonal(row,col);
					}
				}
			}
			if(win){
				printf("BINGO after %d numbers announced\n",step);
				break;
			}
		}
	}
	return 0;
}