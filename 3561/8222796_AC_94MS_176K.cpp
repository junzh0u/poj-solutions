#define _CRT_SECURE_NO_WARNINGS

#include<cstdio>

const int SIZE_MAX=10;
const char EMPTY='.';

int height,width;
char matrix[SIZE_MAX][SIZE_MAX+1];

void eraseLine(int row,int col){
	int rowShift,colShift;
	char line=matrix[row][col];
	switch(line){
	case '-':
		rowShift=0;
		colShift=1;
		break;
	case '|':
		rowShift=1;
		colShift=0;
		break;
	case '\\':
		rowShift=1;
		colShift=1;
		break;
	case '/':
		rowShift=1;
		colShift=-1;
		break;
	}
	while(row>=0 && row<height && col>=0 && col<width){
		if(matrix[row][col]!=line)
			return;
		matrix[row][col]=EMPTY;
		row+=rowShift;
		col+=colShift;
	}
}

bool exactOneLine(){
	bool lineDetected=false;
	for(int row=0;row<height;row++)
		for(int col=0;col<width;col++)
			if(matrix[row][col]!=EMPTY){
				if(lineDetected)
					return false;
				eraseLine(row,col);
				lineDetected=true;
			}
	return lineDetected;
}

int main(){
	int caseNum;
	scanf("%d",&caseNum);
	while(caseNum--){
		scanf("%d%d",&height,&width);
		for(int row=0;row<height;row++)
			scanf("%s",matrix[row]);
		printf(exactOneLine()?"CORRECT\n":"INCORRECT\n");
	}
	return 0;
}