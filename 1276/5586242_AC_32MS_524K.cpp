#include<cstdio>
#include<algorithm>
using namespace std;

const int DENOMINATION_NUM_MAX=10;
const int CASH_MAX=100000;

struct Bill{
	int denomination,num;
};

int cashRequested,denominationNum;
Bill bills[DENOMINATION_NUM_MAX];
short remain[2][CASH_MAX+1];

int main(){
	while(scanf("%d%d",&cashRequested,&denominationNum)!=EOF){
		for(int i=0;i<denominationNum;i++)
			scanf("%d%d",&bills[i].num,&bills[i].denomination);
		memset(remain,-1,sizeof(short)*2*(CASH_MAX+1));
		int cashMax=0;
		for(int i=0;i<denominationNum;i++){
			remain[i%2][0]=bills[i].num;
			int limit=min(cashMax+bills[i].denomination*bills[i].num,cashRequested);
			for(int cash=1;cash<=limit;cash++){
				if(i>0 && remain[(i+1)%2][cash]>=0)
					remain[i%2][cash]=bills[i].num;
				else if(cash>=bills[i].denomination && remain[i%2][cash-bills[i].denomination]>0){
					remain[i%2][cash]=remain[i%2][cash-bills[i].denomination]-1;
					if(cashMax<cash)
						cashMax=cash;
				}
			}
		}
		printf("%d\n",cashMax);
	}
	return 0;
}