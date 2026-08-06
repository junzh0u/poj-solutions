#include<cstdio>
#include<cstring>
#include<algorithm>

const int SEQUENCE_NUM_MAX=10,SEQUENCE_LEN=60;

int main(){
	int caseNum;
	scanf("%d",&caseNum);
	while(caseNum--){
		int sequenceNum;
		scanf("%d",&sequenceNum);
		char sequences[SEQUENCE_NUM_MAX][SEQUENCE_LEN+1];
		for(int sequenceIndex=0;sequenceIndex<sequenceNum;sequenceIndex++)
			scanf("%s",sequences+sequenceIndex);
		char longestSubsequence[SEQUENCE_LEN+1]="";
		for(int begin=0;begin<SEQUENCE_LEN;begin++)
			for(int length=std::max(3,(int)strlen(longestSubsequence));length+begin<=SEQUENCE_LEN;length++){
				char subsequence[SEQUENCE_LEN+1];
				strncpy(subsequence,sequences[0]+begin,length);
				subsequence[length]='\0';
				bool satisfied=true;
				for(int sequenceIndex=1;satisfied && sequenceIndex<sequenceNum;sequenceIndex++)
					if(strstr(sequences[sequenceIndex],subsequence)==NULL)
						satisfied=false;
				if(satisfied){
					if(length>strlen(longestSubsequence))
						strcpy(longestSubsequence,subsequence);
					else if(strcmp(longestSubsequence,subsequence)>0)
						strcpy(longestSubsequence,subsequence);
				}
			}
		if(strlen(longestSubsequence)<3)
			puts("no significant commonalities");
		else
			puts(longestSubsequence);
	}
	return 0;
}