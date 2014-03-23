#define _CRT_SECURE_NO_WARNINGS
//#define DEBUG

#include<cstdio>
#include<cmath>

int remain(int personNum){
	if(personNum==1)
		return 0;
	if(personNum%2==0)
		return remain(personNum/2)*2;
	int half=(personNum+1)/2;
	int id=remain(half);
	id=(id+half-1)%half;
	id*=2;
	return id;
}

int main(){
	while(true){
		char temp[5];
		scanf("%s",temp);
		int personNum=((temp[0]-'0')*10+temp[1]-'0')*(int)std::pow(10.0,temp[3]-'0');
#ifdef DEBUG
		printf("%d\n",personNum);
#endif
		if(personNum==0)
			return 0;
		//printf("%d\n",remain(personNum)+1);

		// A math trick:
		
		personNum<<=1;
		personNum|=1;
		for(int i=31;i>=1;i--)
			if(personNum&(1<<i)){
				personNum^=(1<<i);
				break;
			}
		printf("%d\n",personNum);
		
	}
}