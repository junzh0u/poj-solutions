#include<iostream>
#include<string>
#include<queue>
using namespace std;

typedef queue<string> TEXT;
const int LINEWIDTH=75;

int LineWordLength(TEXT line)
{
	int length=0;
	while(!line.empty())
	{
		length+=line.front().length();
		line.pop();
	}
	return length;
}

TEXT GetLine(TEXT& paragraph)
{
	TEXT line;
	int totalLength=0,wordNum=0;
	while(!paragraph.empty() &&
		totalLength+paragraph.front().length()+wordNum<=LINEWIDTH)
	{
		line.push(paragraph.front());
		totalLength+=paragraph.front().length();
		wordNum++;
		paragraph.pop();
	}
	return line;
}

void PrintLine(TEXT line)
{
	while(true)
	{
		cout<<line.front();
		line.pop();
		if(line.empty())
			break;
		cout<<' ';
	}
	cout<<endl;
}

void AlignLeft(TEXT paragraph)
{
	while(!paragraph.empty())
	{
		TEXT line=GetLine(paragraph);
		PrintLine(line);
	}
}

void AlignRight(TEXT paragraph)
{
	while(!paragraph.empty())
	{
		TEXT line=GetLine(paragraph);
		for(int i=line.size()+LineWordLength(line)-1;i<LINEWIDTH;i++)
			cout<<' ';
		PrintLine(line);
	}
}

void AlignCenter(TEXT paragraph)
{
	while(!paragraph.empty())
	{
		TEXT line=GetLine(paragraph);
		for(int i=line.size()+LineWordLength(line)-1;i<LINEWIDTH-1;i+=2)
			cout<<' ';
		PrintLine(line);
	}
}

void AlignJustify(TEXT paragraph)
{
	while(!paragraph.empty())
	{
		TEXT line=GetLine(paragraph);
		if(paragraph.empty())
			PrintLine(line);
		else
		{
			int totalSpaceNum,averageSpaceNum,anotherSpaceNum;
			totalSpaceNum=LINEWIDTH-LineWordLength(line);
			averageSpaceNum=totalSpaceNum/(line.size()-1);
			anotherSpaceNum=totalSpaceNum%(line.size()-1);
			int outputCount=0;
			while(!line.empty())
			{
				cout<<line.front();
				line.pop();
				if(line.empty())
					break;
				for(int i=0;i<averageSpaceNum;i++)
					cout<<' ';
				if(outputCount<anotherSpaceNum)
					cout<<' ';
				outputCount++;
			}
			cout<<endl;
		}
	}
}

void main()
{
	char alignMode;
	cin>>alignMode;
	string word;
	TEXT paragraph;
	while(cin>>word)
		paragraph.push(word);
	switch(alignMode)
	{
	case 'L':AlignLeft(paragraph);break;
	case 'R':AlignRight(paragraph);break;
	case 'C':AlignCenter(paragraph);break;
	case 'J':AlignJustify(paragraph);break;
	default:break;
	}
}