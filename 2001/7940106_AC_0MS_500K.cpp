#define _CRT_SECURE_NO_WARNINGS

#include<cstdio>

const int WORD_NUM_MAX=1000,WORD_LEN_MAX=20;

class TrieNode{
public:
	bool isWord;
	int count;
	TrieNode* children[26];
public:
	TrieNode(){
		isWord=false;
		count=0;
		for(int i=0;i<26;i++)
			children[i]=NULL;
	}
	void insert(char *word){
		count++;
		if(*word=='\0'){
			isWord=true;
			return;
		}
		if(children[*word-'a']==NULL)
			children[*word-'a']=new TrieNode();
		children[*word-'a']->insert(word+1);
	}
	int prefixLen(char *word){
		if(*word=='\0' || count==1)
			return 0;
		return children[*word-'a']->prefixLen(word+1)+1;
	}
};

int main(){
	char words[WORD_NUM_MAX+1][WORD_LEN_MAX+1];
	int wordNum=0;
	TrieNode root;
	while(true){
		if(scanf("%s",words[wordNum])==EOF)
			break;
		root.insert(words[wordNum]);
		wordNum++;
	}
	for(int i=0;i<wordNum;i++){
		printf("%s",words[i]);
		int len=root.prefixLen(words[i]);
		words[i][len]='\0';
		printf(" %s\n",words[i]);
	}
	return 0;
}