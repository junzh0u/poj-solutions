#include<cstdio>
#include<algorithm>

int find(int *array,int begin,int end,int upperBound){
	if(begin+1==end)
		return begin;
	int mid=(begin+end)/2;
	if(array[mid]<=upperBound)
		return find(array,mid,end,upperBound);
	else
		return find(array,begin,mid,upperBound);
}

bool checkMonthlyLimit(int monthlyLimit,int dayBegin,int dayEnd,int *daySpendSum,int periodNum){
	if(dayBegin>=dayEnd)
		return true;
	if(periodNum<=0)
		return false;
	int nextPeriodBegin=find(daySpendSum,dayBegin+1,dayEnd+1,daySpendSum[dayBegin]+monthlyLimit);
	return checkMonthlyLimit(monthlyLimit,nextPeriodBegin,dayEnd,daySpendSum,periodNum-1);
}

int main(){
	int dayNum,periodNum;
	scanf("%d%d",&dayNum,&periodNum);
	int *daySpendSum=new int[dayNum+1];
	int daySpendMax=0;
	daySpendSum[0]=0;
	for(int dayID=0;dayID<dayNum;dayID++){
		int spend;
		scanf("%d",&spend);
		daySpendSum[dayID+1]=daySpendSum[dayID]+spend;
		if(daySpendMax<spend)
			daySpendMax=spend;
	}
	int monthlyLimitMin=std::max(daySpendMax,daySpendSum[dayNum]/periodNum),monthlyLimitMax=daySpendSum[dayNum];
	while(monthlyLimitMin<monthlyLimitMax){
		int mid=(monthlyLimitMin+monthlyLimitMax)/2;
		if(checkMonthlyLimit(mid,0,dayNum,daySpendSum,periodNum))
			monthlyLimitMax=mid;
		else
			monthlyLimitMin=mid+1;
	}
	printf("%d\n",monthlyLimitMin);
	delete[] daySpendSum;
	return 0;
}