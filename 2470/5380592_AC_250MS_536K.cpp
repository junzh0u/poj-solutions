#include<cstdio>

int main(){
	while(true){
		int length;
		scanf("%d",&length);
		if(length==0)
			break;
		int *permutation=new int[length];
		for(int i=0;i<length;i++)
			scanf("%d",permutation+i);
		bool ambiguous=true;
		for(int i=0;i<length && ambiguous;i++)
			if(permutation[permutation[i]-1]!=i+1)
				ambiguous=false;
		printf(ambiguous?"ambiguous\n":"not ambiguous\n");
		delete[] permutation;
	}
	return 0;
}