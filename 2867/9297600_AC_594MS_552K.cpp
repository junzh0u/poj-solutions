#define _CRT_SECURE_NO_WARNINGS
#include<cstdio>
#include<algorithm>

const int GENE_NUM_MAX=50000;
int geneNum;
int genes[GENE_NUM_MAX];
int positions[GENE_NUM_MAX];

int main(){
	for(int caseId=1;;caseId++){
		scanf("%d",&geneNum);
		if(!geneNum)
			return 0;
		for(int pos=0;pos<geneNum;pos++)
			genes[pos]=pos+1;
		int reversalNum;
		scanf("%d",&reversalNum);
		while(reversalNum--){
			int first,last;
			scanf("%d%d",&first,&last);
			first--;
			last--;
			while(first<last){
				std::swap(genes[first],genes[last]);
				first++;
				last--;
			}
		}
		for(int pos=0;pos<geneNum;pos++)
			positions[genes[pos]-1]=pos;
		printf("Genome %d\n",caseId);
		int queryNum;
		scanf("%d",&queryNum);
		while(queryNum--){
			int gene;
			scanf("%d",&gene);
			printf("%d\n",positions[gene-1]+1);
		}
	}
}