#include<iostream.h>
#define MAX 100

struct Matrix{int iHeight,iWidth,iValue[MAX][MAX];};

Matrix Multiply(Matrix& mLeft,Matrix& mRight)
{
	Matrix mProduct;int i,j,k;
	mProduct.iHeight=mLeft.iHeight;mProduct.iWidth=mRight.iWidth;
	for(i=0;i<mProduct.iHeight;i++)for(j=0;j<mProduct.iWidth;j++)
		for(k=0,mProduct.iValue[i][j]=0;k<mLeft.iWidth;k++)
			mProduct.iValue[i][j]+=mLeft.iValue[i][k]*mRight.iValue[k][j];
	return mProduct;
}

void Input(Matrix& mIn)
{
	int i,j;
	cin>>mIn.iHeight>>mIn.iWidth;
	for(i=0;i<mIn.iHeight;i++)for(j=0;j<mIn.iWidth;j++)cin>>mIn.iValue[i][j];
}

void Output(Matrix& mOut)
{
	int i,j;
	for(i=0;i<mOut.iHeight;i++)
		{for(j=0;j<mOut.iWidth;j++)cout<<mOut.iValue[i][j]<<" ";cout<<endl;}
	cout<<endl;
}

void main()
{
	int iCaseNum,iFactorNum,i;Matrix mResult,mTemp;
	cin>>iCaseNum;
	while(iCaseNum--)
	{
		cin>>iFactorNum;Input(mResult);
		for(i=1;i<iFactorNum;i++){Input(mTemp);mResult=Multiply(mResult,mTemp);}
		Output(mResult);
	}
}