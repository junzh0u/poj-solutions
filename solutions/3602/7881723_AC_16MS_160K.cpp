#define _CRT_SECURE_NO_WARNINGS

#include<cstdio>
#include<set>
#include<cstring>

const int TEXT_SIZE_MAX=100,LIGATURE_NUM=7;

struct Ligature{
	char content[4];
	char symbol;
	bool match(char *text) const{
		for(int i=0;content[i]!='\0';i++)
			if(text[i]!=content[i])
				return false;
		return true;
	}
};

const Ligature LIGATURES[LIGATURE_NUM]={
	{"ffi",'0'},
	{"ffl",'1'},
	{"``",'2'},
	{"''",'3'},
	{"ff",'4'},
	{"fi",'5'},
	{"fl",'6'},
};


int main(){
	char text[TEXT_SIZE_MAX+1];
	std::set<char> typeset;
	while(scanf("%s",text)!=EOF){
		for(int index=0;text[index]!='\0';){
			bool found=false;
			for(int ligatureIndex=0;ligatureIndex<LIGATURE_NUM;ligatureIndex++){
				if(LIGATURES[ligatureIndex].match(text+index)){
					typeset.insert(LIGATURES[ligatureIndex].symbol);
					found=true;
					index+=strlen(LIGATURES[ligatureIndex].content);
					break;
				}
			}
			if(!found){
				typeset.insert(text[index]);
				index++;
			}
		}
	}
	printf("%d\n",typeset.size());
	return 0;
}