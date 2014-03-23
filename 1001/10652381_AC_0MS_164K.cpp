#define _CRT_SECURE_NO_WARNINGS

#include<cstdio>
#include<algorithm>

const int BASE_LEN=6;
const int EXPONENT_MAX=25;
const int POWER_LEN_MAX=200;

class Decimal{
public:
	int digits[POWER_LEN_MAX];
	int length;
	int scale;
	Decimal(){
		std::fill(digits,digits+POWER_LEN_MAX,0);
		length=1;
		scale=0;
	}
	Decimal(char *str){
		std::fill(digits,digits+POWER_LEN_MAX,0);
		length=0;
		for(int i=strlen(str)-1;i>=0;i--){
			if(str[i]=='.')
				scale=length;
			else
				digits[length++]=str[i]-'0';
		}
	}
	Decimal multiply(const Decimal& another){
		Decimal product;
		product.length=length+another.length-1;
		product.scale=scale+another.scale;
		for(int i=0;i<length;i++)
			for(int j=0;j<another.length;j++)
				product.digits[i+j]+=digits[i]*another.digits[j];
		product.normalize();
		return product;
	}
	void normalize(){
		for(int i=0;i<length;i++){
			if(digits[i]>=10){
				digits[i+1]+=digits[i]/10;
				digits[i]%=10;
			}
		}
		if(digits[length])
			length++;
	}
	void print(){
		int first=0;
		while(first<scale && !digits[first])
			first++;
		int last=length-1;
		while(last>=scale && !digits[last])
			last--;
		for(int i=last;i>=first;i--){
			if(i==scale-1)
				printf(".");
			printf("%d",digits[i]);
		}
		printf("\n");
	}
};

int main(){
	char baseStr[BASE_LEN+1];
	int exponent;
	while(scanf("%s%d",baseStr,&exponent)!=EOF){
		Decimal base(baseStr);
		Decimal power(base);
		while(exponent-->1)
			power=power.multiply(base);
		power.print();
		exponent=0;
	}
	return 0;
}