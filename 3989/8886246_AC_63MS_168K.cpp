#define _CRT_SECURE_NO_WARNINGS

#include<cstdio>
#include<cstring>

const int STR_LEN_MAX=8,LETTER_NUM_MAX=5;

int stringValue(char expression[],int letterCode[],int letterValue[]){
	int value=0,factor=1;
	for(int i=strlen(expression)-1;i>=0;i--){
		value+=letterValue[letterCode[expression[i]-'A']]*factor;
		factor*=10;
	}
	return value;
}

int pow10(int exp){
	if(exp==0)
		return 1;
	return pow10(exp-1)*10;
}

int main(){
	int caseNum;
	scanf("%d",&caseNum);
	while(caseNum--){
		char expressions[3][STR_LEN_MAX+1];
		scanf("%s%s%s",expressions[0],expressions[1],expressions[2]);
		bool letterAppear[LETTER_NUM_MAX]={false};
		for(int expressionIndex=0;expressionIndex<3;expressionIndex++)
			for(int position=0;expressions[expressionIndex][position]!='\0';position++)
				letterAppear[expressions[expressionIndex][position]-'A']=true;
		int letterNum=0,letterCode[LETTER_NUM_MAX];
		for(int letter=0;letter<LETTER_NUM_MAX;letter++)
			if(letterAppear[letter])
				letterCode[letter]=letterNum++;
		bool couldNotBeZero[LETTER_NUM_MAX]={false};
		for(int expressionIndex=0;expressionIndex<3;expressionIndex++)
			if(strlen(expressions[expressionIndex])>1)
				couldNotBeZero[letterCode[expressions[expressionIndex][0]-'A']]=true;
		int count=0;
		for(int replace=0;replace<pow10(letterNum);replace++){
			int temp=replace,codeValue[LETTER_NUM_MAX];
			for(int code=0;code<letterNum;code++){
				codeValue[code]=temp%10;
				temp/=10;
			}
			bool legal=true;
			for(int i=0;i+1<letterNum && legal;i++)
				for(int j=i+1;j<letterNum && legal;j++)
					if(codeValue[i]==codeValue[j])
						legal=false;
			if(!legal)
				continue;
			for(int code=0;code<letterNum;code++)
				if(codeValue[code]==0 && couldNotBeZero[code]){
					legal=false;
					break;
				}
				if(!legal)
					continue;
				int expressionValues[3];
				for(int expressionIndex=0;expressionIndex<3;expressionIndex++)
					expressionValues[expressionIndex]=stringValue(expressions[expressionIndex],letterCode,codeValue);
				if(expressionValues[0]+expressionValues[1]==expressionValues[2]){
#ifdef _DEBUG
					printf("%d + %d = %d\n",expressionValues[0],expressionValues[1],expressionValues[2]);
#endif
					count++;
				}
				if(expressionValues[0]-expressionValues[1]==expressionValues[2]){
#ifdef _DEBUG
					printf("%d - %d = %d\n",expressionValues[0],expressionValues[1],expressionValues[2]);
#endif
					count++;
				}
				if(expressionValues[0]*expressionValues[1]==expressionValues[2]){
#ifdef _DEBUG
					printf("%d * %d = %d\n",expressionValues[0],expressionValues[1],expressionValues[2]);
#endif
					count++;
				}
				if(expressionValues[1]!=0 && expressionValues[0]==expressionValues[1]*expressionValues[2]){
#ifdef _DEBUG
					printf("%d / %d = %d\n",expressionValues[0],expressionValues[1],expressionValues[2]);
#endif
					count++;
				}
		}
		printf("%d\n",count);
	}
	return 0;
}