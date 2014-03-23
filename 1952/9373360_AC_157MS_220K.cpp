#define _CRT_SECURE_NO_WARNINGS
#include<cstdio>
#include<algorithm>
#include<set>

const int PRICE_NUM_MAX=5000;
int priceNum,prices[PRICE_NUM_MAX+1],length[PRICE_NUM_MAX+1],count[PRICE_NUM_MAX+1];

int main(){
	scanf("%d",&priceNum);
	for(int i=0;i<priceNum;i++)
		scanf("%d",prices+i);
	prices[priceNum++]=0;
	std::fill(length,length+priceNum,1);
	std::fill(count,count+priceNum,1);
	for(int last=1;last<priceNum;last++){
		for(int i=0;i<last;i++)
			if(prices[i]>prices[last] && length[last]<length[i]+1)
				length[last]=length[i]+1;
		if(length[last]==1)
			continue;
		count[last]=0;
		std::set<int> visited;
		for(int i=last-1;i>=0;i--)
			if(prices[i]>prices[last] && length[last]==length[i]+1 && visited.find(prices[i])==visited.end()){
				count[last]+=count[i];
				visited.insert(prices[i]);
			}
	}
	printf("%d %d\n",length[priceNum-1]-1,count[priceNum-1]);
	return 0;
}