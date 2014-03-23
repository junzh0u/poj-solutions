#include<cstdio>
#include<set>

struct Dot{
	int x,y;
};

struct Slope{
	int numerator,denominator;
};

struct SlopeLess{
	bool operator()(const Slope &left,const Slope &right){
		if(left.denominator==0)
			return false;
		if(right.denominator==0)
			return true;
		//return left.numerator*right.denominator<left.denominator*right.numerator;
		return (double)left.numerator/left.denominator<(double)right.numerator/right.denominator;
	}
};

int main(){
	int dotNum;
	scanf("%d",&dotNum);
	Dot *dots=new Dot[dotNum];
	for(int dotIndex=0;dotIndex<dotNum;dotIndex++)
		scanf("%d%d",&dots[dotIndex].x,&dots[dotIndex].y);
	std::set<Slope,SlopeLess> slopes;
	for(int dotFrom=0;dotFrom<dotNum;dotFrom++)
		for(int dotTo=dotFrom+1;dotTo<dotNum;dotTo++){
			Slope slope;
			slope.denominator=dots[dotTo].x-dots[dotFrom].x;
			slope.numerator=dots[dotTo].y-dots[dotFrom].y;
			slopes.insert(slope);
		}
	printf("%d\n",slopes.size());
	return 0;
}