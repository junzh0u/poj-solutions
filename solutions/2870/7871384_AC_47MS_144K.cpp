#define _CRT_SECURE_NO_WARNINGS

#include<cstdio>
#include<algorithm>

const int HEIGHT_MAX=7,WIDTH_MAX=7;

int height,width,barrierNum;
bool barrier[HEIGHT_MAX][WIDTH_MAX];
int lighten[HEIGHT_MAX][WIDTH_MAX];
int lampAllow[HEIGHT_MAX][WIDTH_MAX];
int lampAround[HEIGHT_MAX][WIDTH_MAX];
int lampNeed;

void lampStatusChange(int row,int col,int change){
	lighten[row][col]+=change;

	for(int r=row-1;r>=0 && !barrier[r][col];r--)
		lighten[r][col]+=change;
	for(int r=row+1;r<height && !barrier[r][col];r++)
		lighten[r][col]+=change;

	for(int c=col-1;c>=0 && !barrier[row][c];c--)
		lighten[row][c]+=change;
	for(int c=col+1;c<width && !barrier[row][c];c++)
		lighten[row][c]+=change;

	if(row-1>=0)
		lampAround[row-1][col]+=change;
	if(row+1<height)
		lampAround[row+1][col]+=change;
	if(col-1>=0)
		lampAround[row][col-1]+=change;
	if(col+1<width)
		lampAround[row][col+1]+=change;
}

void lampOff(int row,int col){
	lampStatusChange(row,col,-1);
}

void lampOn(int row,int col){
	lampStatusChange(row,col,1);
}

bool lampPlacable(int row,int col){
	if(barrier[row][col] || lighten[row][col])
		return false;
	if(row-1>=0 && lampAround[row-1][col]>=lampAllow[row-1][col])
		return false;
	if(row+1<height && lampAround[row+1][col]>=lampAllow[row+1][col])
		return false;
	if(col-1>=0 && lampAround[row][col-1]>=lampAllow[row][col-1])
		return false;
	if(col+1<width && lampAround[row][col+1]>=lampAllow[row][col+1])
		return false;
	return true;
}

bool succeed(){
	for(int row=0;row<height;row++)
		for(int col=0;col<width;col++){
			if(barrier[row][col]){
				if(lampAllow[row][col]!=INT_MAX && lampAllow[row][col]!=lampAround[row][col])
					return false;
			}
			else if(!lighten[row][col])
				return false;
		}
		return true;
}

void trySolution(int row,int col,int lampPlaced){
	if(row>=height){
		if(succeed() && lampNeed>lampPlaced)
			lampNeed=lampPlaced;
		return;
	}
	if(col>=width)
		return trySolution(row+1,0,lampPlaced);
	if(barrier[row][col])
		for(int r=row-1;r>=0;r--)
			if(!barrier[r][col] && !lighten[r][col])
				return;
	trySolution(row,col+1,lampPlaced);
	if(lampPlacable(row,col)){
		lampOn(row,col);
		trySolution(row,col+1,lampPlaced+1);
		lampOff(row,col);
	}
}

int main(){
	while(true){
		scanf("%d%d",&height,&width);
		if(!height && !width)
			return 0;
		for(int row=0;row<height;row++){
			for(int col=0;col<width;col++){
				barrier[row][col]=false;
				lighten[row][col]=0;
				lampAllow[row][col]=INT_MAX;
				lampAround[row][col]=0;
			}
		}
		scanf("%d",&barrierNum);
		while(barrierNum--){
			int row,col,number;
			scanf("%d%d%d",&row,&col,&number);
			row--;col--;
			barrier[row][col]=true;
			if(number!=-1)
				lampAllow[row][col]=number;
		}
		lampNeed=INT_MAX;
		trySolution(0,0,0);
		if(lampNeed==INT_MAX)
			printf("No solution\n");
		else
			printf("%d\n",lampNeed);
	}
}