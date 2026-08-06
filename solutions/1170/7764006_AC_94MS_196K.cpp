#define _CRT_SECURE_NO_WARNINGS
#define DEBUG

#include<cstdio>
#include<algorithm>

int pow(int base,int exp){
	int product=1;
	while(exp--)
		product*=base;
	return product;
}

const int PRODUCT_NUM_MAX=5,OFFER_NUM_MAX=99,LIST_NUMBER_MAX=5;
const int HASH_MAX=7776;	// pow((LIST_NUMBER_MAX+1),PRODUCT_NUM_MAX)

struct Product{
	int code,price;
};

int productNum=0;
Product products[PRODUCT_NUM_MAX];

int getProductIndexByCode(int code){
	for(int index=0;index<productNum;index++)
		if(products[index].code==code)
			return index;
	return -1;
}


class ShoppingList{
public:
	ShoppingList(){
		std::fill(number,number+productNum,0);
	}
	bool operator>(const ShoppingList &another){
		for(int productIndex=0;productIndex<productNum;productIndex++)
			if(number[productIndex]<another.number[productIndex])
				return false;
		return true;
	}
	ShoppingList substract(const ShoppingList &another){
		ShoppingList remain;
		for(int productIndex=0;productIndex<productNum;productIndex++)
			remain.number[productIndex]=number[productIndex]-another.number[productIndex];
		return remain;
	}
	int price(){
		int priceSum=0;
		for(int productIndex=0;productIndex<productNum;productIndex++)
			priceSum+=products[productIndex].price*number[productIndex];
		return priceSum;
	}
	int hash(){
		int result=0;
		for(int productIndex=0;productIndex<productNum;productIndex++)
			result+=number[productIndex]*pow(LIST_NUMBER_MAX+1,productIndex);
		return result;
	}
public:
	int number[PRODUCT_NUM_MAX];
};
struct Offer{
	ShoppingList listRequired;
	int price;
};

int offerNum;
Offer offers[OFFER_NUM_MAX];

int storedAnswer[HASH_MAX];
int lowestPrice(ShoppingList list){
	int hash=list.hash();
	if(storedAnswer[hash]!=-1)
		return storedAnswer[hash];
	int answer=list.price();
	for(int offerIndex=0;offerIndex<offerNum;offerIndex++){
		if(list>offers[offerIndex].listRequired){
			int price=lowestPrice(list.substract(offers[offerIndex].listRequired))+offers[offerIndex].price;
			if(answer>price)
				answer=price;
		}
	}
	return storedAnswer[hash]=answer;
}

int main(){
	scanf("%d",&productNum);
	ShoppingList list;
	for(int productIndex=0;productIndex<productNum;productIndex++)
		scanf("%d%d%d",&products[productIndex].code,list.number+productIndex,&products[productIndex].price);
	scanf("%d",&offerNum);
	for(int offerIndex=0;offerIndex<offerNum;offerIndex++){
		int productKind;
		scanf("%d",&productKind);
		while(productKind--){
			int productCode;
			scanf("%d",&productCode);
			int productIndex=getProductIndexByCode(productCode);
			scanf("%d",offers[offerIndex].listRequired.number+productIndex);
		}
		scanf("%d",&offers[offerIndex].price);
	}

	std::fill(storedAnswer,storedAnswer+HASH_MAX,-1);
	printf("%d\n",lowestPrice(list));
	return 0;
}