#define _CRT_SECURE_NO_WARNINGS

#include<cstdio>
#include<climits>

void shiftRight(int *line,int size){
	int last=line[size-1];
	for(int col=size-1;col-1>=0;col--)
		line[col]=line[col-1];
	line[0]=last;
}

int value(int **matrix,int size){
	int sumMax=INT_MIN;
	for(int col=0;col<size;col++){
		int sum=0;
		for(int row=0;row<size;row++)
			sum+=matrix[row][col];
		if(sumMax<sum)
			sumMax=sum;
	}
#ifdef _DEBUG
	for(int row=0;row<size;row++){
		for(int col=0;col<size;col++)
			printf("%2d",matrix[row][col]);
		printf("\n");
	}
	printf("# %d\n",sumMax);
#endif
	return sumMax;
}

int solve(int **matrix,int size,int row){
	if(row==size)
		return value(matrix,size);
	int result=INT_MAX;
	for(int i=0;i<size;i++){
		shiftRight(matrix[row],size);
		int valueMin=solve(matrix,size,row+1);
		if(result>valueMin)
			result=valueMin;
	}
	return result;
}

int main(){
	while(true){
		int size;
		scanf("%d",&size);
		if(size<0)
			return 0;
		int **matrix=new int*[size];
		for(int row=0;row<size;row++)
			matrix[row]=new int[size];
		for(int row=0;row<size;row++)
			for(int col=0;col<size;col++)
				scanf("%d",matrix[row]+col);
		printf("%d\n",solve(matrix,size,1));
		for(int row=0;row<size;row++)
			delete[] matrix[row];
		delete[] matrix;
	}
}