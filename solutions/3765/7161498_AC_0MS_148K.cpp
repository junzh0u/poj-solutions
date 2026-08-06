#define _CRT_SECURE_NO_WARNINGS

#include<cstdio>
#include<cstring>

const int HEIGHT=23,WIDTH=19;
const char BOARD[HEIGHT][WIDTH+1]={
"         _",
"       _/ \\_",
"     _/ \\_/ \\_",
"   _/ \\_/ \\_/ \\_",
" _/ \\_/ \\_/ \\_/ \\_",
"/ \\_/ \\_/ \\_/ \\_/ \\",
"\\_/ \\_/ \\_/ \\_/ \\_/",
"/ \\_/ \\_/ \\_/ \\_/ \\",
"\\_/ \\_/ \\_/ \\_/ \\_/",
"/ \\_/ \\_/ \\_/ \\_/ \\",
"\\_/ \\_/ \\_/ \\_/ \\_/",
"/ \\_/ \\_/ \\_/ \\_/ \\",
"\\_/ \\_/ \\_/ \\_/ \\_/",
"/ \\_/ \\_/ \\_/ \\_/ \\",
"\\_/ \\_/ \\_/ \\_/ \\_/",
"/ \\_/ \\_/ \\_/ \\_/ \\",
"\\_/ \\_/ \\_/ \\_/ \\_/",
"/ \\_/ \\_/ \\_/ \\_/ \\",
"\\_/ \\_/ \\_/ \\_/ \\_/",
"  \\_/ \\_/ \\_/ \\_/",
"    \\_/ \\_/ \\_/",
"      \\_/ \\_/",
"        \\_/"
};

int main(){
	while(true){
		int pieceNum;
		scanf("%d\n",&pieceNum);
		if(pieceNum==0)
			return 0;
		char boardWithPieces[HEIGHT][WIDTH+1];
		for(int row=0;row<HEIGHT;row++)
			strcpy(boardWithPieces[row],BOARD[row]);
		for(int pieceIndex=0;pieceIndex<pieceNum;pieceIndex++){
			char col,type;
			int row;
			scanf("%c%d %c\n",&col,&row,&type);
			col-='A';
			row=((col<=4)?(17+col):(25-col))-2*row;
			col=2*col+1;
			boardWithPieces[row][col]=type;
		}
		for(int row=0;row<HEIGHT;row++)
			printf("%s\n",boardWithPieces[row]);
	}
}