#define _CRT_SECURE_NO_WARNINGS
//#define DEBUG

#include<cstdio>
#include<cctype>

const int LEN_MAX=1000;

int keyToPress(char letter){
	static const char KEYS[26]={2,2,2,3,3,3,
								4,4,4,5,5,5,6,6,6,
								7,7,7,7,8,8,8,9,9,9,9};
	if(letter==' ')
		return 1;
	if(isupper(letter))
		return KEYS[letter-'A'];
	return -1;
}

int timeToPress(char letter){
	static const char COUNTS[26]={	1,2,3,1,2,3,
									1,2,3,1,2,3,1,2,3,
									1,2,3,4,1,2,3,1,2,3,4};
	if(letter==' ')
		return 1;
	if(isupper(letter))
		return COUNTS[letter-'A'];
	return -1;
}

int main(){
	int caseNum;
	scanf("%d\n",&caseNum);
	while(caseNum--){
		int press,wait;
		scanf("%d %d\n",&press,&wait);
		char str[LEN_MAX];
		gets(str);
#ifdef DEBUG
		printf("# %d %d %s\n",press,wait,str);
#endif
		int time=0;
		int prevKey=-1;
		for(int i=0;str[i]!='\0';i++){
			int key=keyToPress(str[i]);
			if(key!=1 && key==prevKey)
				time+=wait;
			prevKey=key;
			time+=timeToPress(str[i])*press;
		}
		printf("%d\n",time);
	}
	return 0;
}