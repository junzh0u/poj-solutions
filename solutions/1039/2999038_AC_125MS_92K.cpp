#include<cstdio>
#include<iomanip>
#include<cmath>
using namespace std;

const int BENT_NUM_MAX=20;
typedef pair<double,double> Point;
int bentNum;
Point pipe[BENT_NUM_MAX];

class Line{
public:
	Line(Point,Point);
	Point Cross(Line);
	double XToY(double x);
private:
	double intercept,slope;
};

Line::Line(Point left,Point right){
	slope=(left.second-right.second)/(left.first-right.first);
	intercept=left.second-slope*left.first;
}

Point Line::Cross(Line another){
	double x=(another.intercept-intercept)/(slope-another.slope);
	return Point(x,XToY(x));
}

double Line::XToY(double x){
	return slope*x+intercept;
}

Point Down(Point up){
	return Point(up.first,up.second-1.0);
}

int Compare(double left,double right){
	double distance=left-right;
	if(fabs(distance)<1e-10)
		return 0;
	return (distance<0)?(-1):1;
}

int Cross(int up,int down,int bent){
	return true;
}

int main(){
	while(scanf("%d",&bentNum) && bentNum>0){
		for(int bent=0;bent<bentNum;bent++)
			scanf("%lf%lf",&pipe[bent].first,&pipe[bent].second);
		double distanceMax=pipe[0].first;
		for(int up=0;up<bentNum;up++)
			for(int down=0;down<bentNum;down++){
				if(up==down)
					continue;
				double distance=pipe[0].first;
				Line light(pipe[up],Down(pipe[down]));
				for(int bent=0;bent<bentNum;bent++){
					int pass=Compare(light.XToY(pipe[bent].first),pipe[bent].second)+Compare(light.XToY(pipe[bent].first),pipe[bent].second-1.0);
					if(abs(pass)<=1)
						distance=pipe[bent].first;
					else{
						if(bent!=0){
							if(pass>0)
								distance=light.Cross(Line(pipe[bent-1],pipe[bent])).first;
							else
								distance=light.Cross(Line(Down(pipe[bent-1]),Down(pipe[bent]))).first;
						}
						break;
					}
				}
				if(Compare(distanceMax,distance)<0)
					distanceMax=distance;
			}
		if(Compare(distanceMax,pipe[bentNum-1].first)==0)
			printf("Through all the pipe.\n");
		else
			printf("%.2lf\n",distanceMax);
	}
	return 0;
}