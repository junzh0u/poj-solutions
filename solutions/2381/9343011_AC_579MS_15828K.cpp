#define _CRT_SECURE_NO_WARNINGS
#include<cstdio>

const int MODULUS_MAX=16000000;
bool appear[MODULUS_MAX]={false};

int main(){
	unsigned multiple,increment,modulus,number;
	scanf("%u%u%u%u",&multiple,&increment,&modulus,&number);
	while(!appear[number]){
		appear[number]=true;
		number=(multiple*number+increment)%modulus;
	}
	int current=0;
	while(!appear[current])
		current++;
	unsigned gapMax=0;
	while(true){
		unsigned next=current+1;
		while(!appear[next] && next<modulus)
			next++;
		if(next>=modulus)
			break;
		if(gapMax<next-current)
			gapMax=next-current;
		current=next;
	}
	printf("%d\n",gapMax);
	return 0;
}