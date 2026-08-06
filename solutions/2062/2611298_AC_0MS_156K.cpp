#include<iostream>
#include<string>
#include<algorithm>
#include<cctype>
using namespace std;

typedef string CARD;

int Value(char value)
{
	if(isdigit(value))
		return value-'0';
	switch(value)
	{
	case 'T':return 10;
	case 'J':return 11;
	case 'Q':return 12;
	case 'K':return 13;
	case 'A':return 14;
	}
	return 0;
}

int Suit(char suit)
{
	switch(suit)
	{
	case 'H':return 4;
	case 'S':return 3;
	case 'D':return 2;
	case 'C':return 1;
	}
	return 0;
}

bool CardCmp(CARD left,CARD right)
{
	if(Value(left[0])!=Value(right[0]))
		return Value(left[0])>Value(right[0]);
	return Suit(left[1])>Suit(right[1]);
}

void main()
{
	int caseNum,i;
	const int CARDNUMMAX=26;
	cin>>caseNum;
	while(caseNum--)
	{
		int cardNum;
		cin>>cardNum;
		CARD Adam[CARDNUMMAX],Eve[CARDNUMMAX];
		for(i=0;i<cardNum;i++)
			cin>>Adam[i];
		sort(Adam,Adam+cardNum,CardCmp);
		//for(i=0;i<cardNum;i++)
		//	cout<<Adam[i];
		for(i=0;i<cardNum;i++)
			cin>>Eve[i];
		sort(Eve,Eve+cardNum,CardCmp);
		int adamPtr=0,evePtr=0,point=0;
		while(adamPtr<cardNum && evePtr<cardNum)
		{
			if(CardCmp(Eve[evePtr],Adam[adamPtr]))
			{
				evePtr++;
				point++;
			}
			adamPtr++;
		}
		cout<<point<<endl;
	}
}