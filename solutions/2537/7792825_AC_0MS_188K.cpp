#define _CRT_SECURE_NO_WARNINGS
#define DEBUG

#include<cstdio>

const int WORD_LEN_MAX=100,ALPHABET_SIZE_MAX=10;

int alphabetSize,wordLen;
double probabilty[WORD_LEN_MAX][ALPHABET_SIZE_MAX];

int main(){
	while(scanf("%d%d",&alphabetSize,&wordLen)!=EOF){
		alphabetSize++;
		for(int last=0;last<alphabetSize;last++)
			probabilty[0][last]=100.0/alphabetSize;
		for(int end=1;end<wordLen;end++){
			for(int last=0;last<alphabetSize;last++){
				probabilty[end][last]=probabilty[end-1][last]/alphabetSize;
				if(last-1>=0)
					probabilty[end][last]+=probabilty[end-1][last-1]/alphabetSize;
				if(last+1<alphabetSize)
					probabilty[end][last]+=probabilty[end-1][last+1]/alphabetSize;
			}
		}
		double probabiltySum=0.0;
		for(int last=0;last<alphabetSize;last++)
			probabiltySum+=probabilty[wordLen-1][last];
		printf("%.5f\n",probabiltySum);
	}
	return 0;
}