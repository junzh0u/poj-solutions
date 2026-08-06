#define _CRT_SECURE_NO_WARNINGS
//#define DEBUG

#include<iostream>
#include<map>
#include<string>
#include<algorithm>
using namespace std;

const int UNDEF=-1;

double lenTotal;
int houseNum;
map<string,int> houseIndexes;
int pathNum;
double **pathLen;
double lenNeed;

double *distanceMin;
int inTreeCount;

void connect(int index){
	distanceMin[index]=0.0;
	inTreeCount++;
	for(int to=0;to<houseNum;to++){
		if(pathLen[index][to]==UNDEF)
			continue;
		if(distanceMin[to]==UNDEF || distanceMin[to]>pathLen[index][to])
			distanceMin[to]=pathLen[index][to];
	}
}

bool prim(){
	// Allocate memory
	lenNeed=0.0;
	inTreeCount=0;
	distanceMin=new double[houseNum];
	fill(distanceMin,distanceMin+houseNum,UNDEF);
	// Initialize first node
	connect(0);
	// Main loop
	while(inTreeCount<houseNum){
		int next=UNDEF;
		for(int index=0;index<houseNum;index++){
			if(distanceMin[index]==UNDEF || distanceMin[index]==0.0)
				continue;
			if(next==UNDEF || distanceMin[next]>distanceMin[index])
				next=index;
		}
		if(next==UNDEF)
			return false;
		lenNeed+=distanceMin[next];
		connect(next);
	}
	// Reclaim memory
	delete[] distanceMin;
	// Return
	return true;
}

int main(){
	// Read length total
	cin>>lenTotal;
	// Read houses
	cin>>houseNum;
	for(int houseIndex=0;houseIndex<houseNum;houseIndex++){
		string houseName;
		cin>>houseName;
		houseIndexes[houseName]=houseIndex;
	}
	// Allocate memory
	pathLen=new double*[houseNum];
	for(int from=0;from<houseNum;from++){
		pathLen[from]=new double[houseNum];
		fill(pathLen[from],pathLen[from]+houseNum,UNDEF);
	}
	// Read pathes
	cin>>pathNum;
	for(int pathIndex=0;pathIndex<pathNum;pathIndex++){
		string fromName,toName;
		cin>>fromName>>toName;
		int fromIndex=houseIndexes[fromName],toIndex=houseIndexes[toName];
		cin>>pathLen[fromIndex][toIndex];
		pathLen[toIndex][fromIndex]=pathLen[fromIndex][toIndex];
	}
#ifdef DEBUG
	for(int from=0;from<houseNum;from++){
		for(int to=0;to<houseNum;to++)
			cout<<pathLen[from][to]<<"\t";
		cout<<endl;
	}
#endif
	// Minimum spanning tree - PRIM
	if(!prim()){
#ifdef DEBUG
		cerr<<"Error!"<<endl;
#endif
	}
	// Output
	if(lenNeed>lenTotal)
		cout<<"Not enough cable"<<endl;
	else
		cout<<"Need "<<lenNeed<<" miles of cable"<<endl;
	// Reclaim memory
	for(int from=0;from<houseNum;from++)
		delete[] pathLen[from];
	delete[] pathLen;
	// Return
	return 0;
}