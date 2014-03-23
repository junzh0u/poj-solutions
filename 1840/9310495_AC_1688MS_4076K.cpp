#define _CRT_SECURE_NO_WARNINGS
#include<cstdio>
#include<algorithm>

const int X_MIN=-50,X_MAX=50,COEF_NUM=5;
const int X_RANGE=X_MAX-X_MIN;
const int ARRAY_SIZE=X_RANGE*X_RANGE*X_RANGE;

int coef[COEF_NUM],left[ARRAY_SIZE];

int binaryCount(int begin,int end,int value){
	if(begin>=end)
		return 0;
	if(begin+1==end)
		return (left[begin]==value)?1:0;
	if(value<left[begin])
		return 0;
	if(value>left[end-1])
		return 0;
	int mid=(begin+end)/2;
	return binaryCount(begin,mid,value)+binaryCount(mid,end,value);
}

int main(){
	for(int i=0;i<COEF_NUM;i++)
		scanf("%d",coef+i);
	int x[COEF_NUM],part[COEF_NUM],index=0;
	for(x[0]=X_MIN;x[0]<=X_MAX;x[0]++){
		if(x[0]==0)
			continue;
		part[0]=coef[0]*x[0]*x[0]*x[0];
		for(x[1]=X_MIN;x[1]<=X_MAX;x[1]++){
			if(x[1]==0)
				continue;
			part[1]=coef[1]*x[1]*x[1]*x[1];
			for(x[2]=X_MIN;x[2]<=X_MAX;x[2]++){
				if(x[2]==0)
					continue;
				part[2]=coef[2]*x[2]*x[2]*x[2];
				left[index++]=part[0]+part[1]+part[2];
			}
		}
	}
	std::sort(left,left+ARRAY_SIZE);
	int count=0;
	for(x[3]=X_MIN;x[3]<=X_MAX;x[3]++){
		if(x[3]==0)
			continue;
		part[3]=coef[3]*x[3]*x[3]*x[3];
		for(x[4]=X_MIN;x[4]<=X_MAX;x[4]++){
			if(x[4]==0)
				continue;
			part[4]=coef[4]*x[4]*x[4]*x[4];
			count+=binaryCount(0,ARRAY_SIZE,-part[3]-part[4]);
		}
	}
	printf("%d\n",count);
	return 0;
}