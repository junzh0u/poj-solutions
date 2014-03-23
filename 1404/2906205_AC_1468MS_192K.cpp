#include<iostream>
using namespace std;

const int KEY_NUM_MAX=90,LETTER_NUM_MAX=90,PRICE_INF=-1;

class Keyboard
{
public:
	Keyboard();
	bool Input();
	void Solve();
	void Output();
private:
	//static const int KEY_NUM_MAX=90,LETTER_NUM_MAX=90,PRICE_INF=-1;
	int keyNum,letterNum;
	char keyName[KEY_NUM_MAX+1];
	char letterName[LETTER_NUM_MAX+1];
	int sequence[LETTER_NUM_MAX];
	int priceMin[KEY_NUM_MAX][LETTER_NUM_MAX];
	int minLetter[KEY_NUM_MAX][LETTER_NUM_MAX];
	int letterInKey[KEY_NUM_MAX];
	int TryKey(int,int);
	int KeyPrice(int,int);
};

Keyboard::Keyboard()
{
}

bool Keyboard::Input()
{
	cin>>keyNum>>letterNum>>keyName>>letterName;
	for(int i=0;i<letterNum;i++)
		cin>>sequence[i];
	return true;
}

void Keyboard::Solve()
{
	for(int i=0;i<keyNum;i++)
		for(int j=0;j<letterNum;j++)
		{
			priceMin[i][j]=PRICE_INF;
			minLetter[i][j]=0;
		}
	TryKey(keyNum-1,letterNum);
	int key=keyNum-1,letter=letterNum-1;
	while(key>=0)
	{
		letterInKey[key]=minLetter[key][letter];
		letter-=letterInKey[key];
		key--;
	}
}

void Keyboard::Output()
{
	int letter=0;
	for(int key=0;key<keyNum;key++)
	{
		cout<<keyName[key]<<": ";
		for(int i=0;i<letterInKey[key];i++)
			cout<<letterName[letter++];
		cout<<endl;
	}
	cout<<endl;
}

int Keyboard::TryKey(int key,int letterNum)
{
	if(priceMin[key][letterNum-1]!=PRICE_INF)
		return priceMin[key][letterNum-1];
	if(key==0)
	{
		minLetter[key][letterNum-1]=letterNum;
		return priceMin[key][letterNum-1]=KeyPrice(letterNum-1,letterNum);
	}
	for(int letterUse=letterNum-key;letterUse>0;letterUse--)
	{
		int price=KeyPrice(letterNum-1,letterUse)+TryKey(key-1,letterNum-letterUse);
		if(priceMin[key][letterNum-1]==PRICE_INF || priceMin[key][letterNum-1]>price)
		{
			priceMin[key][letterNum-1]=price;
			minLetter[key][letterNum-1]=letterUse;
		}
	}
	return priceMin[key][letterNum-1];
}

int Keyboard::KeyPrice(int lastLetter,int letterNum)
{
	int price=0;
	for(int order=1;order<=letterNum;order++)
		price+=order*sequence[lastLetter-letterNum+order];
	return price;
}

int main()
{
	int caseNum;
	cin>>caseNum;
	for(int code=1;code<=caseNum;code++)
	{
		Keyboard testCase;
		testCase.Input();
		testCase.Solve();
		cout<<"Keypad #"<<code<<":"<<endl;
		testCase.Output();
	}
	return 0;
}