#define _CRT_SECURE_NO_WARNINGS

#include<cstdio>
#include<climits>

const int SIZE_LIMIT=250;

int main(){
	int size,subSize,queryNum;
	scanf("%d%d%d",&size,&subSize,&queryNum);
	int elevation[SIZE_LIMIT][SIZE_LIMIT];
	for(int row=0;row<size;row++)
		for(int col=0;col<size;col++)
			scanf("%d",elevation[row]+col);
	while(queryNum--){
		int rowBegin,colBegin;
		scanf("%d%d",&rowBegin,&colBegin);
		rowBegin--;
		colBegin--;
		int max=0,min=INT_MAX;
		for(int row=rowBegin;row<rowBegin+subSize;row++)
			for(int col=colBegin;col<colBegin+subSize;col++){
				if(max<elevation[row][col])
					max=elevation[row][col];
				if(min>elevation[row][col])
					min=elevation[row][col];
			}
		printf("%d\n",max-min);
	}
	return 0;
}