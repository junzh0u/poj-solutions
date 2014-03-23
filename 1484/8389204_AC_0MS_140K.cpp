#define _CRT_SECURE_NO_WARNINGS

#include<cstdio>
#include<algorithm>

int main(){
	int sequenceId=1;
	while(true){
		int deviceNum,operationNum,capacity;
		scanf("%d%d%d",&deviceNum,&operationNum,&capacity);
		if(!deviceNum && !operationNum && !capacity)
			return 0;
		int* consumption=new int[deviceNum];
		for(int i=0;i<deviceNum;i++)
			scanf("%d",consumption+i);
		bool blown=false;
		int consumptionMax=0;
		bool* deviceOn=new bool[deviceNum];
		std::fill(deviceOn,deviceOn+deviceNum,false);
		int consumptionSum=0;
		while(operationNum--){
			int deviceIndex;
			scanf("%d",&deviceIndex);
			deviceIndex--;
			if(blown)
				continue;
			deviceOn[deviceIndex]=!deviceOn[deviceIndex];
			if(deviceOn[deviceIndex])
				consumptionSum+=consumption[deviceIndex];
			else
				consumptionSum-=consumption[deviceIndex];
			if(consumptionSum>capacity)
				blown=true;
			else if(consumptionMax<consumptionSum)
				consumptionMax=consumptionSum;
		}
		delete[] deviceOn;
		delete[] consumption;
		printf("Sequence %d\n",sequenceId++);
		if(blown)
			printf("Fuse was blown.\n");
		else
			printf("Fuse was not blown.\nMaximal power consumption was %d amperes.\n",consumptionMax);
		printf("\n");
	}
}