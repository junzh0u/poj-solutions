#include<iostream.h>

const int SIZE=1000;
int path[SIZE][SIZE];

int getPath(int row,int col)
{
	if(row<0 || col<0)
		return 0;
	else return path[row][col];
}

void main()
{
	const char FREE='.';
	int caseNum;
	cin>>caseNum;
	while(caseNum--)
	{
		int rowNum,columnNum;
		cin>>rowNum>>columnNum;
		char town[SIZE][SIZE+1];
		for(int row=0;row<rowNum;row++)
			cin>>town[row];
		for(row=0;row<rowNum;row++)
			for(int column=0;column<columnNum;column++)
				if(row==0 && column==0)
					path[row][column]=1;
				else if(town[row][column]==FREE)
					path[row][column]=getPath(row,column-1)+getPath(row-1,column);
				else
					path[row][column]=0;
		cout<<"Existuje "
			<<path[rowNum-1][columnNum-1]
			<<" ruznych cest."<<endl;
	}
}
