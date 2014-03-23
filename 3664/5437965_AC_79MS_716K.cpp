#include<cstdio>
#include<algorithm>
using namespace std;

struct Cow{
	int id,firstRoundBallot,secondRoundBallot;
};

bool FirstRoundCompare(const Cow &left,const Cow& right){
	return left.firstRoundBallot>right.firstRoundBallot;
}

bool SecondRoundCompare(const Cow &left,const Cow& right){
	return left.secondRoundBallot>right.secondRoundBallot;
}

int main(){
	int cowNum,secondRoundScale;
	scanf("%d%d",&cowNum,&secondRoundScale);
	Cow *cows=new Cow[cowNum];
	for(int i=0;i<cowNum;i++){
		cows[i].id=i+1;
		scanf("%d%d",&cows[i].firstRoundBallot,&cows[i].secondRoundBallot);
	}
	sort(cows,cows+cowNum,FirstRoundCompare);
	sort(cows,cows+secondRoundScale,SecondRoundCompare);
	printf("%d\n",cows[0].id);
	delete[] cows;
	return 0;
}