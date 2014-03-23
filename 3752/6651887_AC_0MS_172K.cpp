#include<cstdio>

const int ROW_STEP[4]={0,1,0,-1},COL_STEP[4]={1,0,-1,0};

int main(){
	int height,width;
	scanf("%d%d",&height,&width);
	char **matrix=new char*[height];
	for(int row=0;row<height;row++){
		matrix[row]=new char[width];
		for(int col=0;col<width;col++)
			matrix[row][col]='\0';
	}
	int row=0,col=0,count=0,direction=0,fill=0;
	while(count<height*width){
		matrix[row][col]='A'+fill;
		fill=(fill+1)%26;
		int rowNext=row+ROW_STEP[direction],colNext=col+COL_STEP[direction];
		if(rowNext<0 || rowNext>=height || colNext<0 || colNext>=width || matrix[rowNext][colNext]!='\0'){
			direction=(direction+1)%4;
			rowNext=row+ROW_STEP[direction];
			colNext=col+COL_STEP[direction];
		}
		row=rowNext;
		col=colNext;
		count++;
	}
	for(int row=0;row<height;row++){
		for(int col=0;col<width;col++)
			printf("   %c",matrix[row][col]);
		printf("\n");
	}
	for(int row=0;row<height;row++)
		delete[] matrix[row];
	delete[] matrix;
	return 0;
}