#define _CRT_SECURE_NO_WARNINGS

#include<cstdio>

const int TICKET_NUM_MAX=10;

int ticketNum;
long long sumCount[TICKET_NUM_MAX/2+1][TICKET_NUM_MAX/2*9+1];

void countSum(){
	for(int ticket=0;ticket<=TICKET_NUM_MAX/2;ticket++)
		for(int sum=0;sum<=TICKET_NUM_MAX/2*9;sum++)
			sumCount[ticket][sum]=0;
	sumCount[0][0]=1;
	for(int ticket=0;ticket<=ticketNum/2;ticket++)
		for(int sum=0;sum<=ticket*9;sum++)
			for(int digit=0;digit<=9;digit++)
				if(sum>=digit)
					sumCount[ticket][sum]+=sumCount[ticket-1][sum-digit];
}

int main(){
	scanf("%d",&ticketNum);
	countSum();
	long long result=0;
	for(int sum=0;sum<=ticketNum*9/2;sum++)
		result+=sumCount[ticketNum/2][sum]*sumCount[ticketNum/2][sum];
	printf("%lld\n",result);
	return 0;
}