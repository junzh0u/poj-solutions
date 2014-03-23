#include<iostream>
using namespace std;

const int SCORE_MATRIX[5][5]={	5,	-1,	-2,	-1,	-3,
											-1,	5,	-3,	-2,	-4,
											-2,	-3,	5,	-2,	-2,
											-1,	-2,	-2,	5,	-1,
											-3,	-4,	-2,	-1,	0	};
const int LEN_MAX=100,INF_SCORE=-100000;

class GeneMatch
{
public:
	GeneMatch();
	void Input();
	void Output();
private:
	//static const int SCORE_MATRIX[5][5];
	//static const int LEN_MAX=100,INF_SCORE=-100000;

	int Similarity(int,int);
	int Score(char,char);
	int Code(char);

	int leftLen,rightLen;
	char left[LEN_MAX+1],right[LEN_MAX+1];
	int greatScore[LEN_MAX+1][LEN_MAX+1];
};

/*const int GeneMatch::SCORE_MATRIX[5][5]={	5,	-1,	-2,	-1,	-3,
											-1,	5,	-3,	-2,	-4,
											-2,	-3,	5,	-2,	-2,
											-1,	-2,	-2,	5,	-1,
											-3,	-4,	-2,	-1,	0	};*/

GeneMatch::GeneMatch()
{
	for(int i=0;i<LEN_MAX+1;i++)
		for(int j=0;j<LEN_MAX+1;j++)
			greatScore[i][j]=INF_SCORE;
	greatScore[0][0]=0;
}

void GeneMatch::Input()
{
	cin>>leftLen>>left>>rightLen>>right;
}

void GeneMatch::Output()
{
	cout<<Similarity(leftLen,rightLen)<<endl;
}

int GeneMatch::Similarity(int leftPos,int rightPos)
{
	if(greatScore[leftPos][rightPos]!=INF_SCORE)
		return greatScore[leftPos][rightPos];
	int great=INF_SCORE,temp;
	if(leftPos>0)
	{
		temp=Similarity(leftPos-1,rightPos)+Score(left[leftPos-1],'-');
		if(great<temp)
			great=temp;
		if(rightPos>0)
		{
			temp=Similarity(leftPos-1,rightPos-1)+Score(left[leftPos-1],right[rightPos-1]);
			if(great<temp)
				great=temp;
		}
	}
	if(rightPos>0)
	{
		temp=Similarity(leftPos,rightPos-1)+Score('-',right[rightPos-1]);
		if(great<temp)
			great=temp;
	}
	return greatScore[leftPos][rightPos]=great;
}

int GeneMatch::Score(char left,char right)
{
	return SCORE_MATRIX[Code(left)][Code(right)];
}

int GeneMatch::Code(char letter)
{
	switch(letter)
	{
	case'A':return 0;
	case'C':return 1;
	case'G':return 2;
	case'T':return 3;
	case'-':return 4;
	default:return -1;
	}
}

void main()
{
	int caseNum;
	cin>>caseNum;
	while((caseNum--)>0)
	{
		GeneMatch testCase;
		testCase.Input();
		testCase.Output();
	}
}