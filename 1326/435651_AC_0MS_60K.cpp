//Mileage Bank

#include<iostream.h>
#include<math.h>
#include<string.h>
#include<ctype.h>

int calPerk(int,char);

int main()
{
	char chCode,chTemp;
	int iActMile,iTotalPerk=0;
	while(cin.peek()!='#')
	{
		while(cin.peek()!='0')
		{
			while(!isdigit(cin.peek()))
				cin.get(chTemp);
			cin>>iActMile>>chCode;
			iTotalPerk+=calPerk(iActMile,chCode);
			cin.ignore(1);
		}
		cout<<iTotalPerk<<endl;
		iTotalPerk=0;
		cin.ignore(2);
	}
	return 0;
}

int calPerk(int iActual,char chClass)
{
	int iPerk;
	if(chClass=='F' || chClass=='f')
		iPerk=2*iActual;
	else if(chClass=='B' || chClass=='b')
		iPerk=int(ceil(1.5*iActual));
	else if(chClass=='Y' || chClass=='y')
	{
		if(iActual<500)
			iPerk=500;
		else
			iPerk=iActual;
	}
	return iPerk;
}