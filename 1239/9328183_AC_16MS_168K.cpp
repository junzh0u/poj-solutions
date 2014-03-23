#define _CRT_SECURE_NO_WARNINGS
#include<cstdio>
#include<cstring>
#include<cstdlib>
#include<algorithm>

const int DIGIT_NUM_MAX=80;
const char END_OF_INPUT[]="0";

int compare(char *digits,int leftFirst,int leftLast,int rightFirst,int rightLast){
	while(leftLast-leftFirst<rightLast-rightFirst)
		if(digits[rightFirst++]!='0')
			return -1;
	while(leftLast-leftFirst>rightLast-rightFirst)
		if(digits[leftFirst++]!='0')
			return 1;
	while(leftFirst<=leftLast){
		if(digits[leftFirst]!=digits[rightFirst])
			return digits[leftFirst]-digits[rightFirst];
		leftFirst++;
		rightFirst++;
	}
	return 0;
}

bool lastMaxBetter(int len,int *lastMaxLeft,int *lastMaxRight){
	for(int first=0;first<len;first=lastMaxLeft[first]+1)
		if(lastMaxLeft[first]!=lastMaxRight[first])
			return lastMaxLeft[first]>lastMaxRight[first];
	return false;
}

int main(){
	char digits[DIGIT_NUM_MAX+1];
	while(scanf("%s",digits)){
		if(strcmp(digits,END_OF_INPUT)==0)
			return 0;
		int len=strlen(digits),firstMax[DIGIT_NUM_MAX];
		std::fill(firstMax,firstMax+len,0);
		for(int last=1;last<len;last++){
			for(int first=last;first>0;first--){
				int prevLast=first-1;
				int prevFirst=firstMax[prevLast];
				if(compare(digits,prevFirst,prevLast,first,last)<0){
					firstMax[last]=first;
					break;
				}
			}
		}
#ifdef _DEBUG
		int sectionNum=0;
		char sections[DIGIT_NUM_MAX][DIGIT_NUM_MAX+1];
		for(int last=len-1;last>=0;last=firstMax[last]-1){
			strncpy(sections[sectionNum],digits+firstMax[last],last-firstMax[last]+1);
			sections[sectionNum++][last-firstMax[last]+1]='\0';
		}
		printf("1: ");
		for(int i=sectionNum-1;i>=0;i--){
			printf("%s",sections[i]);
			if(i)
				printf(",");
		}
		printf("\n");
#endif
		int lastMaxBest[DIGIT_NUM_MAX],lastSectionFirst=firstMax[len-1];
		std::fill(lastMaxBest,lastMaxBest+len,0);
		while(true){
			int lastMax[DIGIT_NUM_MAX];
			std::fill(lastMax,lastMax+len,0);
			lastMax[lastSectionFirst]=len-1;
			for(int first=lastSectionFirst-1;first>=0;first--){
				for(int last=lastSectionFirst-1;last>=first;last--){
					int nextFirst=last+1;
					int nextLast=lastMax[nextFirst];
					if(compare(digits,first,last,nextFirst,nextLast)<0){
						lastMax[first]=last;
						break;
					}
				}
			}
#ifdef _DEBUG
			printf("2: ");
			for(int first=0;first<len;first=lastMax[first]+1){
				int last=lastMax[first];
				char temp=digits[last+1];
				digits[last+1]='\0';
				if(first)
					printf(",");
				printf("%s",digits+first);
				digits[last+1]=temp;
			}
			printf("\n");
#endif
			if(lastMaxBetter(len,lastMax,lastMaxBest))
				std::copy(lastMax,lastMax+len,lastMaxBest);
			if(lastSectionFirst>0 && digits[lastSectionFirst-1]=='0')
				lastSectionFirst--;
			else
				break;
		}
		for(int first=0;first<len;first=lastMaxBest[first]+1){
			int last=lastMaxBest[first];
			char temp=digits[last+1];
			digits[last+1]='\0';
			if(first)
				printf(",");
			printf("%s",digits+first);
			digits[last+1]=temp;
		}
		printf("\n");
	}
}