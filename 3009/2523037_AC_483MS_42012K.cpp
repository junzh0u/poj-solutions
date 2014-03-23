#include<deque>
#include<queue>
#include<cstdio>
using namespace std;

typedef int Direction;
typedef pair<int,int> Coordinate;
const Direction		Direction_Begin=0,
					Direction_Up=0,
					Direction_Down=1,
					Direction_Left=2,
					Direction_Right=3,
					Direction_End=4;
const Coordinate MOVE[Direction_End]={	Coordinate(1,0),
										Coordinate(-1,0),
										Coordinate(0,-1),
										Coordinate(0,1)};
const int SIZE=20,LIMIT=10;
const int VACANT=0,BLOCK=1,START=2,GOAL=3;

int max(int left,int right)
{
	return (left>right)?left:right;
}

int min(int left,int right)
{
	return (left<right)?left:right;
}

class Curling
{
public:
	Curling(int width,int height);
	~Curling();
	bool Input();
	bool move(Direction);
	bool ReachGoal();
	int GetStep();
private:
	Coordinate start,goal;
	bool Available(Coordinate);
	int width,height,step;
	int board[SIZE][SIZE];
};

Curling::Curling(int width,int height)
{
	Curling::width=width;
	Curling::height=height;
	step=0;
}

Curling::~Curling()
{
}

bool Curling::Input()
{
	for(int row=0;row<height;row++)
		for(int column=0;column<width;column++)
		{
			scanf("%d",&board[row][column]);
			if(board[row][column]==START)
				start=Coordinate(row,column);
			else if(board[row][column]==GOAL)
				goal=Coordinate(row,column);
		}
	return true;
}

bool Curling::move(Direction moveTo)
{
	Coordinate face=MOVE[moveTo],current=start;
	current.first+=face.first;
	current.second+=face.second;
	if(!Available(current))
		return false;
	if(board[current.first][current.second]==BLOCK)
		return false;
	while(Available(current) && board[current.first][current.second]==VACANT)
	{
		current.first+=face.first;
		current.second+=face.second;
	}
	if(!Available(current))
		return false;
	if(board[current.first][current.second]==GOAL)
	{
		board[start.first][start.second]=VACANT;
		start=current;
		goal=Coordinate(-1,-1);
	}
	else if(board[current.first][current.second]==BLOCK)
	{
		board[current.first][current.second]=VACANT;
		board[start.first][start.second]=VACANT;
		start=Coordinate(current.first-face.first,current.second-face.second);
	}
	board[start.first][start.second]=START;
	step++;
	return true;
}

bool Curling::ReachGoal()
{
	return !Available(goal);
}

int Curling::GetStep()
{
	return step;
}

bool Curling::Available(Coordinate position)
{
	return position.first>=0 && position.first<height && 
		position.second>=0 && position.second<width;
}


void main()
{
	while(true)
	{
		int width,height;
		scanf("%d%d",&width,&height);
		if(width==0 || height==0)
			break;
		Curling start(width,height);
		start.Input();
		queue<Curling> BFS;
		BFS.push(start);
		bool goalReached=false;
		int step;
		while(!goalReached)
		{
			Curling oldStatus(width,height),current(width,height);
			if(BFS.empty())
				break;
			oldStatus=BFS.front();
			BFS.pop();
			for(Direction moveTo=Direction_Begin;moveTo<Direction_End;moveTo++)
			{
				current=oldStatus;
				if(current.move(moveTo))
				{
					if(current.GetStep()<LIMIT)
						BFS.push(current);
					if(current.ReachGoal())
					{
						goalReached=true;
						step=current.GetStep();
						break;
					}
				}
			}
		}
		printf("%d\n",goalReached?step:(-1));
	}
}