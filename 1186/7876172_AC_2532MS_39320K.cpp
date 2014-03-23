#define _CRT_SECURE_NO_WARNINGS

#include<cstdio>
#include<algorithm>

const int N_MAX=6,HASH_SIZE=5000000;

struct HashKey{
	int value,count;
};

int n,m;
int k[N_MAX],p[N_MAX];
HashKey hashTable[HASH_SIZE];
int count=0;

int pow(int base,int exp){
	int result=1;
	while(exp){
		result*=base;
		exp--;
	}
	return result;
}

int hash(int value){
	return std::abs(value)%HASH_SIZE;
}

void insert(int value){
	for(int pos=hash(value);;pos++){
		if(hashTable[pos].count==0)
			hashTable[pos].value=value;
		if(hashTable[pos].value==value){
			hashTable[pos].count++;
			return;
		}
	}
}

int lookup(int value){
	for(int pos=hash(value);;pos++){
		if(hashTable[pos].count==0)
			return 0;
		if(hashTable[pos].value==value)
			return hashTable[pos].count;
	}
}

void dfsLeft(int pos,int value){
	if(pos>=n/2)
		return insert(value);
	for(int x=1;x<=m;x++)
		dfsLeft(pos+1,value+k[pos]*pow(x,p[pos]));
}

void dfsRight(int pos,int value){
	if(pos>=n){
		count+=lookup(-value);
		return;
	}
	for(int x=1;x<=m;x++)
		dfsRight(pos+1,value+k[pos]*pow(x,p[pos]));
}

int main(){
	memset(hashTable,0,sizeof(HashKey)*HASH_SIZE);
	scanf("%d%d",&n,&m);
	for(int i=0;i<n;i++)
		scanf("%d%d",k+i,p+i);
	if(n==1){
		printf("%d\n",k[0]?0:m);
		return 0;
	}
	dfsLeft(0,0);
	dfsRight(n/2,0);
	printf("%d\n",count);
	return 0;
}