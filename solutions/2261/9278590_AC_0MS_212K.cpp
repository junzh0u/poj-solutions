#define _CRT_SECURE_NO_WARNINGS
#include<cstdio>
#include<map>
using std::map;

const int COUNTRY_NUM=16,ROUND_NUM=5,COUNTRY_NAME_LEN=10;
char names[COUNTRY_NUM][COUNTRY_NAME_LEN+1];
double defeat[COUNTRY_NUM][COUNTRY_NUM];

map<int,double> chanceToWin(int round,int pos){
	map<int,double> chanceTable;
	if(round==1){
		chanceTable[pos]=1.0;
		return chanceTable;
	}
	map<int,double> tableLeft=chanceToWin(round-1,2*pos),tableRight=chanceToWin(round-1,2*pos+1);
	for(map<int,double>::iterator iterLeft=tableLeft.begin();iterLeft!=tableLeft.end();iterLeft++){
		for(map<int,double>::iterator iterRight=tableRight.begin();iterRight!=tableRight.end();iterRight++){
			int countryLeft=iterLeft->first,countryRight=iterRight->first;
			double chanceMeet=iterLeft->second*iterRight->second;
			chanceTable[countryLeft]+=chanceMeet*defeat[countryLeft][countryRight];
			chanceTable[countryRight]+=chanceMeet*defeat[countryRight][countryLeft];
		}
	}
	return chanceTable;
}

int main(){
	for(int i=0;i<COUNTRY_NUM;i++)
		scanf("%s",names+i);
	for(int i=0;i<COUNTRY_NUM;i++){
		for(int j=0;j<COUNTRY_NUM;j++){
			scanf("%lf",defeat[i]+j);
			defeat[i][j]/=100.0;
		}
	}
	map<int,double> chanceTable=chanceToWin(ROUND_NUM,0);
	for(map<int,double>::iterator iter=chanceTable.begin();iter!=chanceTable.end();iter++)
		printf("%-10s p=%.2f%%\n",names[iter->first],iter->second*100.0);
	return 0;
}