#define _CRT_SECURE_NO_WARNINGS
#include<cstdio>
#include<climits>

const int N_MAX=12;
int main(){
	int three[N_MAX]={1};
	for(int n=1;n<N_MAX;n++)
		three[n]=2*three[n-1]+1;
	int four[N_MAX]={1};
	for(int n=1;n<N_MAX;n++){
		four[n]=INT_MAX;
		for(int k=1;k<=n;k++){
			int step=2*four[n-k]+three[k-1];
			if(four[n]>step)
				four[n]=step;
		}
	}
	for(int n=0;n<N_MAX;n++)
		printf("%d\n",four[n]);
	return 0;
}