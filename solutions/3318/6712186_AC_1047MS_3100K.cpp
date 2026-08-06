#include<cstdio>
#include<algorithm>

const int MATRIX_SIZE_MAX=500,CHECK_TIMES=1;
int size;
int a[MATRIX_SIZE_MAX][MATRIX_SIZE_MAX],b[MATRIX_SIZE_MAX][MATRIX_SIZE_MAX],c[MATRIX_SIZE_MAX][MATRIX_SIZE_MAX];
int factor[MATRIX_SIZE_MAX],resultLeft[MATRIX_SIZE_MAX],resultRight[MATRIX_SIZE_MAX];
int temp[MATRIX_SIZE_MAX];

void read(int matrix[MATRIX_SIZE_MAX][MATRIX_SIZE_MAX]){
	for(int row=0;row<size;row++)
		for(int col=0;col<size;col++)
			scanf("%d",matrix[row]+col);
}

void multiply(int left[MATRIX_SIZE_MAX],int right[MATRIX_SIZE_MAX][MATRIX_SIZE_MAX],int product[MATRIX_SIZE_MAX]){
	for(int col=0;col<size;col++){
		product[col]=0;
		for(int index=0;index<size;index++)
			product[col]+=left[index]*right[index][col];
	}
}

bool equals(int left[MATRIX_SIZE_MAX],int right[MATRIX_SIZE_MAX]){
	for(int col=0;col<size;col++)
		if(left[col]!=right[col])
			return false;
	return true;
}

int main(){
	scanf("%d",&size);
	read(a);
	read(b);
	read(c);
	for(int col=0;col<size;col++)
		factor[col]=col+1;
	bool equal=true;
	for(int checkID=0;checkID<CHECK_TIMES;checkID++){
		std::random_shuffle(factor,factor+size);
		multiply(factor,a,temp);
		multiply(temp,b,resultLeft);
		multiply(factor,c,resultRight);
		if(!equals(resultLeft,resultRight)){
			equal=false;
			break;
		}
	}
	printf(equal?"YES\n":"NO\n");
	return 0;
}