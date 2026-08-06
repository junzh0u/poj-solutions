#include<iostream>
#include<map>
#include<string>
#include<cctype>
#include<cstdlib>
using namespace std;

typedef map<string,int> MSI;
MSI msiAtomTable;

void InputAtomTable()
{
	string strSymbol;int iWeight;
	while(cin>>strSymbol)
	{
		if(strSymbol.compare("END_OF_FIRST_PART")==0)return;
		cin>>iWeight;
		msiAtomTable.insert(MSI::value_type(strSymbol,iWeight));
	}
}

int MolecularWeight(string strFormula)
{
	int iDeque[50]={0},iDepth=0,i,iLast;const int iLen=strFormula.length();
	string strAtom,strNum;
	for(i=0;i<strFormula.length();i++)
	{
		if(isupper(strFormula[i]))
		{
			strAtom=strFormula[i];
			if(i<iLen-1 && islower(strFormula[i+1]))strAtom+=strFormula[++i];
			if(msiAtomTable.find(strAtom)==msiAtomTable.end())return -1;
			iDeque[iDepth]+=(iLast=msiAtomTable[strAtom]);
		}
		else if(isdigit(strFormula[i]))
		{
			strNum=strFormula[i];
			if(i<iLen-1 && isdigit(strFormula[i+1]))strNum+=strFormula[++i];
			iDeque[iDepth]+=iLast*(atoi(strNum.c_str())-1);
		}
		else if(strFormula[i]=='(')iDepth++;
		else if(strFormula[i]==')')
		{
			strNum=strFormula[++i];
			if(i<iLen-1 && isdigit(strFormula[i+1]))strNum+=strFormula[++i];
			iDeque[iDepth]*=atoi(strNum.c_str());	
			iDeque[iDepth-1]+=iDeque[iDepth];iDeque[iDepth--]=0;
		}
	}
	return iDeque[0];
}

void main()
{
	string strFormula;int iWeight;
	InputAtomTable();
	while(cin>>strFormula)
	{
		if(strFormula.compare("0")==0)return;
		iWeight=MolecularWeight(strFormula);
		if(iWeight<0)cout<<"UNKNOWN"<<endl;
		else cout<<iWeight<<endl;
	}
}