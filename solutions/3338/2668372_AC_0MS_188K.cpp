#include<iostream>
#include<memory>
using namespace std;

#define COORDINATE Point
#define GRID Point

int min(int left,int right)
{
	return (left<right)?left:right;
}

int max(int left,int right)
{
	return (left>right)?left:right;
}

class Point
{
	friend istream& operator>>(istream&,Point&);
public:
	Point(int=0,int=0);
	~Point(){};
	int GetX();
	int GetY();
	bool SetX(int);
	bool SetY(int);
	bool Set(int,int);
	Point operator+(Point);
private:
	int x,y;
};

Point::Point(int newX,int newY)
{
	Set(newX,newY);
}

int Point::GetX()
{
	return x;
}

int Point::GetY()
{
	return y;
}

bool Point::SetX(int newX)
{
	x=newX;
	return true;
}

bool Point::SetY(int newY)
{
	y=newY;
	return true;
}

bool Point::Set(int newX,int newY)
{
	SetX(newX);
	SetY(newY);
	return true;
}

Point Point::operator+(Point addend)
{
	return Point(x+addend.x,y+addend.y);
}

istream& operator>>(istream& input,Point& curPoint)
{
	int x,y;
	input>>x>>y;
	curPoint.Set(x,y);
	return input;
}

class Rectangle
{
	friend istream& operator>>(istream&,Rectangle&);
public:
	Rectangle(Point=Point(0,0),Point=Point(0,0));
	~Rectangle(){};
	bool Set(Point,Point);
	bool Cut(GRID,GRID);
private:
	bool Inside(GRID);
	bool Outside(GRID);
	Point leftBottom,rightTop;
};

Rectangle::Rectangle(Point cornerA,Point cornerB)
{
	Set(cornerA,cornerB);
}

bool Rectangle::Set(Point cornerA,Point cornerB)
{
	leftBottom.SetX(min(cornerA.GetX(),cornerB.GetX()));
	leftBottom.SetY(min(cornerA.GetY(),cornerB.GetY()));
	rightTop.SetX(max(cornerA.GetX(),cornerB.GetX()));
	rightTop.SetY(max(cornerA.GetY(),cornerB.GetY()));
	return true;
}

bool Rectangle::Cut(GRID current,GRID next)
{
	return	(Inside(current) && Outside(next)) ||
			(Outside(current) && Inside(next));
}

bool Rectangle::Inside(GRID grid)
{
	return	grid.GetX()>=leftBottom.GetX() && grid.GetX()<rightTop.GetX() &&
			grid.GetY()>=leftBottom.GetY() && grid.GetY()<rightTop.GetY();
}

bool Rectangle::Outside(GRID grid)
{
	return !Inside(grid);
}

istream& operator>>(istream& input,Rectangle& curRectangle)
{
	Point cornerA,cornerB;
	input>>cornerA>>cornerB;
	curRectangle.Set(cornerA,cornerB);
	return input;
}

class Case
{
public:
	Case();
	~Case(){};
	bool Input();
	void Output();
private:
	enum Constant{	RECTANGLE_NUM_MAX=50,
					WIDTH_MAX=20,
					LENGTH_MAX=20};
	enum Direction{	Dir_First=0,
					Dir_Up=0,
					Dir_Down=1,
					Dir_Left=2,
					Dir_Right=3,
					Dir_Last=3};
	static const COORDINATE DirectionCoordinate[4];
	void Count();
	void DFSTravel(GRID);
	bool GridInBoard(GRID);
	bool InOnePiece(GRID,GRID);
	int width,length,peopleNum,pieceNum;
	Rectangle cut[RECTANGLE_NUM_MAX];
	bool gridVisited[LENGTH_MAX][WIDTH_MAX];
};

const COORDINATE Case::DirectionCoordinate[4]={	COORDINATE(0,1),
												COORDINATE(0,-1),
												COORDINATE(-1,0),
												COORDINATE(1,0)};

Case::Case()
{
	pieceNum=0;
	memset(gridVisited,false,sizeof(gridVisited));
}

bool Case::Input()
{
	cin>>width>>length;
	if(width==0)
		return false;
	cin>>peopleNum;
	for(int people=0;people<peopleNum;people++)
		cin>>cut[people];
	return true;
}

void Case::Output()
{
	Count();
	cout<<pieceNum<<endl;
}

void Case::Count()
{
	for(int gridLeft=0;gridLeft<length;gridLeft++)
		for(int gridBottom=0;gridBottom<width;gridBottom++)
			if(!gridVisited[gridLeft][gridBottom])
			{
				pieceNum++;
				DFSTravel(GRID(gridLeft,gridBottom));
			}
}

void Case::DFSTravel(GRID current)
{
	gridVisited[current.GetX()][current.GetY()]=true;
	for(int dir=Dir_First;dir<=Dir_Last;dir++)
	{
		GRID next=current+DirectionCoordinate[dir];
		if(	GridInBoard(next) && 
			!gridVisited[next.GetX()][next.GetY()] && 
			InOnePiece(current,next))
			DFSTravel(next);
	}
}

bool Case::GridInBoard(GRID grid)
{
	return	grid.GetX()>=0 && grid.GetX()<length &&
			grid.GetY()>=0 && grid.GetY()<width;
}

bool Case::InOnePiece(GRID current,GRID next)
{
	for(int people=0;people<peopleNum;people++)
		if(cut[people].Cut(current,next))
			return false;
	return true;
}

void main()
{
	while(true)
	{
		Case cakeCut;
		if(!cakeCut.Input())
			break;
		cakeCut.Output();
	}
}