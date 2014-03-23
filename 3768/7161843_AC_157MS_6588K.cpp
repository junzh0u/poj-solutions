#define _CRT_SECURE_NO_WARNINGS
#include<cstdio>
#include<cmath>

const int ORIGINAL_SIZE_MAX=5;
const int FINAL_SIZE_MAX=3000;

int originalSize,level;
char original[ORIGINAL_SIZE_MAX][ORIGINAL_SIZE_MAX+1];
char final[FINAL_SIZE_MAX][FINAL_SIZE_MAX+1];

void expand(int rowBegin,int colBegin,int level){
	if(level==1){
		for(int row=0;row<originalSize;row++)
			for(int col=0;col<originalSize;col++)
				final[row+rowBegin][col+colBegin]=original[row][col];
		return;
	}
	int blockSize=(int)pow((double)originalSize,level-1);
	for(int row=0;row<originalSize;row++)
		for(int col=0;col<originalSize;col++)
			if(original[row][col]!=' ')
				expand(rowBegin+row*blockSize,colBegin+col*blockSize,level-1);
}

int main(){
	while(true){
		scanf("%d",&originalSize);
		if(originalSize==0)
			return 0;
		gets(original[0]);
		for(int row=0;row<originalSize;row++)
			gets(original[row]);
		scanf("%d",&level);
		int finalSize=(int)pow((double)originalSize,level);
		for(int row=0;row<finalSize;row++){
			for(int col=0;col<finalSize;col++)
				final[row][col]=' ';
			final[row][finalSize]='\0';
		}
		expand(0,0,level);
		for(int row=0;row<finalSize;row++)
			printf("%s\n",final[row]);
	}
}