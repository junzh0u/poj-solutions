#include<cstdio>
#include<algorithm>
#include<functional>

int main(){
	int caseNum;
	scanf("%d",&caseNum);
	while(caseNum-->0){
		int itemNum;
		scanf("%d",&itemNum);
		int *price=new int[itemNum];
		for(int i=0;i<itemNum;i++)
			scanf("%d",price+i);
		std::sort(price,price+itemNum,std::greater<int>());
		int discount=0;
		for(int i=2;i<itemNum;i+=3)
			discount+=price[i];
		printf("%d\n",discount);
		delete[] price;
	}
	return 0;
}