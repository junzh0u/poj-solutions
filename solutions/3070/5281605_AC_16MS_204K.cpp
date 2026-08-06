#include<cstdio>
#include<cmath>
using namespace std;

struct Matrix2{
	int content[2][2];
};

Matrix2 MultiplyModulus10000(const Matrix2& left,const Matrix2& right){
	Matrix2 product={0,0,0,0};
	for(int row=0;row<2;row++)
		for(int col=0;col<2;col++)
			for(int i=0;i<2;i++)
				product.content[row][col]=(product.content[row][col]+left.content[row][i]*right.content[i][col])%10000;
	return product;
}

Matrix2 PowerModulus10000(const Matrix2 base,const int exponent){
	if(exponent==0){
		const Matrix2 POWER_ZERO={{1,0,0,1}};
		return POWER_ZERO;
	}
	Matrix2 power=PowerModulus10000(base,exponent/2);
	power=MultiplyModulus10000(power,power);
	if(exponent%2==1)
		power=MultiplyModulus10000(power,base);
	return power;
}

int main(){
	int n;
	const Matrix2 BASE={{1,1,1,0}};
	while(true){
		scanf("%d",&n);
		if(n==-1)
			break;
		printf("%d\n",PowerModulus10000(BASE,n).content[0][1]);
	}
}