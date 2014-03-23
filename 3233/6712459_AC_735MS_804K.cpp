#include<cstdio>
#include<memory.h>

const int SIZE_LIMIT=30;
struct Matrix{
	int elements[SIZE_LIMIT][SIZE_LIMIT];
	Matrix(){
		memset(elements,0,sizeof(int)*SIZE_LIMIT*SIZE_LIMIT);
	}
	int* operator[](int row){
		return elements[row];
	}
};

int size,exponent,base;
Matrix matrix;

const int EXP_BITS_MAX=32;
Matrix powerBits[EXP_BITS_MAX];

Matrix mulitiply(Matrix left,Matrix right){
	Matrix product;
	for(int row=0;row<size;row++)
		for(int col=0;col<size;col++){
			product.elements[row][col]=0;
			for(int index=0;index<size;index++)
				product[row][col]=(product[row][col]+left[row][index]*right[index][col])%base;
		}
	return product;
}

Matrix add(Matrix left,Matrix right){
	Matrix sum;
	for(int row=0;row<size;row++)
		for(int col=0;col<size;col++)
			sum[row][col]=(left[row][col]+right[row][col])%base;
	return sum;
}

Matrix power(int exponent){
	Matrix result;
	for(int row=0;row<size;row++)
		result[row][row]=1;
	int bit=0;
	while(exponent){
		if(exponent&1)
			result=mulitiply(result,powerBits[bit]);
		exponent>>=1;
		bit++;
	}
	return result;
}

Matrix accumulate(int exponent){
	if(exponent==1)
		return matrix;
	if(exponent%2==1)
		return add(accumulate(exponent-1),power(exponent));
	Matrix half=accumulate(exponent/2);
	return add(half,mulitiply(half,power(exponent/2)));
}

int main(){
	scanf("%d%d%d",&size,&exponent,&base);
	for(int row=0;row<size;row++)
		for(int col=0;col<size;col++){
			scanf("%d",matrix[row]+col);
			matrix[row][col]%=base;
			powerBits[0][row][col]=matrix[row][col];
		}
	for(int index=1;index<EXP_BITS_MAX && (1<<index)<=exponent;index++)
		powerBits[index]=mulitiply(powerBits[index-1],powerBits[index-1]);

	Matrix result=accumulate(exponent);
	for(int row=0;row<size;row++){
		for(int col=0;col<size;col++)
			printf("%d ",result[row][col]);
		printf("\n");
	}
	return 0;
}