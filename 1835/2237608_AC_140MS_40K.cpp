#include<iostream.h>

struct Direction{int iFront,iTop,iLeft;};
struct Coordinate{int iX,iY,iZ;};

const int DIRECTION[6][3]={{1,0,0},{0,1,0},{0,0,1},{-1,0,0},{0,-1,0},{0,0,-1}};

void Back(Direction& dFace)
{
	dFace.iFront=(dFace.iFront+3)%6;
	dFace.iLeft=(dFace.iLeft+3)%6;
}

void Left(Direction& dFace)
{
	int iTemp=dFace.iFront;
	dFace.iFront=dFace.iLeft;
	dFace.iLeft=(iTemp+3)%6;
}

void Right(Direction& dFace)
{
	int iTemp=dFace.iLeft;
	dFace.iLeft=dFace.iFront;
	dFace.iFront=(iTemp+3)%6;
}

void Up(Direction& dFace)
{
	int iTemp=dFace.iFront;
	dFace.iFront=dFace.iTop;
	dFace.iTop=(iTemp+3)%6;
}

void Down(Direction& dFace)
{
	int iTemp=dFace.iTop;
	dFace.iTop=dFace.iFront;
	dFace.iFront=(iTemp+3)%6;
}

void Go(Coordinate& cMan,Direction& dFace,int iStep)
{
	cMan.iX+=iStep*DIRECTION[dFace.iFront][0];
	cMan.iY+=iStep*DIRECTION[dFace.iFront][1];
	cMan.iZ+=iStep*DIRECTION[dFace.iFront][2];
}

void main()
{
	int iCaseNum,iStepNum,iStep;
	char cTurn[10];
	Direction dFace;
	Coordinate cMan;
	cin>>iCaseNum;
	while(iCaseNum--)
	{
		cin>>iStepNum;
		dFace.iFront=0;dFace.iTop=2;dFace.iLeft=4;
		cMan.iX=cMan.iY=cMan.iZ=0;
		while(iStepNum--)
		{
			cin>>cTurn>>iStep;
			switch(cTurn[0])
			{
			case 'b':Back(dFace);break;
			case 'l':Left(dFace);break;
			case 'r':Right(dFace);break;
			case 'u':Up(dFace);break;
			case 'd':Down(dFace);break;
			default:break;
			}
			Go(cMan,dFace,iStep);
		}
		cout<<cMan.iX<<" "<<cMan.iY<<" "<<cMan.iZ<<" "<<dFace.iFront<<endl;
	}
}