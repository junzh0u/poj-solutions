#define _CRT_SECURE_NO_WARNINGS
#include<cstdio>

const int CUBIC_MAX=53,TETRAHEDRAL_MAX=95;

int cubic[CUBIC_MAX+1],tetrahedral[TETRAHEDRAL_MAX+1];

int main(){
	for(int i=0;i<=CUBIC_MAX;i++)
		cubic[i]=i*i*i;
	for(int i=0;i<=TETRAHEDRAL_MAX;i++)
		tetrahedral[i]=i*(i+1)*(i+2)/6;
#ifdef _DEBUG
	printf("cubic:\n");
	for(int i=0;i<=CUBIC_MAX;i++)
		printf("%d\t",cubic[i]);
	printf("\ntetrahedral:\n");
	for(int i=0;i<=TETRAHEDRAL_MAX;i++)
		printf("%d\t",tetrahedral[i]);
	printf("\n");
#endif
	while(true){
		int offer;
		scanf("%d",&offer);
		if(!offer)
			return 0;
		int request=0;
		for(int i=0;i<=CUBIC_MAX;i++)
			for(int j=0;j<=TETRAHEDRAL_MAX;j++)
				if(offer>=cubic[i]+tetrahedral[j] && request<cubic[i]+tetrahedral[j])
					request=cubic[i]+tetrahedral[j];
		printf("%d\n",request);
	}
}