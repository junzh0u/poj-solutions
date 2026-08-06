#include<cstdio>
#include<string>

const int VALUE[26]={2,5,4,4,1,6,5,5,1,7,6,3,5,2,3,5,7,2,1,2,4,6,6,7,5,7},LEN_MAX=7,WORD_NUM_MAX=5040;
int collect[26]={0},score[WORD_NUM_MAX]={0},scoreMax=0,wordNum=0;
char dictionary[WORD_NUM_MAX][LEN_MAX+1];

bool Legal(char* word){
	int count[26]={0};
	for(int i=0;i<strlen(word);i++){
		count[word[i]-'a']++;
		if(count[word[i]-'a']>collect[word[i]-'a'])
			return false;
	}
	return true;
}

int Score(char* word){
	int score=0;
	for(int i=0;i<strlen(word);i++)
		score+=VALUE[word[i]-'a'];
	return score;
}

int main(){
	char collectString[LEN_MAX+1];
	gets(collectString);
	for(int i=0;i<strlen(collectString);i++)
		collect[collectString[i]-'a']++;
	while(true){
		gets(dictionary[wordNum]);
		if(dictionary[wordNum][0]=='.')
			break;
		if(Legal(dictionary[wordNum])){
			score[wordNum]=Score(dictionary[wordNum]);
			if(scoreMax<score[wordNum])
				scoreMax=score[wordNum];
			for(int i=0;i<wordNum;i++){
				char temp[2*LEN_MAX+1];
				strcpy(temp,dictionary[i]);
				strcat(temp,dictionary[wordNum]);
				if(Legal(temp) && scoreMax<score[i]+score[wordNum])
					scoreMax=score[i]+score[wordNum];
			}
			wordNum++;
		}
	}
	printf("%d\n",scoreMax);
	return 0;
}