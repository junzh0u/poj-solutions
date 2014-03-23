#include<cstdio>
#include<algorithm>

int main(){
	int serialNum;
	scanf("%d",&serialNum);
	int *serials=new int[serialNum],serialMax=0;
	for(int index=0;index<serialNum;index++){
		scanf("%d",serials+index);
		if(serialMax<serials[index])
			serialMax=serials[index];
	}
	bool *prime=new bool[serialMax+1];
	prime[0]=false;
	std::fill(prime+1,prime+serialMax+1,true);
	for(int factor=2;factor*factor<=serialMax;factor++)
		if(prime[factor])
			for(int num=2*factor;num<=serialMax;num+=factor)
				prime[num]=false;
	int *factorMax=new int[serialNum];
	std::fill(factorMax,factorMax+serialNum,1);
	for(int factor=2;factor<=serialMax;factor++)
		if(prime[factor])
			for(int index=0;index<serialNum;index++)
				if(serials[index]%factor==0 && factorMax[index]<factor)
					factorMax[index]=factor;
	int chosenIndex=0;
	for(int index=1;index<serialNum;index++)
		if(factorMax[chosenIndex]<factorMax[index])
			chosenIndex=index;
	printf("%d\n",serials[chosenIndex]);
	delete[] factorMax;
	delete[] prime;
	delete[] serials;
	return 0;
}