#define _CRT_SECURE_NO_WARNINGS
#include<cstdio>
#include<algorithm>
#include<functional>

const int ITEM_NUM_MAX=100000;
int itemNum,binLen,itemLen[ITEM_NUM_MAX];

int main(){
	scanf("%d%d",&itemNum,&binLen);
	for(int i=0;i<itemNum;i++)
		scanf("%d",itemLen+i);
	std::sort(itemLen,itemLen+itemNum);
	int first=0,last=itemNum-1,binNum=0;
	while(first<last){
		binNum++;
		if(itemLen[first]+itemLen[last]<=binLen)
			first++;
		last--;
	}
	if(first==last)
		binNum++;
	printf("%d\n",binNum);
	return 0;
}