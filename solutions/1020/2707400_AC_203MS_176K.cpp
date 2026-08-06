#include<iostream>
#include<algorithm>
#include<functional>
using namespace std;

const int PIECE_NUM_MAX=30,TOTAL_SIZE_MAX=60;
typedef pair<int,int> COORDINATE;

class Case
{
public:
	Case();
	bool Input();
	void Output();
private:
	//const static int PIECE_NUM_MAX=16,TOTAL_SIZE_MAX=40;
	void Solve();
	bool Cut();
	COORDINATE FirstUncutGrid();
	bool Cutable(COORDINATE,int);
	void CutPiece(COORDINATE,int);
	void ResumePiece(COORDINATE,int);
	int totalSize,pieceNum,pieceSize[PIECE_NUM_MAX],pieceUnused;
	bool gridCut[TOTAL_SIZE_MAX][TOTAL_SIZE_MAX],pieceUsed[PIECE_NUM_MAX];
	bool suitable;
};

Case::Case()
{
}

bool Case::Input()
{
	cin>>totalSize>>pieceNum;
	pieceUnused=pieceNum;
	for(int piece=0;piece<pieceNum;piece++)
	{
		cin>>pieceSize[piece];
		pieceUsed[piece]=false;
	}
	sort(pieceSize,pieceSize+pieceNum,greater<int>());
	for(int row=0;row<totalSize;row++)
		for(int col=0;col<totalSize;col++)
			gridCut[row][col]=false;
	return true;
}

void Case::Output()
{
	Solve();
	cout<<(suitable?"KHOOOOB!":"HUTUTU!")<<endl;
}

void Case::Solve()
{
	int pieceAreaSum=0;
	for(int piece=0;piece<pieceNum;piece++)
		pieceAreaSum+=pieceSize[piece]*pieceSize[piece];
	if(pieceAreaSum!=totalSize*totalSize)
	{
		suitable=false;
		return;
	}
	suitable=Cut();
}

bool Case::Cut()
{
	if(pieceUnused==0)
		return true;
	COORDINATE gridLeftTop=FirstUncutGrid();
	if(gridLeftTop.first<0)
		return false;
	int lastFalseSize=-1;
	for(int piece=0;piece<pieceNum;piece++)
	{
		if(pieceUsed[piece] || pieceSize[piece]==lastFalseSize)
			continue;
		if(Cutable(gridLeftTop,piece))
		{
			CutPiece(gridLeftTop,piece);
			if(Cut())
				return true;
			ResumePiece(gridLeftTop,piece);
			lastFalseSize=pieceSize[piece];
		}
	}
	return false;
}

void Case::CutPiece(COORDINATE leftTop,int piece)
{
	for(int row=leftTop.first;row<leftTop.first+pieceSize[piece];row++)
		for(int col=leftTop.second;col<leftTop.second+pieceSize[piece];col++)
			gridCut[row][col]=true;
	pieceUsed[piece]=true;
	pieceUnused--;
}

void Case::ResumePiece(COORDINATE leftTop,int piece)
{
	for(int row=leftTop.first;row<leftTop.first+pieceSize[piece];row++)
		for(int col=leftTop.second;col<leftTop.second+pieceSize[piece];col++)
			gridCut[row][col]=false;
	pieceUsed[piece]=false;
	pieceUnused++;
}

COORDINATE Case::FirstUncutGrid()
{
	COORDINATE grid;
	for(grid.first=0;grid.first<totalSize;grid.first++)
		for(grid.second=0;grid.second<totalSize;grid.second++)
			if(!gridCut[grid.first][grid.second])
				return grid;
	return COORDINATE(-1,-1);
}

bool Case::Cutable(COORDINATE leftTop,int piece)
{
	if(leftTop.first+pieceSize[piece]>totalSize || leftTop.second+pieceSize[piece]>totalSize)
		return false;
	for(int row=leftTop.first;row<leftTop.first+pieceSize[piece];row++)
		for(int col=leftTop.second;col<leftTop.second+pieceSize[piece];col++)
			if(gridCut[row][col])
				return false;
	return true;
}

void main()
{
	int caseNum;
	cin>>caseNum;
	Case cutCake;
	while((caseNum--)>0)
	{
		cutCake.Input();
		cutCake.Output();
	}
}