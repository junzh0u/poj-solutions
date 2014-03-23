#define _CRT_SECURE_NO_WARNINGS

#include<cstdio>
#include<algorithm>
#include<set>

const int WORKYARD_NUM_MAX=50,BOX_NUM_MAX=20;

int main(){
	int caseID=0;

	while(true){
		caseID++;
		
		int workyardNum;
		scanf("%d",&workyardNum);
		if(workyardNum==0)
			return 0;
		
		int profitSum=0;
		std::set<int> prulNumToBuy;
		prulNumToBuy.insert(0);

		for(int workyardIndex=0;workyardIndex<workyardNum;workyardIndex++){
			int boxNum;
			scanf("%d",&boxNum);

			int prices[BOX_NUM_MAX];
			for(int boxIndex=0;boxIndex<boxNum;boxIndex++)
				scanf("%d",prices+boxIndex);

			int profits[BOX_NUM_MAX+1]={0};
			for(int boxIndex=0;boxIndex<boxNum;boxIndex++)
				profits[boxIndex+1]=profits[boxIndex]+10-prices[boxIndex];

			int profitMax=0;
			for(int buyNum=0;buyNum<=boxNum;buyNum++)
				if(profitMax<profits[buyNum])
					profitMax=profits[buyNum];

			profitSum+=profitMax;

			std::set<int> result;
			for(int buyNum=0;buyNum<=boxNum;buyNum++)
				if(profitMax==profits[buyNum])
					for(std::set<int>::iterator iter=prulNumToBuy.begin();iter!=prulNumToBuy.end();iter++)
						result.insert(*iter+buyNum);
			prulNumToBuy=result;
		}

		printf("Workyards %d\n",caseID);
		printf("Maximum profit is %d.\n",profitSum);
		printf("Number of pruls to buy:");
		int count=0;
		for(std::set<int>::iterator iter=prulNumToBuy.begin();iter!=prulNumToBuy.end() && count<10;iter++,count++)
			printf(" %d",*iter);
		printf("\n\n");
	}
}