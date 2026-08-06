#include<cstdio>
#include<cmath>
#include<string>
#include<iostream>
using namespace std;

#define CAPACITY 100

class HugeInteger
{
	friend ostream& operator<<(ostream&,HugeInteger&);
	friend istream& operator>>(istream&,HugeInteger&);
	friend HugeInteger operator+(int,HugeInteger&);
	friend HugeInteger operator-(int,HugeInteger&);
	friend HugeInteger operator*(int,HugeInteger&);
	friend HugeInteger operator/(int,HugeInteger&);
public:
	HugeInteger operator+(HugeInteger&);HugeInteger operator+(int);
	HugeInteger operator-(HugeInteger&);HugeInteger operator-(int);
	HugeInteger operator*(HugeInteger&);HugeInteger operator*(int);
	HugeInteger operator/(HugeInteger&);HugeInteger operator/(int);
	HugeInteger operator++();HugeInteger operator++(int);
	HugeInteger operator--();HugeInteger operator--(int);
	operator int();
	HugeInteger(int=0);
	HugeInteger(string);
	void Input();
	void Output();
	void Add(HugeInteger);void Add(int);
	void Subtract(HugeInteger);void Subtract(int);
	void Multiply(HugeInteger);void Multiply(int);
	void Divide(HugeInteger);void Divide(int);
	void Modulus(HugeInteger);void Modulus(int);
	bool IsEqualTo(HugeInteger);bool IsEqualTo(int);
	bool IsNotEqualTo(HugeInteger);bool IsNotEqualTo(int);
	bool IsGreaterThan(HugeInteger);bool IsGreaterThan(int);
	bool IsLessThan(HugeInteger);bool IsLessThan(int);
	bool IsGreaterThanOrEqualTo(HugeInteger);bool IsGreaterThanOrEqualTo(int);
	bool IsLessThanOrEqualTo(HugeInteger);bool IsLessThanOrEqualTo(int);
	bool IsZero();
	bool IsNegative();
	void Set(int);
	void Set(string);
private:
	void Round();
	void RoundNegative();
	int iDigit;
	int iArray[CAPACITY];
};

HugeInteger::HugeInteger(int iValue)
{
	memset(iArray,0,sizeof(int)*CAPACITY);
	Set(iValue);
}

HugeInteger::HugeInteger(string strValue)
{
	memset(iArray,0,sizeof(int)*CAPACITY);
	Set(strValue);
}

void HugeInteger::Input()
{
	string strValue;
	cin>>strValue;
	Set(strValue);
}

void HugeInteger::Output()
{
	int i;
	if(iDigit==0)
	{
		cout<<"0"<<endl;
		return;
	}
	if(IsNegative())
	{
		cout<<"-";
		for(i=iDigit-1;i>=0;i--)
			cout<<0-iArray[i];
	}
	else
		for(i=iDigit-1;i>=0;i--)
			cout<<iArray[i];
	cout<<endl;
}

void HugeInteger::Add(HugeInteger hiAddend)
{
	HugeInteger hiSum;
	for(int i=0;i<iDigit || i<hiAddend.iDigit;i++)
		iArray[i]+=hiAddend.iArray[i];
	Round();
	if(abs(iArray[CAPACITY-1])>=10)
	{
		Set(0);
		cerr<<"Data Overflowed!"<<endl;
	}
}

void HugeInteger::Add(int iAddend)
{
	if(abs(iAddend+iArray[0])<=LONG_MAX)
	{
		iArray[0]+=iAddend;
		Round();
	}
	else
		Add(HugeInteger(iAddend));
}

void HugeInteger::Subtract(HugeInteger hiSubtrahend)
{
	hiSubtrahend.Multiply(-1);
	Add(hiSubtrahend);
}

void HugeInteger::Subtract(int iSubtrahend)
{
	Add(0-iSubtrahend);
}

void HugeInteger::Multiply(HugeInteger hiMultiplier)
{
	int i,j,iTemp[CAPACITY];
	if(iDigit+hiMultiplier.iDigit>CAPACITY+1)
	{
		cerr<<"Data Overflowed!"<<endl;
		Set(0);
		return;
	}
	memset(iTemp,0,sizeof(int)*CAPACITY);
	for(i=0;i<hiMultiplier.iDigit;i++)
		for(j=0;j<iDigit;j++)
			iTemp[i+j]+=hiMultiplier.iArray[i]*iArray[j];
	memcpy(iArray,iTemp,sizeof(int)*CAPACITY);
	Round();
	if(abs(iArray[CAPACITY-1])>=10)
	{
		cerr<<"Data Overflowed!"<<endl;
		Set(0);
	}
}

void HugeInteger::Multiply(int iMultiplier)
{
	if(abs(iMultiplier*9)<=LONG_MAX)
	{
		for(int i=0;i<iDigit;i++)
			iArray[i]*=iMultiplier;
		Round();
	}
	else
		Multiply(HugeInteger(iMultiplier));
}

void HugeInteger::Divide(HugeInteger hiDivisor)
{
	HugeInteger hiQuotient(0),hiProduct(0);
	int i;
	bool bNegative=false;
	if(hiDivisor.IsZero())
	{
		cerr<<"Zero Divisor!"<<endl;
		return;
	}
	if(IsNegative())
	{
		Multiply(-1);
		bNegative=!bNegative;
	}
	if(hiDivisor.IsNegative())
	{
		hiDivisor.Multiply(-1);
		bNegative=!bNegative;
	}
	if(IsLessThan(hiDivisor))
	{
		Set(0);
		return;
	}
	hiQuotient.iDigit=iDigit-hiDivisor.iDigit+1;
	for(i=hiQuotient.iDigit-1;i>=0;i--)
	{
		hiProduct.Set(0);
		hiProduct.iDigit=i+1;
		hiProduct.iArray[i]=1;
		hiProduct.Multiply(hiDivisor);
		while(IsGreaterThanOrEqualTo(hiProduct))
		{
			hiQuotient.iArray[i]++;
			Subtract(hiProduct);
		}
	}
	hiQuotient.Round();
	(*this)=hiQuotient;
	Round();
	if(bNegative)
		Multiply(-1);
}

void HugeInteger::Divide(int iDivisor)
{
	Divide(HugeInteger(iDivisor));
}

void HugeInteger::Modulus(HugeInteger hiModulus)
{
	HugeInteger hiTemp;
	hiTemp=(*this);
	hiTemp.Divide(hiModulus);
	hiTemp.Multiply(hiModulus);
	Subtract(hiTemp);
}

void HugeInteger::Modulus(int iModulus)
{
	Modulus(HugeInteger(iModulus));
}

bool HugeInteger::IsEqualTo(HugeInteger hiRight)
{
	if(iDigit!=hiRight.iDigit)
		return false;
	for(int i=0;i<iDigit;i++)
		if(iArray[i]!=hiRight.iArray[i])
			return false;
	return true;
}

bool HugeInteger::IsEqualTo(int iRight)
{
	return IsEqualTo(HugeInteger(iRight));
}

bool HugeInteger::IsNotEqualTo(HugeInteger hiRight)
{
	return !IsEqualTo(hiRight);
}

bool HugeInteger::IsNotEqualTo(int iRight)
{
	return IsNotEqualTo(HugeInteger(iRight));
}

bool HugeInteger::IsGreaterThan(HugeInteger hiRight)
{
	if(IsNegative()!=hiRight.IsNegative())
		return !IsNegative();
	if(iDigit!=hiRight.iDigit)
		return IsNegative()!=(iDigit>hiRight.iDigit);
	for(int i=iDigit-1;i>=0;i--)
		if(iArray[i]!=hiRight.iArray[i])
			return IsNegative()!=(iArray[i]>hiRight.iArray[i]);
	return false;
}

bool HugeInteger::IsGreaterThan(int iRight)
{
	return IsGreaterThan(HugeInteger(iRight));
}

bool HugeInteger::IsLessThan(HugeInteger hiRight)
{
	if(IsNegative()!=hiRight.IsNegative())
		return IsNegative();
	if(iDigit!=hiRight.iDigit)
		return IsNegative()!=(iDigit<hiRight.iDigit);
	for(int i=iDigit-1;i>=0;i--)
		if(iArray[i]!=hiRight.iArray[i])
			return IsNegative()!=(iArray[i]<hiRight.iArray[i]);
	return false;
}

bool HugeInteger::IsLessThan(int iRight)
{
	return IsLessThan(HugeInteger(iRight));
}

bool HugeInteger::IsGreaterThanOrEqualTo(HugeInteger hiRight)
{
	return IsEqualTo(hiRight) || IsGreaterThan(hiRight);
}

bool HugeInteger::IsGreaterThanOrEqualTo(int iRight)
{
	return IsGreaterThanOrEqualTo(HugeInteger(iRight));
}

bool HugeInteger::IsLessThanOrEqualTo(HugeInteger hiRight)
{
	return IsEqualTo(hiRight) || IsLessThan(hiRight);
}

bool HugeInteger::IsLessThanOrEqualTo(int iRight)
{
	return IsLessThanOrEqualTo(HugeInteger(iRight));
}

bool HugeInteger::IsZero()
{
	return iDigit==0;
}

bool HugeInteger::IsNegative()
{
	return iArray[iDigit-1]<0;
}

void HugeInteger::Set(int iValue)
{
	memset(iArray,0,sizeof(int)*CAPACITY);
	iDigit=0;
	iArray[0]=iValue;
	Round();
	if(iArray[CAPACITY-1]>=10)
	{
		memset(iArray,0,sizeof(int)*CAPACITY);
		iDigit=0;
		cerr<<"Data Overflowed!"<<endl;
	}
}

void HugeInteger::Set(string strValue)
{
	memset(iArray,0,sizeof(int)*CAPACITY);
	iDigit=0;
	int iLen,i;
	bool bNegative;
	iLen=strValue.length();
	bNegative=(strValue[0]=='-' && iLen!=1);
	if(iLen>CAPACITY+(bNegative?1:0))
	{
		cerr<<"Data Overflowed!"<<endl;
		return;
	}
	for(i=(bNegative?1:0);i<iLen;i++)
	{
		if(isdigit(strValue[i]))
			iArray[iLen-1-i]=(strValue[i]-'0')*(bNegative?(-1):1);
		else
		{
			memset(iArray,0,sizeof(int)*CAPACITY);
			cerr<<"Unexpected Character!"<<endl;
			return;
		}
	}
	Round();
}

void HugeInteger::Round()
{
	for(int i=0;i<CAPACITY-1;i++)
	{
		if(abs(iArray[i])>=10)
		{
			iArray[i+1]+=iArray[i]/10;
			iArray[i]%=10;
		}
		if(iArray[i]<0)
		{
			iArray[i]+=10;
			iArray[i+1]--;
		}
	}
	for(iDigit=CAPACITY;iDigit>0;iDigit--)
		if(iArray[iDigit-1]!=0)
			break;
	if(IsNegative())
		RoundNegative();
}

void HugeInteger::RoundNegative()
{
	for(int i=0;i<CAPACITY-1;i++)
		if(iArray[i]>0)
		{
			iArray[i]-=10;
			iArray[i+1]++;
		}
	for(iDigit=CAPACITY;iDigit>0;iDigit--)
		if(iArray[iDigit-1]!=0)
			break;
}

HugeInteger HugeInteger::operator+(HugeInteger& hiLeft)
{
	HugeInteger hiResult=(*this);
	hiResult.Add(hiLeft);
	return hiResult;
}

HugeInteger HugeInteger::operator+(int iLeft)
{
	HugeInteger hiResult=(*this);
	hiResult.Add(iLeft);
	return hiResult;
}

HugeInteger HugeInteger::operator-(HugeInteger& hiLeft)
{
	HugeInteger hiResult=(*this);
	hiResult.Subtract(hiLeft);
	return hiResult;
}

HugeInteger HugeInteger::operator-(int iLeft)
{
	HugeInteger hiResult=(*this);
	hiResult.Subtract(iLeft);
	return hiResult;
}

HugeInteger HugeInteger::operator*(HugeInteger& hiLeft)
{
	HugeInteger hiResult=(*this);
	hiResult.Multiply(hiLeft);
	return hiResult;
}

HugeInteger HugeInteger::operator*(int iLeft)
{
	HugeInteger hiResult=(*this);
	hiResult.Multiply(iLeft);
	return hiResult;
}

HugeInteger HugeInteger::operator/(HugeInteger& hiLeft)
{
	HugeInteger hiResult=(*this);
	hiResult.Divide(hiLeft);
	return hiResult;
}

HugeInteger HugeInteger::operator/(int iLeft)
{
	HugeInteger hiResult=(*this);
	hiResult.Divide(iLeft);
	return hiResult;
}

HugeInteger HugeInteger::operator++()
{
	Add(1);
	return *this;
}

HugeInteger HugeInteger::operator++(int iFlag)
{
	HugeInteger hiResult=(*this);
	Add(1);
	return hiResult;
}

HugeInteger HugeInteger::operator--()
{
	Subtract(1);
	return *this;
}

HugeInteger HugeInteger::operator--(int iFlag)
{
	HugeInteger hiResult=(*this);
	Subtract(1);
	return hiResult;
}

HugeInteger::operator int()
{
	int iTemp=1,iResult=0,i;
	for(i=0;i<iDigit;i++)
	{
		iResult+=iArray[i]*iTemp;
		iTemp*=10;
	}
	return iResult;
}

ostream& operator << (ostream& output,HugeInteger& hi)
{
	int i;
	if(hi.iDigit==0)
	{
		output<<"0"<<endl;
		return output;
	}
	if(hi.IsNegative())
	{
		output<<"-";
		for(i=hi.iDigit-1;i>=0;i--)
			output<<0-hi.iArray[i];
	}
	else
		for(i=hi.iDigit-1;i>=0;i--)
			output<<hi.iArray[i];
	return output;
}

istream &operator>>(istream &input,HugeInteger& hi)
{
	string strValue;
	input>>strValue;
	hi.Set(strValue);
	return input;
}

HugeInteger operator+(int iLeft,HugeInteger& hiRight)
{
	HugeInteger hiResult(iLeft);
	hiResult.Add(hiRight);
	return hiResult;
}

HugeInteger operator-(int iLeft,HugeInteger& hiRight)
{
	HugeInteger hiResult(iLeft);
	hiResult.Subtract(hiRight);
	return hiResult;
}

HugeInteger operator*(int iLeft,HugeInteger& hiRight)
{
	HugeInteger hiResult(iLeft);
	hiResult.Multiply(hiRight);
	return hiResult;
}

HugeInteger operator/(int iLeft,HugeInteger& hiRight)
{
	HugeInteger hiResult(iLeft);
	hiResult.Divide(hiRight);
	return hiResult;
}

void main()
{
	unsigned __int64 iPosition;int i;bool bFirst;HugeInteger hiPower[64];
	hiPower[63].Set(1);for(i=62;i>=0;i--)hiPower[i]=hiPower[i+1]*3;
	while(scanf("%I64d",&iPosition) && iPosition>0)
	{
		bFirst=true;printf("{");iPosition--;
		for(i=63;i>=0;i--,iPosition>>=1)
			if((1&iPosition)!=0)
				{if(!bFirst)cout<<",";cout<<" "<<hiPower[i];bFirst=false;}
		cout<<" }"<<endl;
	}
}