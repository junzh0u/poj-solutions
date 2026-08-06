#include<cstdio>
#include<algorithm>
using namespace std;

const int COIN_CLASS_NUM_MAX=100,PRICE_RANGE_MAX=100000;

short remain[COIN_CLASS_NUM_MAX][PRICE_RANGE_MAX+1];
bool pay[PRICE_RANGE_MAX+1];

struct Coin{
	int value,num;
};
Coin coins[COIN_CLASS_NUM_MAX];

bool CoinCompare(const Coin& left,const Coin& right){
	return left.value*left.num<right.value*right.num;
}

int main(){
	while(true){
		int coinClassNum,priceRange;
		scanf("%d%d",&coinClassNum,&priceRange);
		if(coinClassNum==0 && priceRange==0)
			break;
		for(int i=0;i<coinClassNum;i++)
			scanf("%d",&coins[i].value);
		for(int i=0;i<coinClassNum;i++)
			scanf("%d",&coins[i].num);
		sort(coins,coins+coinClassNum,CoinCompare);
		memset(remain,-1,sizeof(short)*coinClassNum*(PRICE_RANGE_MAX+1));
		memset(pay,false,sizeof(bool)*(priceRange+1));
		int priceMax=0,count=0;
		for(int i=0;i<coinClassNum;i++){
			remain[i][0]=coins[i].num;
			int limit=min(priceMax+coins[i].value*coins[i].num,priceRange);
			for(int price=1;price<=limit;price++){
				if(pay[price])
					remain[i][price]=coins[i].num;
				else if(price>=coins[i].value && remain[i][price-coins[i].value]>0){
					remain[i][price]=remain[i][price-coins[i].value]-1;
					pay[price]=true;
					count++;
					if(priceMax<price)
						priceMax=price;
				}
			}
		}
		printf("%d\n",count);
	}
	return 0;
}