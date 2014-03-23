#include<cstdio>
#include<algorithm>

struct Snowflake{
	int armLen[6];
};

const int HASH_MAX=1000000,EMPTY=-1;

int hash(const Snowflake& snowflake){
	int sum=0;
	for(int armIndex=0;armIndex<6;armIndex++)
		sum+=snowflake.armLen[armIndex];
	return sum%HASH_MAX;
}

bool equals(const Snowflake& left,const Snowflake& right){
	for(int shift=0;shift<6;shift++){
		int same=true;
		for(int armIndex=0;armIndex<6;armIndex++)
			if(left.armLen[armIndex]!=right.armLen[(armIndex+shift)%6])
				same=false;
		if(same)
			return true;
		same=true;
		for(int armIndex=0;armIndex<6;armIndex++)
			if(left.armLen[armIndex]!=right.armLen[(5-armIndex+shift)%6])
				same=false;
		if(same)
			return true;
	}
	return false;
}

bool twinsExist(int existIndex,int newIndex,Snowflake snowflakes[],int next[]){
	if(equals(snowflakes[existIndex],snowflakes[newIndex]))
		return true;
	if(next[existIndex]==EMPTY){
		next[existIndex]=newIndex;
		return false;
	}
	return twinsExist(next[existIndex],newIndex,snowflakes,next);
}

int main(){
	int snowflakeNum;
	scanf("%d",&snowflakeNum);
	Snowflake *snowflakes=new Snowflake[snowflakeNum];
	int *hashTable=new int[HASH_MAX];
	std::fill(hashTable,hashTable+HASH_MAX,EMPTY);
	int *next=new int[snowflakeNum];
	std::fill(next,next+snowflakeNum,EMPTY);
	bool twinFound=false;
	for(int snowflakeIndex=0;snowflakeIndex<snowflakeNum;snowflakeIndex++){
		for(int armIndex=0;armIndex<6;armIndex++)
			scanf("%d",snowflakes[snowflakeIndex].armLen+armIndex);
		if(twinFound)
			continue;
		int hashCode=hash(snowflakes[snowflakeIndex]);
		if(hashTable[hashCode]==EMPTY)
			hashTable[hashCode]=snowflakeIndex;
		else
			twinFound=twinsExist(hashTable[hashCode],snowflakeIndex,snowflakes,next);
	}
	if(twinFound)
		printf("Twin snowflakes found.\n");
	else
		printf("No two snowflakes are alike.\n");
	delete[] next;
	delete[] hashTable;
	delete[] snowflakes;
	return 0;
}