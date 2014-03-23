#include<cstdio>

const int SCOREMAX=1000,EXIT=0;

class Contestant
{
public:
	Contestant();
	~Contestant();
	bool judge(int);
	int average();
private:
	int max,min,sum,judgeNum;
};

Contestant::Contestant()
{
	min=SCOREMAX+1;
	max=sum=judgeNum=0;
}

Contestant::~Contestant()
{
}

bool Contestant::judge(int score)
{
	if(score>max)
		max=score;
	if(score<min)
		min=score;
	sum+=score;
	judgeNum++;
	return true;
}

int Contestant::average()
{
	return (sum-max-min)/(judgeNum-2);
}

void main()
{
	int judgeNum,score;
	while(scanf("%d",&judgeNum))
	{
		if(judgeNum==EXIT)
			break;
		Contestant performance;
		while(judgeNum--)
		{
			scanf("%d",&score);
			performance.judge(score);
		}
		printf("%d\n",performance.average());
	}
}