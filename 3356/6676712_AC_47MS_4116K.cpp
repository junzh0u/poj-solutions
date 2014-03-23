#include<cstdio>
#include<algorithm>

int main(){
	while(true){
		int fromLen;
		if(scanf("%d",&fromLen)==EOF)
			break;
		char *from=new char[fromLen+1];
		scanf("%s",from);

		int toLen;
		scanf("%d",&toLen);
		char *to=new char[toLen+1];
		scanf("%s",to);

		int **transform=new int*[fromLen+1];
		for(int i=0;i<=fromLen;i++)
			transform[i]=new int[toLen+1];

		for(int toEnd=0;toEnd<=toLen;toEnd++)
			transform[0][toEnd]=toEnd;

		for(int fromEnd=1;fromEnd<=fromLen;fromEnd++){
			transform[fromEnd][0]=fromEnd;
			for(int toEnd=1;toEnd<=toLen;toEnd++){
				if(from[fromEnd-1]==to[toEnd-1])
					transform[fromEnd][toEnd]=transform[fromEnd-1][toEnd-1];
				else
					transform[fromEnd][toEnd]=std::min(std::min(transform[fromEnd][toEnd-1],transform[fromEnd-1][toEnd]),
						transform[fromEnd-1][toEnd-1])+1;
			}
		}

		printf("%d\n",transform[fromLen][toLen]);

		for(int i=0;i<=fromLen;i++)
			delete[] transform[i];
		delete[] transform;
	}
	return 0;
}