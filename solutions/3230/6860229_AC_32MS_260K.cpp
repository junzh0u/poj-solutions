#include<cstdio>
#include<climits>

int main(){
	while(true){
		int cityNum,dayNum;
		scanf("%d%d",&cityNum,&dayNum);
		if(cityNum==0 && dayNum==0)
			return 0;

		int **cost=new int*[cityNum];
		for(int from=0;from<cityNum;from++){
			cost[from]=new int[cityNum];
			for(int to=0;to<cityNum;to++)
				scanf("%d",cost[from]+to);
		}

		int **income=new int*[dayNum];
		for(int day=0;day<dayNum;day++){
			income[day]=new int[cityNum];
			for(int city=0;city<cityNum;city++)
				scanf("%d",income[day]+city);
		}

		int **totalIncome=new int*[dayNum];
		for(int day=0;day<dayNum;day++){
			totalIncome[day]=new int[cityNum];
			for(int city=0;city<cityNum;city++)
				totalIncome[day][city]=0-INT_MAX;
		}

		for(int city=0;city<cityNum;city++)
			totalIncome[0][city]=income[0][city]-cost[0][city];
		for(int day=0;day+1<dayNum;day++)
			for(int from=0;from<cityNum;from++)
				for(int to=0;to<cityNum;to++)
					if(totalIncome[day+1][to]<totalIncome[day][from]+income[day+1][to]-cost[from][to])
						totalIncome[day+1][to]=totalIncome[day][from]+income[day+1][to]-cost[from][to];

		int maxIncome=0-INT_MAX;
		for(int city=0;city<cityNum;city++)
			if(maxIncome<totalIncome[dayNum-1][city])
				maxIncome=totalIncome[dayNum-1][city];
		printf("%d\n",maxIncome);
		
		for(int day=0;day<dayNum;day++)
			delete[] totalIncome[day];
		delete[] totalIncome;

		for(int day=0;day<dayNum;day++)
			delete[] income[day];
		delete income;

		for(int from=0;from<cityNum;from++)
			delete[] cost[from];
		delete[] cost;
	}
}