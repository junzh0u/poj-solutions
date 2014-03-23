#define _CRT_SECURE_NO_WARNINGS
#include<cstdio>
#include<algorithm>
#include<cmath>

struct Fraction{
	int numerator,denominator;
};

const int UNIT_NUM_MAX=7;

Fraction sum;
int productMax,unitNumMax,count;

int gcd(int greater,int less){
	if(greater<less)
		return gcd(less,greater);
	if(less==0)
		return greater;
	if(greater%less==0)
		return less;
	return gcd(less,greater%less);
}

Fraction minus(Fraction minuend,Fraction subtrahend){
	Fraction difference;
	difference.denominator=minuend.denominator*subtrahend.denominator;
	difference.numerator=minuend.numerator*subtrahend.denominator-subtrahend.numerator*minuend.denominator;
	int divisor=gcd(difference.denominator,abs(difference.numerator));
	difference.denominator/=divisor;
	difference.numerator/=divisor;
	return difference;
}

Fraction parts[UNIT_NUM_MAX];

void tryPartition(Fraction sum,int partNum,int denominatorMin,int denominatorMax){
	if(sum.numerator==0){
		count++;
#ifdef _DEBUG
		for(int i=0;i<partNum;i++){
			if(i!=0)
				printf("+");
			printf("1/%d",parts[i].denominator);
		}
		printf("\n");
#endif
		return;
	}
	if(partNum>=unitNumMax)
		return;
	for(parts[partNum].denominator=std::max(denominatorMin,(sum.denominator+sum.numerator-1)/sum.numerator);
		parts[partNum].denominator<=std::min(denominatorMax,sum.denominator*unitNumMax/sum.numerator);
		parts[partNum].denominator++){
			parts[partNum].numerator=1;
			Fraction difference=minus(sum,parts[partNum]);
			if(difference.numerator<0)
				break;
			tryPartition(difference,partNum+1,parts[partNum].denominator,denominatorMax/parts[partNum].denominator);
	}
}

int main(){
	while(true){
		scanf("%d%d%d%d",&sum.numerator,&sum.denominator,&productMax,&unitNumMax);
		if(!sum.numerator && !sum.denominator && !productMax && !unitNumMax)
			return 0;
		count=0;
		tryPartition(sum,0,1,productMax);
		printf("%d\n",count);
	}
}