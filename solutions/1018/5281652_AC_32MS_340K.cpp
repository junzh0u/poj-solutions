#include<cstdio>
#include<functional>
#include<set>
using namespace std;

struct Device{
	int factoryNum;
	int *bandwidths,*prices;
	int minBandwidth,maxBandwidth;
};

int main(){
	int caseNum;
	scanf("%d",&caseNum);
	while(caseNum-->0){
		int deviceNum;
		scanf("%d",&deviceNum);
		Device *devices=new Device[deviceNum];
		int minBandwidth=INT_MAX,maxBandwidth=INT_MAX;
		for(int i=0;i<deviceNum;i++){
			scanf("%d",&devices[i].factoryNum);
			devices[i].bandwidths=new int[devices[i].factoryNum];
			devices[i].prices=new int[devices[i].factoryNum];
			devices[i].maxBandwidth=INT_MIN;
			devices[i].minBandwidth=INT_MAX;
			for(int j=0;j<devices[i].factoryNum;j++){
				scanf("%d%d",devices[i].bandwidths+j,devices[i].prices+j);
				if(devices[i].minBandwidth>devices[i].bandwidths[j])
					devices[i].minBandwidth=devices[i].bandwidths[j];
				if(devices[i].maxBandwidth<devices[i].bandwidths[j])
					devices[i].maxBandwidth=devices[i].bandwidths[j];
			}
			if(minBandwidth>devices[i].minBandwidth)
				minBandwidth=devices[i].minBandwidth;
			if(maxBandwidth>devices[i].maxBandwidth)
				maxBandwidth=devices[i].maxBandwidth;
		}
		set<int> bandwidthTried;
		double maxBandwidthPerPrice=0.0;
		for(int i=0;i<deviceNum;i++){
			for(int j=0;j<devices[i].factoryNum;j++){
				if(devices[i].bandwidths[j]>=minBandwidth && devices[i].bandwidths[j]<=maxBandwidth &&
					bandwidthTried.find(devices[i].bandwidths[j])==bandwidthTried.end()){
						int minPrice=0;
						for(int k=0;k<deviceNum;k++){
							int minDevicePrice=INT_MAX;
							for(int l=0;l<devices[k].factoryNum;l++)
								if(devices[k].bandwidths[l]>=devices[i].bandwidths[j] &&
									minDevicePrice>devices[k].prices[l])
									minDevicePrice=devices[k].prices[l];
							minPrice+=minDevicePrice;
						}
						if(maxBandwidthPerPrice<devices[i].bandwidths[j]/(double)minPrice)
							maxBandwidthPerPrice=devices[i].bandwidths[j]/(double)minPrice;
						bandwidthTried.insert(devices[i].bandwidths[j]);
				}
			}
		}
		printf("%.3lf\n",maxBandwidthPerPrice);
		for(int i=0;i<deviceNum;i++){
			delete[] devices[i].bandwidths;
			delete[] devices[i].prices;
		}
		delete[] devices;
	}
	return 0;
}